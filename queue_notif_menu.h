#ifndef QUEUE_NOTIF_MENU_H
#define QUEUE_NOTIF_MENU_H

#include <iostream>
#include "queue.h"
using namespace std;

void notificationQueueMenu() {

    QueueArray qa;
    QueueLinkedList ql;

    int pilihNotif = -1;
    string msg;

    while (pilihNotif != 0) {

        cout << "\n=== MENU NOTIFIKASI (QUEUE) ===\n";
        cout << "1. Tambah Notifikasi (Enqueue)\n";
        cout << "2. Ambil Notifikasi (Dequeue)\n";
        cout << "3. Tampilkan Antrian (ARRAY)\n";
        cout << "4. Tampilkan Antrian (LINKED LIST)\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        cin >> pilihNotif;
        cin.ignore();

        switch (pilihNotif) {

            case 1:
                cout << "Masukkan pesan notifikasi: ";
                getline(cin, msg);
                qa.enqueue(msg);
                ql.enqueue(msg);
                break;

            case 2:
                cout << "\n--- ARRAY ---\n";
                qa.dequeue();

                cout << "\n--- LINKED LIST ---\n";
                ql.dequeue();
                break;

            case 3:
                qa.show();
                break;

            case 4:
                ql.show();
                break;
        }
    }
}

#endif
