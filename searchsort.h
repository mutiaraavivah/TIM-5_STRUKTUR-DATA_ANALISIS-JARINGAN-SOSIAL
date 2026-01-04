#ifndef SEARCHSORT_H
#define SEARCHSORT_H

#include "relation.h"

void searchUserByName(Relation& net, const string& keyword) {
    cout << "\n===== HASIL PENCARIAN =====\n";
    bool found = false;
    for (int i=0; i<net.getTotalUsers(); i++) {
        User* u = net.getUserByIndex(i);
        if (u->getName().find(keyword) != string::npos) {
            u->showSummary();
            found = true;
        }
    }
    if (!found) cout << "Tidak ada user dengan nama mengandung \"" << keyword << "\"\n";
}

void sortUserByFriends(Relation& net) {
    int total = net.getTotalUsers();
    for (int i=0; i<total-1; i++) {
        for (int j=0; j<total-1; ++j) {
            if (net.getUserByIndex(j)->getFriends().size() < net.getUserByIndex(j+1)->getFriends().size()) {
                net.swapUsers(j, j+1);
            }
        }
    }
    cout << "User diurutkan berdasarkan jumlah teman (terbanyak -> terkecil)\n";
}

#endif