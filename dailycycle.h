#ifndef DAILYCYCLE_H
#define DAILYCYCLE_H

#include <iostream>
#include <string>
using namespace std;

struct ActivityNode {
    string activity;
    ActivityNode* next;
};

struct DailyCycle {
    ActivityNode* tail;

    DailyCycle() { tail = nullptr; }

    bool isEmpty() { return tail == nullptr; }

    void addActivity(const string& act) {
        ActivityNode* newNode = new ActivityNode;
        newNode->activity = act;
        if (isEmpty()) {
            tail = newNode;
            newNode->next = tail;
        } else {
            newNode->next = tail->next;
            tail->next = newNode;
            tail = newNode;
        }
        cout << "Aktivitas \"" << act << "\" ditambahkan ke siklus harian.\n";
    }

    void showCycle() {
        if (isEmpty()) {
            cout << "(Siklus harian kosong)\n";
            return;
        }
        ActivityNode* temp = tail->next;
        cout << "\n=== Siklus Aktivitas Harian ===\n";
        do {
            cout << "- " << temp->activity << endl;
            temp = temp->next;
        } while (temp != tail->next);
    }

    void simulateCycle(int times) {
        if (isEmpty()) {
            cout << "(Belum ada aktivitas)\n";
            return;
        }
        ActivityNode* temp = tail->next;
        cout << "\nSimulasi " << times << " putaran aktivitas:\n";
        for (int i = 0; i < times; ++i) {
            cout << "[" << i + 1 << "] " << temp->activity << endl;
            temp = temp->next;
        }
    }

    void clearCycle() {
        if (isEmpty()) return;
        ActivityNode* temp = tail->next;
        while (temp != tail) {
            ActivityNode* del = temp;
            temp = temp->next;
            delete del;
        }
        delete tail;
        tail = nullptr;
    }
};

void dailyCycleMenu() {
    DailyCycle dc;
    int choice;
    string act;

    while (true) {
        cout << "\n===== MENU SIKLUS HARIAN =====\n";
        cout << "1. Tambah Aktivitas\n";
        cout << "2. Tampilkan Siklus Aktivitas\n";
        cout << "3. Simulasikan Siklus\n";
        cout << "4. Hapus Semua Aktivitas\n";
        cout << "0. Kembali ke Menu Tambahan\n";
        cout << "Pilih: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            cout << "Masukkan aktivitas: ";
            getline(cin, act);
            dc.addActivity(act);
        } 
        else if (choice == 2) {
            dc.showCycle();
        } 
        else if (choice == 3) {
            int n;
            cout << "Simulasi berapa kali putaran? ";
            cin >> n; cin.ignore();
            dc.simulateCycle(n);
        } 
        else if (choice == 4) {
            dc.clearCycle();
            cout << "Semua aktivitas dihapus.\n";
        } 
        else if (choice == 0) {
            cout << "Kembali ke menu tambahan...\n";
            break;
        } 
        else cout << "Pilihan tidak valid.\n";
    }
}

#endif