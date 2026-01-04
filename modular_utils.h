#ifndef MODULAR_UTILS_H
#define MODULAR_UTILS_H

#include "fileio.h"
#include "analysis.h"
#include "searchsort.h"
#include "activity.h"
#include "circlefriends.h"
#include "dailycycle.h"
#include "message.h"
#include "notes.h"
#include "stack_notif.h"
#include "queue_notif_menu.h"
#include <iostream>
using namespace std;

void showExtendedMenu(Relation& net, ActivityLog& log) {
    int pilih = -1; // supaya langsung masuk ke loop

    while (true) {
        cout << "\n===== FITUR TAMBAHAN =====\n";
        cout << "1. Simpan Data ke File\n";
        cout << "2. Muat Data dari File\n";
        cout << "3. Statistik Jaringan\n";
        cout << "4. Cari User Berdasarkan Nama\n";
        cout << "5. Urutkan User Berdasarkan Jumlah Teman\n";
        cout << "6. Tampilkan Log Aktivitas\n";
        cout << "7. Kelola Lingkaran Pertemanan\n";
        cout << "8. Siklus Aktivitas Harian\n";
        cout << "9. Pesan Antar User\n";
        cout << "10. Catatan Jaringan\n";
        cout << "11. Notifikasi(STACK)\n";
        cout << "12. Notifikasi(QUEUE)\n";
        cout << "0. Kembali ke Menu Utama\n";
        
        cout << "\nPilih Menu Tambahan: ";
        if (!(cin >> pilih)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore(); // buang newline

        switch (pilih) {
            case 1:
                saveNetworkData(net, "data.txt");
                log.add("Data disimpan ke file.");
                break;

            case 2:
                loadNetworkData(net, "data.txt");
                log.add("Data dimuat dari file.");
                break;

            case 3:
                showNetworkStatistics(net);
                log.add("Menampilkan statistik jaringan.");
                break;

            case 4: {
                string key;
                cout << "Masukkan Nama: ";
                getline(cin, key);
                searchUserByName(net, key);
                log.add("Pencarian user dilakukan: " + key);
                break;
            }

            case 5:
                sortUserByFriends(net);
                log.add("Mengurutkan user berdasarkan jumlah teman.");
                break;

            case 6:
                log.show();
                break;
                
            case 7:
                circleFriendsMenu();
                break;
            	
            case 8:
                dailyCycleMenu();
                break;
            	
            case 9: {
                MessagingSystem msg(&net);
                msg.messageMenu();
                log.add("Mengakses fitur pesan antar user.");
                break;
            }
			
            case 10:
                notesMenu();                 // singly
                log.add("Mengakses catatan jaringan.");
                break;
                
            case 11:
                notificationStackMenu();     // <-- STACK
                log.add("Mengakses stack notifikasi.");
                break;
			    
            case 12:
                notificationQueueMenu();     // <-- QUEUE
                log.add("Mengakses queue notifikasi.");
                break;

            case 0:
                cout << "Kembali ke menu utama...\n";
                return; // keluar dari fungsi tanpa error

            default:
                cout << "Pilihan tidak valid.\n";
        }

        cout << "\n(Press ENTER untuk kembali ke menu fitur tambahan)";
        cin.get();
    }
}

#endif // MODULAR_UTILS_H