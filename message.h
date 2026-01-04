#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include <ctime>
#include "relation.h"
using namespace std;

// =============================
// Node Pesan (Circular Linked List)
// =============================
struct Message {
    string sender;
    string receiver;
    string content;
    string timestamp;

    Message* next;  // circular: hanya next

    Message(const string& s, const string& r, const string& c) {
        sender = s;
        receiver = r;
        content = c;
        time_t now = time(0);
        timestamp = ctime(&now);
        if (!timestamp.empty() && timestamp.back() == '\n')
            timestamp.pop_back();
        next = nullptr;
    }
};

// =============================
// Kotak Pesan (Circular Linked List)
// =============================
struct MessageBox {
    Message* tail;   // tail = pesan terakhir (paling baru)
    int count;       // jumlah pesan (biar traversal reverse bisa)

    MessageBox() {
        tail = nullptr;
        count = 0;
    }

    bool isEmpty() const {
        return tail == nullptr;
    }

    Message* head() const {
        if (!tail) return nullptr;
        return tail->next;
    }

    void addMessage(const string& sender, const string& receiver, const string& content) {
        Message* newMsg = new Message(sender, receiver, content);

        if (!tail) {
            tail = newMsg;
            newMsg->next = tail; // circular ke diri sendiri
        } else {
            newMsg->next = tail->next; // new -> head
            tail->next = newMsg;       // tail -> new
            tail = newMsg;             // new jadi tail (terbaru)
        }
        count++;
    }

    void showMessages() {
        if (!tail) {
            cout << "(Tidak ada pesan)\n";
            return;
        }

        Message* temp = tail->next; // head (paling lama)
        int index = 1;
        for (int i = 0; i < count; i++) {
            cout << index++ << ". Dari: " << temp->sender
                 << " | Untuk: " << temp->receiver << endl;
            cout << "   Isi: " << temp->content << endl;
            cout << "   Waktu: " << temp->timestamp << "\n\n";
            temp = temp->next;
        }
    }

    // tampil dari terbaru -> terlama (tanpa prev, tapi output tetap sama)
    void showMessagesReverse() {
        if (!tail) {
            cout << "(Tidak ada pesan)\n";
            return;
        }

        int index = 1;
        // cara aman tanpa vector: untuk posisi k dari belakang, cari node itu dari head
        for (int k = count; k >= 1; --k) {
            Message* temp = tail->next; // head
            for (int step = 1; step < k; ++step) {
                temp = temp->next;
            }

            cout << index++ << ". Dari: " << temp->sender
                 << " | Untuk: " << temp->receiver << endl;
            cout << "   Isi: " << temp->content << endl;
            cout << "   Waktu: " << temp->timestamp << "\n\n";
        }
    }

    void deleteMessage(int index) {
        if (!tail) {
            cout << "Tidak ada pesan untuk dihapus.\n";
            return;
        }

        if (index < 1 || index > count) {
            cout << "Indeks pesan tidak valid.\n";
            return;
        }

        Message* cur = tail->next; // head
        Message* prev = tail;

        // cari node ke-index
        for (int i = 1; i < index; i++) {
            prev = cur;
            cur = cur->next;
        }

        // jika hanya satu node
        if (count == 1) {
            delete cur;
            tail = nullptr;
            count = 0;
            cout << "Pesan ke-" << index << " dihapus.\n";
            return;
        }

        // hapus cur
        prev->next = cur->next;

        // kalau yang dihapus adalah tail, update tail
        if (cur == tail) {
            tail = prev;
        }

        delete cur;
        count--;
        cout << "Pesan ke-" << index << " dihapus.\n";
    }

    void clearAll() {
        if (!tail) {
            cout << "Semua pesan dihapus.\n";
            return;
        }

        Message* h = tail->next;
        Message* temp = h;

        for (int i = 0; i < count; i++) {
            Message* del = temp;
            temp = temp->next;
            delete del;
        }

        tail = nullptr;
        count = 0;
        cout << "Semua pesan dihapus.\n";
    }
};

// =============================
// Sistem Pesan Antar User
// =============================
struct MessagingSystem {
    Relation* network;
    MessageBox inbox;
    MessageBox sentBox;

    MessagingSystem(Relation* net) {
        network = net;
    }

    void sendMessage() {
        string from, to, msg;
        cout << "Masukkan ID pengirim: ";
        getline(cin, from);
        cout << "Masukkan ID penerima: ";
        getline(cin, to);

        User* sender = network->getUser(from);
        User* receiver = network->getUser(to);
        if (!sender || !receiver) {
            cout << "Salah satu user tidak ditemukan.\n";
            return;
        }

        cout << "Ketik pesan: ";
        getline(cin, msg);

        sentBox.addMessage(from, to, msg);
        inbox.addMessage(from, to, msg);

        cout << "Pesan berhasil dikirim ke " << to << ".\n";
    }

    void showInbox() {
        cout << "\n===== PESAN MASUK =====\n";
        inbox.showMessages();
    }

    void showSent() {
        cout << "\n===== PESAN TERKIRIM =====\n";
        sentBox.showMessages();
    }

    void showInboxReverse() {
        cout << "\n===== PESAN MASUK (DARI TERBARU) =====\n";
        inbox.showMessagesReverse();
    }

    void deleteMessage() {
        int idx;
        cout << "Hapus pesan nomor berapa di kotak masuk? ";
        cin >> idx;
        cin.ignore();
        inbox.deleteMessage(idx);
    }

    void messageMenu() {
        int pilihan;
        while (true) {
            cout << "\n===== MENU PESAN ANTAR USER =====\n";
            cout << "1. Kirim Pesan\n";
            cout << "2. Lihat Pesan Masuk (lama -> baru)\n";
            cout << "3. Lihat Pesan Terkirim\n";
            cout << "4. Hapus Pesan dari Inbox\n";
            cout << "5. Hapus Semua Pesan di Inbox\n";
            cout << "6. Lihat Pesan Masuk (baru -> lama)\n";
            cout << "0. Kembali ke Menu Tambahan\n";
            cout << "Pilih: ";
            if (!(cin >> pilihan)) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            cin.ignore();

            if (pilihan == 1) sendMessage();
            else if (pilihan == 2) showInbox();
            else if (pilihan == 3) showSent();
            else if (pilihan == 4) deleteMessage();
            else if (pilihan == 5) inbox.clearAll();
            else if (pilihan == 6) showInboxReverse();
            else if (pilihan == 0) break;
            else cout << "Pilihan tidak valid.\n";
        }
    }
};

#endif