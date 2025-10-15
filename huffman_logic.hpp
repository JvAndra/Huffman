#ifndef HUFFMAN_LOGIC_HPP
#define HUFFMAN_LOGIC_HPP

#include <queue>
#include <vector>
#include <map>
#include <set>         
#include <fstream>
#include <iostream>
#include <bitset>      
#include <cctype>      
#include "huffman_tree.hpp"

// Constrói a Árvore de Huffman a partir de um mapa de frequência
std::shared_ptr<Node> buildHuffmanTree(const std::map<std::string, unsigned long long>& freq_map) {
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, CompareNodes> pq;
    for (const auto& pair : freq_map) {
        pq.push(std::make_shared<Node>(pair.first, pair.second));
    }

    if (pq.size() <= 1) {
        auto leaf = pq.empty() ? nullptr : pq.top();
        return std::make_shared<Node>(leaf, nullptr);
    }

    while (pq.size() > 1) {
        std::shared_ptr<Node> left = pq.top();
        pq.pop();
        std::shared_ptr<Node> right = pq.top();
        pq.pop();
        auto parent = std::make_shared<Node>(left, right);
        pq.push(parent);
    }
    return pq.top();
}

// Função auxiliar recursiva para gerar os códigos
void generateCodesHelper(
    const std::shared_ptr<Node>& node,
    const std::string& current_code,
    std::map<std::string, std::string>& huffman_codes) {

    if (!node) return;
    if (node->isLeaf()) {
        huffman_codes[node->symbol] = current_code.empty() ? "0" : current_code;
        return;
    }
    generateCodesHelper(node->left, current_code + "0", huffman_codes);
    generateCodesHelper(node->right, current_code + "1", huffman_codes);
}

// Gera o mapa de códigos a partir da árvore
std::map<std::string, std::string> generateCodes(const std::shared_ptr<Node>& root) {
    std::map<std::string, std::string> huffman_codes;
    if (root) {
        generateCodesHelper(root, "", huffman_codes);
    }
    return huffman_codes;
}

void compressFile(const std::string& input_filename, const std::string& output_filename,
                  const std::map<std::string, std::string>& huffman_codes,
                  const std::set<std::string>& keywords) {

    std::ifstream input_file(input_filename);
    if (!input_file.is_open()) {
        std::cerr << "Erro ao abrir arquivo de entrada: " << input_filename << std::endl;
        return;
    }

    std::ofstream output_file(output_filename, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Erro ao criar arquivo de saida: " << output_filename << std::endl;
        return;
    }

    std::string bit_string = "";
    unsigned long long symbol_count = 0;
    
    char c;
    std::string word_buffer;
    while (input_file.get(c)) {
        std::string symbol_to_encode = "";
        
        if (std::isalpha(c) || c == '_') {
            word_buffer += c;
            while (input_file.peek() != EOF && (std::isalnum(input_file.peek()) || input_file.peek() == '_')) {
                input_file.get(c);
                word_buffer += c;
            }
            if (keywords.count(word_buffer)) {
                symbol_to_encode = word_buffer;
            } else {
                for (char letter : word_buffer) {
                    std::string sym(1, letter);
                    if (huffman_codes.count(sym)) {
                        bit_string += huffman_codes.at(sym);
                        symbol_count++;
                    }
                }
            }
            word_buffer.clear();
        } else if (c == '#') {
             word_buffer += c;
             while (input_file.peek() != EOF && std::isalpha(input_file.peek())) {
                input_file.get(c);
                word_buffer += c;
            }
            if (keywords.count(word_buffer)) {
                symbol_to_encode = word_buffer;
            } else {
                 for (char letter : word_buffer) {
                    std::string sym(1, letter);
                     if (huffman_codes.count(sym)) {
                        bit_string += huffman_codes.at(sym);
                        symbol_count++;
                    }
                }
            }
            word_buffer.clear();
        } else {
            symbol_to_encode = std::string(1, c);
        }

        if (!symbol_to_encode.empty() && huffman_codes.count(symbol_to_encode)) {
            bit_string += huffman_codes.at(symbol_to_encode);
            symbol_count++;
        }
    }
    input_file.close();

    // Escreve os metadados 
    output_file.write(reinterpret_cast<const char*>(&symbol_count), sizeof(symbol_count));

    while (bit_string.length() >= 8) {
        std::string byte_str = bit_string.substr(0, 8);
        bit_string = bit_string.substr(8);
        char byte = static_cast<char>(std::stoul(byte_str, nullptr, 2));
        output_file.write(&byte, 1);
    }
    
    if (!bit_string.empty()) {
        bit_string.append(8 - bit_string.length(), '0'); // Padding
        char byte = static_cast<char>(std::stoul(bit_string, nullptr, 2));
        output_file.write(&byte, 1);
    }

    output_file.close();
    std::cout << "Arquivo '" << input_filename << "' comprimido com sucesso para '" << output_filename << "'" << std::endl;
}


void decompressFile(const std::string& input_filename, const std::string& output_filename,
                    const std::shared_ptr<Node>& root) {
    
    std::ifstream input_file(input_filename, std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Erro ao abrir arquivo de entrada: " << input_filename << std::endl;
        return;
    }

    std::ofstream output_file(output_filename);
    if (!output_file.is_open()) {
        std::cerr << "Erro ao criar arquivo de saida: " << output_filename << std::endl;
        return;
    }

    if (!root) {
        std::cerr << "Erro: Arvore de Huffman invalida." << std::endl;
        return;
    }

    // Lê os metadados para saber quando parar
    unsigned long long total_symbol_count;
    input_file.read(reinterpret_cast<char*>(&total_symbol_count), sizeof(total_symbol_count));

    std::string bit_string = "";
    char byte;
    while (input_file.get(byte)) {
        bit_string += std::bitset<8>(byte).to_string();
    }

    unsigned long long decoded_symbols = 0;
    std::shared_ptr<Node> current_node = root;
    for (char bit : bit_string) {
        if (decoded_symbols >= total_symbol_count) {
            break; 
        }

        current_node = (bit == '0') ? current_node->left : current_node->right;

        if (current_node->isLeaf()) {
            output_file << current_node->symbol;
            decoded_symbols++;
            current_node = root;
        }
    }

    input_file.close();
    output_file.close();

    std::cout << "Arquivo '" << input_filename << "' descomprimido com sucesso para '" << output_filename << "'" << std::endl;
}

#endif 