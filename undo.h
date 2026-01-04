#ifndef UNDO_H
#define UNDO_H

#include <string>
#include <iostream>

enum ActionType {
    ACT_NONE = 0,
    ACT_SEND_REQUEST = 1,
    ACT_ACCEPT_REQUEST = 2
};

struct ActionRecord {
    ActionType type;
    std::string a; // fromId
    std::string b; // toId
};

class ActionUndoStack {
private:
    static const int MAX_STACK = 200;
    ActionRecord st[MAX_STACK];
    int top;

public:
    ActionUndoStack() : top(-1) {}

    bool isEmpty() const { return top < 0; }
    bool isFull() const { return top >= MAX_STACK - 1; }

    bool push(ActionType type, const std::string& a, const std::string& b) {
        if (isFull()) return false;
        st[++top] = ActionRecord{type, a, b};
        return true;
    }

    bool pop(ActionRecord& out) {
        if (isEmpty()) return false;
        out = st[top--];
        return true;
    }

    void showAll() const {
        std::cout << "\n===== STACK PEMBATALAN (UNDO) =====\n";
        if (isEmpty()) { std::cout << "(Kosong)\n"; return; }
        for (int i = top; i >= 0; --i) {
            std::cout << (top - i + 1) << ". ";
            if (st[i].type == ACT_SEND_REQUEST) std::cout << "SEND_REQUEST ";
            else if (st[i].type == ACT_ACCEPT_REQUEST) std::cout << "ACCEPT_REQUEST ";
            else std::cout << "UNKNOWN ";
            std::cout << st[i].a << " -> " << st[i].b << "\n";
        }
    }
};

#endif