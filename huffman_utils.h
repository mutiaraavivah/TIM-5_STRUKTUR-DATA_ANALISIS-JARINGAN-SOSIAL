#ifndef HUFFMAN_UTILS_H
#define HUFFMAN_UTILS_H

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

struct HuffmanResult {
    long long originalBits;
    long long encodedBits;
    double compressionRatio;
};

namespace Huffman {
    struct Node {
        char ch;
        long long freq;
        Node* left;
        Node* right;
        Node(char c, long long f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
        Node(Node* a, Node* b) : ch('\0'), freq(a->freq + b->freq), left(a), right(b) {}
    };

    struct Cmp {
        bool operator()(const Node* a, const Node* b) const { return a->freq > b->freq; }
    };

    inline void buildCodes(Node* node, const std::string& prefix, std::unordered_map<char, int>& lens) {
        if (!node) return;
        if (!node->left && !node->right) {
            lens[node->ch] = (prefix.empty() ? 1 : (int)prefix.size());
            return;
        }
        buildCodes(node->left, prefix + "0", lens);
        buildCodes(node->right, prefix + "1", lens);
    }

    inline void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    inline HuffmanResult analyze(const std::string& text) {
        HuffmanResult res{0, 0, 1.0};
        if (text.empty()) return res;

        std::unordered_map<char, long long> freq;
        for (char c : text) freq[c]++;

        std::priority_queue<Node*, std::vector<Node*>, Cmp> pq;
        for (auto& kv : freq) pq.push(new Node(kv.first, kv.second));

        while (pq.size() > 1) {
            Node* a = pq.top(); pq.pop();
            Node* b = pq.top(); pq.pop();
            pq.push(new Node(a, b));
        }

        Node* root = pq.top();
        std::unordered_map<char, int> lens;
        buildCodes(root, "", lens);

        res.originalBits = (long long)text.size() * 8LL;
        res.encodedBits = 0;
        for (auto& kv : freq) res.encodedBits += kv.second * (long long)lens[kv.first];
        res.compressionRatio = (res.originalBits == 0) ? 1.0 : (double)res.encodedBits / (double)res.originalBits;

        destroy(root);
        return res;
    }
}

#endif