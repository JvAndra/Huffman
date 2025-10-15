#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include <string>
#include <memory> 

struct Node {
    std::string symbol;
    unsigned long long frequency;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;

    // Construtor para nós-folha 
    Node(std::string sym, unsigned long long freq)
        : symbol(sym), frequency(freq), left(nullptr), right(nullptr) {}

    // Construtor para nós internos 
    Node(std::shared_ptr<Node> l, std::shared_ptr<Node> r)
        : symbol(""), frequency(l->frequency + r->frequency), left(l), right(r) {}

    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

// Comparador para a std::priority_queue se comportar como uma min-heap
struct CompareNodes {
    bool operator()(const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const {
        return a->frequency > b->frequency;
    }
};

#endif 