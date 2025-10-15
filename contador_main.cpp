#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cctype>

// A lista de palavras-chave 
const std::set<std::string> CPP_KEYWORDS = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor",
    "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t",
    "class", "compl", "concept", "const", "consteval", "constexpr", "constinit",
    "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype",
    "default", "delete", "do", "double", "dynamic_cast", "else", "enum",
    "explicit", "export", "extern", "false", "float", "for", "friend", "goto",
    "if", "inline", "int", "long", "mutable", "namespace", "new", "noexcept",
    "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private",
    "protected", "public", "register", "reinterpret_cast", "requires", "return",
    "short", "signed", "sizeof", "static", "static_assert", "static_cast",
    "struct", "switch", "template", "this", "thread_local", "throw", "true",
    "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
    "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq", 
    "#include", "#define", "#ifdef", "#ifndef", "#endif", "#if", "#else", "#elif"
};

// Lógica de contagem 
void processFile(const std::string& filename, std::map<std::string, unsigned long long>& freq_map) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro: Nao foi possivel abrir o arquivo " << filename << std::endl;
        return;
    }
    char c;
    std::string word_buffer;
    while (file.get(c)) {
        if (std::isalpha(c) || c == '_') {
            word_buffer += c;
            while (file.peek() != EOF && (std::isalnum(file.peek()) || file.peek() == '_')) { file.get(c); word_buffer += c; }
            if (CPP_KEYWORDS.count(word_buffer)) { freq_map[word_buffer]++; } 
            else { for (char letter : word_buffer) { freq_map[std::string(1, letter)]++; } }
            word_buffer.clear();
        } else if (c == '#') { 
             word_buffer += c;
             while (file.peek() != EOF && std::isalpha(file.peek())) { file.get(c); word_buffer += c; }
             if (CPP_KEYWORDS.count(word_buffer)) { freq_map[word_buffer]++; } 
             else { for (char letter : word_buffer) { freq_map[std::string(1, letter)]++; } }
            word_buffer.clear();
        } else {
            freq_map[std::string(1, c)]++;
        }
    }
}

std::string escapeString(const std::string& input) {
    std::string output = "";
    for (char c : input) {
        switch (c) {
            case '\"': output += "\\\""; break; // Aspas duplas
            case '\\': output += "\\\\"; break; // Barra invertida
            case '\n': output += "\\n";  break; // Quebra de linha
            case '\t': output += "\\t";  break; // Tabulação
            case '\r': output += "\\r";  break; // Retorno de carro
            default:   output += c;      break;
        }
    }
    return output;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: " << argv[0] << " <arquivo1.cpp> [arquivo2.cpp] ..." << std::endl;
        return 1;
    }

    std::map<std::string, unsigned long long> frequency_map;

    for (int i = 1; i < argc; ++i) {
        processFile(argv[i], frequency_map);
    }
    
    std::ofstream output_file("meu_mapa.txt");
    if (!output_file.is_open()) {
        std::cerr << "Erro ao criar o arquivo de saida meu_mapa.txt" << std::endl;
        return 1;
    }

    output_file << "// Tabela de Frequencia Gerada Automaticamente" << std::endl;
    output_file << "const std::map<std::string, unsigned long long> FIXED_FREQUENCY_MAP = {" << std::endl;
    for (const auto& pair : frequency_map) {
        // Usando a nova função de escape
        output_file << "    {\"" << escapeString(pair.first) << "\", " << pair.second << "}," << std::endl;
    }
    output_file << "};" << std::endl;
    
    output_file.close();
    std::cout << "Arquivo 'meu_mapa.txt' gerado com sucesso e com os caracteres especiais corrigidos." << std::endl;

    return 0;
}