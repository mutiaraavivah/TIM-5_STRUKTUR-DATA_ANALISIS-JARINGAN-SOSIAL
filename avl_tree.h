#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <string>

class AVLTree {
private:
    struct Node {
        std::string key;
        int height;
        Node* left;
        Node* right;
        explicit Node(const std::string& k) : key(k), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    static int h(Node* n) { return n ? n->height : 0; }
    static int max2(int a, int b) { return (a > b) ? a : b; }
    static int balance(Node* n) { return n ? (h(n->left) - h(n->right)) : 0; }

    static Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max2(h(y->left), h(y->right)) + 1;
        x->height = max2(h(x->left), h(x->right)) + 1;
        return x;
    }

    static Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max2(h(x->left), h(x->right)) + 1;
        y->height = max2(h(y->left), h(y->right)) + 1;
        return y;
    }

    Node* insert(Node* node, const std::string& key, std::string& rotationNote) {
        if (!node) return new Node(key);

        if (key < node->key) node->left = insert(node->left, key, rotationNote);
        else if (key > node->key) node->right = insert(node->right, key, rotationNote);
        else return node;

        node->height = 1 + max2(h(node->left), h(node->right));
        int bf = balance(node);

        if (bf > 1 && key < node->left->key) {
            rotationNote = "LL rotation at key: " + node->key;
            return rotateRight(node);
        }
        if (bf < -1 && key > node->right->key) {
            rotationNote = "RR rotation at key: " + node->key;
            return rotateLeft(node);
        }
        if (bf > 1 && key > node->left->key) {
            rotationNote = "LR rotation at key: " + node->key;
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (bf < -1 && key < node->right->key) {
            rotationNote = "RL rotation at key: " + node->key;
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    static void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    std::string insert(const std::string& key) {
        std::string note;
        root = insert(root, key, note);
        return note;
    }
};

#endif