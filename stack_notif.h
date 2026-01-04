#ifndef STACK_NOTIF_H
#define STACK_NOTIF_H

#include <iostream>
#include <string>
using namespace std;

const int MAX_STACK = 100;

struct NotificationStack {
    string data[MAX_STACK];
    int top;

    NotificationStack() {
        top = -1;
    }

    bool isEmpty() {
        return top == -1;
    }

    bool isFull() {
        return top == MAX_STACK - 1;
    }

    void push(const string& notif) {
        if (isFull()) {
            cout << "Stack notifikasi penuh, tidak bisa menambah.\n";
            return;
        }
        data[++top] = notif;
        cout << "Notifikasi ditambahkan ke stack.\n";
    }

    void pop() {
        if (isEmpty()) {
            cout << "Stack kosong, tidak ada notifikasi untuk di-pop.\n";
            return;
        }
        cout << "Mengambil notifikasi paling baru: \"" << data[top] << "\"\n";
        top--;
    }

    string peek() {
        if (isEmpty()) {
            cout << "Stack kosong.\n";
            return "";
        }
        return data[top];
    }

    void showAll() {
        if (isEmpty()) {
            cout << "(Belum ada notifikasi di stack)\n";
            return;
        }
        cout << "\n=== ISI STACK NOTIFIKASI (atas -> bawah) ===\n";
        for (int i = top; i >= 0; --i) {
            cout << (top - i + 1) << ". " << data[i] << endl;
        }
    }

    void clearAll() {
        top = -1;
        cout << "Semua notifikasi di stack dihapus.\n";
    }
};

void notificationStackMenu() {
    NotificationStack st;
    int pilih;
    string teks;

    while (true) {
        cout << "\n===== MENU STACK NOTIFIKASI =====\n";
        cout << "1. Tambah notifikasi (push)\n";
        cout << "2. Ambil notifikasi terbaru (pop)\n";
        cout << "3. Lihat notifikasi teratas (peek)\n";
        cout << "4. Tampilkan semua notifikasi\n";
        cout << "5. Hapus semua notifikasi\n";
        cout << "0. Kembali ke Menu Fitur Tambahan\n";
        cout << "Pilih: ";

        if (!(cin >> pilih)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(); // buang newline

        if (pilih == 1) {
            cout << "Tulis isi notifikasi: ";
            getline(cin, teks);
            st.push(teks);
        }
        else if (pilih == 2) {
            st.pop();
        }
        else if (pilih == 3) {
            {
                string topNotif = st.peek();
                if (!topNotif.empty())
                    cout << "Notifikasi paling atas: \"" << topNotif << "\"\n";
            }
        }
        else if (pilih == 4) {
            st.showAll();
        }
        else if (pilih == 5) {
            st.clearAll();
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