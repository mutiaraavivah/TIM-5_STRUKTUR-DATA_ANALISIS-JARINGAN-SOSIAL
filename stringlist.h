#ifndef STRINGLIST_H
#define STRINGLIST_H

#include <string>
using namespace std;

// Pengganti sederhana untuk std::vector<string>
class StringList {
private:
    string* data;
    int n;
    int cap;

    void grow() {
        int newCap = (cap == 0) ? 4 : cap * 2;
        string* nd = new string[newCap];
        for (int i = 0; i < n; i++) nd[i] = data[i];
        delete[] data;
        data = nd;
        cap = newCap;
    }

public:
    StringList() : data(nullptr), n(0), cap(0) {}

    StringList(const StringList& other) : data(nullptr), n(0), cap(0) {
        for (int i = 0; i < other.n; i++) push_back(other.data[i]);
    }

    StringList& operator=(const StringList& other) {
        if (this == &other) return *this;
        delete[] data;
        data = nullptr; n = 0; cap = 0;
        for (int i = 0; i < other.n; i++) push_back(other.data[i]);
        return *this;
    }

    ~StringList() { delete[] data; }

    int size() const { return n; }
    bool empty() const { return n == 0; }

    string& operator[](int i) { return data[i]; }
    const string& operator[](int i) const { return data[i]; }

    void push_back(const string& s) {
        if (n >= cap) grow();
        data[n++] = s;
    }

    bool contains(const string& s) const {
        for (int i = 0; i < n; i++) if (data[i] == s) return true;
        return false;
    }

    bool eraseAt(int idx) {
        if (idx < 0 || idx >= n) return false;
        for (int i = idx; i < n - 1; i++) data[i] = data[i + 1];
        n--;
        return true;
    }
};

#endif