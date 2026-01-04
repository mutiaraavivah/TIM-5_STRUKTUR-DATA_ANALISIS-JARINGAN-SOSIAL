#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <iostream>
#include "relation.h"
using namespace std;

void showNetworkStatistics(Relation& net) {
    int totalUsers = net.getTotalUsers();
    if (totalUsers == 0) {
        cout << "Belum ada pengguna.\n";
        return;
    }

    int totalFriends = 0;
    int maxFriends = -1;
    string maxUser;

    for (int i=0; i<totalUsers; i++) {
        User* u = net.getUserByIndex(i);
        int friendsCount = u->getFriends().size();
        totalFriends += friendsCount;

        if (friendsCount > maxFriends) {
            maxFriends = friendsCount;
            maxUser = u->getName();
        }
    }

    double avgFriends = (double)totalFriends / totalUsers;

    cout << "\n===== STATISTIK JARINGAN =====\n";
    cout << "Total User: " << totalUsers << endl;
    cout << "Total Koneksi: " << totalFriends << endl;
    cout << "Rata-rata teman per user: " << avgFriends << endl;
    cout << "User dengan teman terbanyak: " << maxUser << " (" << maxFriends << " teman\n";
}

#endif