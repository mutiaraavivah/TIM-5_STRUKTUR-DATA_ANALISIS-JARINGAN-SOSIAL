#ifndef CIRCLEFRIENDS_H
#define CIRCLEFRIENDS_H

#include <iostream>
#include <string>
using namespace std;

struct FriendNode {
    string friendName;
    FriendNode* next;
};

struct FriendCircle {
    FriendNode* tail;

    FriendCircle() {
        tail = nullptr;
    }

    bool isEmpty() {
        return tail == nullptr;
    }

    void addFriend(const string& name) {
        FriendNode* newNode = new FriendNode;
        newNode->friendName = name;
        if (isEmpty()) {
            tail = newNode;
            newNode->next = tail;
        } else {
            newNode->next = tail->next;
            tail->next = newNode;
            tail = newNode;
        }
        cout << "Teman " << name << " ditambahkan ke lingkaran.\n";
    }

    void showCircle() {
        if (isEmpty()) {
            cout << "(Lingkaran kosong)\n";
            return;
        }
        FriendNode* temp = tail->next;
        cout << "Lingkaran Pertemanan:\n";
        do {
            cout << "- " << temp->friendName << endl;
            temp = temp->next;
        } while (temp != tail->next);
    }

    void removeFriend(const string& name) {
        if (isEmpty()) {
            cout << "Lingkaran kosong.\n";
            return;
        }

        FriendNode* current = tail->next;
        FriendNode* prev = tail;
        do {
            if (current->friendName == name) {
                if (current == tail && current->next == tail) {
                    delete current;
                    tail = nullptr;
                } else {
                    prev->next = current->next;
                    if (current == tail)
                        tail = prev;
                    delete current;
                }
                cout << "Teman " << name << " dihapus dari lingkaran.\n";
                return;
            }
            prev = current;
            current = current->next;
        } while (current != tail->next);

        cout << "Teman tidak ditemukan.\n";
    }

    void clearCircle() {
        if (isEmpty()) return;
        FriendNode* temp = tail->next;
        while (temp != tail) {
            FriendNode* del = temp;
            temp = temp->next;
            delete del;
        }
        delete tail;
        tail = nullptr;
    }
};

void circleFriendsMenu() {
    FriendCircle circle;
    int choice;
    string name;

    while (true) {
        cout << "\n===== MENU LINGKARAN PERTEMANAN =====\n";
        cout << "1. Tambah Teman ke Lingkaran\n";
        cout << "2. Tampilkan Lingkaran\n";
        cout << "3. Hapus Teman dari Lingkaran\n";
        cout << "4. Hapus Seluruh Lingkaran\n";
        cout << "0. Kembali ke Menu Tambahan\n";
        cout << "Pilih: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            cout << "Masukkan nama teman: ";
            getline(cin, name);
            circle.addFriend(name);
        } 
        else if (choice == 2) {
            circle.showCircle();
        } 
        else if (choice == 3) {
            cout << "Masukkan nama yang ingin dihapus: ";
            getline(cin, name);
            circle.removeFriend(name);
        } 
        else if (choice == 4) {
            circle.clearCircle();
            cout << "Lingkaran dihapus seluruhnya.\n";
        } 
        else if (choice == 0) {
            cout << "Kembali ke menu tambahan...\n";
            break;
        } 
        else cout << "Pilihan tidak valid.\n";
    }
}

#endif