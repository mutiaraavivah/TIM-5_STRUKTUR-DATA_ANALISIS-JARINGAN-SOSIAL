#ifndef NOTES_SLL_H
#define NOTES_SLL_H

#include <iostream>
#include <string>
using namespace std;

// Node untuk Circular Linked List (Catatan Jaringan)
struct NoteNode {
    string text;
    NoteNode* next;
};

struct NoteList {
    NoteNode* tail; // circular pakai tail

    NoteList() {
        tail = nullptr;
    }

    bool isEmpty() {
        return tail == nullptr;
    }

    void addNote(const string& t) {
        NoteNode* node = new NoteNode;
        node->text = t;

        if (isEmpty()) {
            tail = node;
            node->next = tail; // circular ke diri sendiri
        } else {
            node->next = tail->next; // masuk setelah tail (ke head)
            tail->next = node;
            tail = node; // node baru jadi tail (di akhir)
        }
        cout << "Catatan ditambahkan ke daftar.\n";
    }

    void showNotes() {
        if (isEmpty()) {
            cout << "(Belum ada catatan)\n";
            return;
        }
        cout << "\n=== DAFTAR CATATAN JARINGAN ===\n";

        NoteNode* head = tail->next;
        NoteNode* temp = head;
        int i = 1;

        do {
            cout << i++ << ". " << temp->text << endl;
            temp = temp->next;
        } while (temp != head);
    }

    void clearAll() {
        if (isEmpty()) {
            cout << "Semua catatan dihapus.\n";
            return;
        }

        NoteNode* head = tail->next;
        NoteNode* temp = head;

        do {
            NoteNode* del = temp;
            temp = temp->next;
            delete del;
        } while (temp != head);

        tail = nullptr;
        cout << "Semua catatan dihapus.\n";
    }
};

// Menu untuk mengelola NoteList (CIRCULAR LINKED LIST)
void notesMenu() {
    NoteList notes;
    int pilih;
    string teks;

    while (true) {
        cout << "\n===== MENU CATATAN JARINGAN (SINGLY LINKED LIST) =====\n";
        cout << "1. Tambah catatan\n";
        cout << "2. Lihat semua catatan\n";
        cout << "3. Hapus semua catatan\n";
        cout << "0. Kembali ke Menu Fitur Tambahan\n";
        cout << "Pilih: ";

        if (!(cin >> pilih)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (pilih == 1) {
            cout << "Tulis catatan: ";
            getline(cin, teks);
            notes.addNote(teks);
        }
        else if (pilih == 2) {
            notes.showNotes();
        }
        else if (pilih == 3) {
            notes.clearAll();
        }
        else if (pilih == 0) {
            cout << "Kembali ke menu fitur tambahan...\n";
            break;
        }
        else {
            cout << "Pilihan tidak valid.\n";
        }

        cout << "\n(Press ENTER untuk lanjut)";
        cin.get();
    }
}

#endif