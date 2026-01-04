#ifndef RELATION_H
#define RELATION_H

#include <string>
#include <iostream>
#include "user.h"
#include "stringlist.h"
using namespace std;

#define MAX_USERS 50

class Relation {
private:
    User users[MAX_USERS];
    int totalUsers;

    int findIndexById(const string& id) const;

public:
    Relation();
    int size() const;
    int getTotalUsers() const;
    User* getUserByIndex(int idx);

    bool addUser(const string& id, const string& name);

    // Tambahan untuk instruksi tugas: Undo / pembatalan via STACK
    bool cancelFriendRequest(const string& fromId, const string& toId);
    bool undoAcceptFriendRequest(const string& toId, const string& fromId);

    bool userExists(const string& id) const;

    bool sendFriendRequest(const string& fromId, const string& toId);
    bool acceptFriendRequest(const string& toId, const string& fromId);
    bool rejectFriendRequest(const string& toId, const string& fromId);

    bool makeFriends(const string& id1, const string& id2);

    void showAllUsers() const;
    void showUserDetails(const string& id) const;
    User* getUser(const string& id);

    void showFriendsOf(const string& id);

    void swapUsers(int i, int j) {
        if (i>=0 && j>=0 && i<totalUsers && j<totalUsers) {
            User temp = users[i];
            users[i] = users[j];
            users[j] = temp;
        }
    }
};

// Implementasi

Relation::Relation() : totalUsers(0) { }

int Relation::size() const { return totalUsers; }

int Relation::findIndexById(const string& id) const {
    for (int i = 0; i < totalUsers; ++i) {
        if (users[i].getID() == id) return i;
    }
    return -1;
}

bool Relation::addUser(const string& id, const string& name) {
    if (totalUsers >= MAX_USERS) {
        cout << "User limit reached (" << MAX_USERS << "). Cannot add more.\n";
        return false;
    }
    if (findIndexById(id) != -1) {
        cout << "ID already used. Choose another ID.\n";
        return false;
    }
    users[totalUsers] = User(id, name);
    totalUsers++;
    cout << "User berhasil ditambahkan: " << name << " (ID: " << id << ")\n";
    return true;
}

bool Relation::userExists(const string& id) const {
    return (findIndexById(id) != -1);
}

bool Relation::sendFriendRequest(const string& fromId, const string& toId) {
    if (fromId == toId) {
        cout << "Tidak bisa mengirim permintaan ke diri sendiri.\n";
        return false;
    }
    int iFrom = findIndexById(fromId);
    int iTo   = findIndexById(toId);
    if (iFrom == -1 || iTo == -1) {
        cout << "Salah satu ID tidak ditemukan.\n";
        return false;
    }
    User* from = &users[iFrom];
    User* to   = &users[iTo];

    // jika sudah teman
    StringList fromFriends = from->getFriends();
    for (int k = 0; k < fromFriends.size(); ++k) {
        if (fromFriends[k] == toId) {
            cout << "Sudah berteman.\n";
            return false;
        }
    }

    // jika sudah mengirim request sebelumnya
    StringList sent = from->getSentRequests();
    for (int k = 0; k < sent.size(); ++k) {
        if (sent[k] == toId) {
            cout << "Permintaan sudah dikirim sebelumnya.\n";
            return false;
        }
    }

    from->addSentRequest(toId);
    to->addReceivedRequest(fromId);
    cout << "Permintaan pertemanan dikirim dari " << fromId << " ke " << toId << ".\n";
    return true;
}

// ======= Tambahan (Undo) =======
// Membatalkan request yang masih pending: hapus di sentReq pengirim dan recvQueue penerima
bool Relation::cancelFriendRequest(const string& fromId, const string& toId) {
    User* from = getUser(fromId);
    User* to   = getUser(toId);
    if (!from || !to) {
        cout << "User tidak ditemukan.\n";
        return false;
    }

    StringList pending = to->getReceivedRequests();
    if (!pending.contains(fromId)) {
        cout << "Tidak ada pending request yang bisa dibatalkan.\n";
        return false;
    }

    to->removeReceivedRequest(fromId);
    from->removeSentRequest(toId);

    cout << "Permintaan pertemanan dibatalkan: " << fromId << " -> " << toId << "\n";
    return true;
}

// Undo accept: hapus pertemanan lalu kembalikan request ke status pending (restore)
bool Relation::undoAcceptFriendRequest(const string& toId, const string& fromId) {
    User* to   = getUser(toId);
    User* from = getUser(fromId);
    if (!to || !from) {
        cout << "User tidak ditemukan.\n";
        return false;
    }

    StringList f = to->getFriends();
    if (!f.contains(fromId)) {
        cout << "Undo gagal: kedua user belum berteman.\n";
        return false;
    }

    to->removeFriend(fromId);
    from->removeFriend(toId);

    // restore pending request
    to->addReceivedRequest(fromId);
    from->addSentRequest(toId);

    cout << "Undo accept berhasil: relasi teman dihapus dan pending request dipulihkan.\n";
    return true;
}
// ======= End Tambahan =======

bool Relation::acceptFriendRequest(const string& toId, const string& fromId) {
    int iTo   = findIndexById(toId);
    int iFrom = findIndexById(fromId);
    if (iTo == -1 || iFrom == -1) {
        cout << "Salah satu ID tidak ditemukan.\n";
        return false;
    }
    User* to   = &users[iTo];
    User* from = &users[iFrom];

    StringList recv = to->getReceivedRequests();
    bool found = false;
    for (int k = 0; k < recv.size(); ++k) {
        if (recv[k] == fromId) { found = true; break; }
    }
    if (!found) {
        cout << "Tidak ada permintaan dari " << fromId << " kepada " << toId << ".\n";
        return false;
    }

    to->removeReceivedRequest(fromId);
    from->removeSentRequest(toId);

    to->addFriend(fromId);
    from->addFriend(toId);

    cout << toId << " menerima permintaan dari " << fromId << ". Sekarang mereka berteman.\n";
    return true;
}

bool Relation::rejectFriendRequest(const string& toId, const string& fromId) {
    int iTo   = findIndexById(toId);
    int iFrom = findIndexById(fromId);
    if (iTo == -1 || iFrom == -1) {
        cout << "Salah satu ID tidak ditemukan.\n";
        return false;
    }
    User* to   = &users[iTo];
    User* from = &users[iFrom];

    StringList recv = to->getReceivedRequests();
    bool found = false;
    for (int k = 0; k < recv.size(); ++k) {
        if (recv[k] == fromId) { found = true; break; }
    }
    if (!found) {
        cout << "Tidak ada permintaan dari " << fromId << " kepada " << toId << ".\n";
        return false;
    }

    to->removeReceivedRequest(fromId);
    from->removeSentRequest(toId);
    cout << toId << " menolak permintaan dari " << fromId << ".\n";
    return true;
}

bool Relation::makeFriends(const string& id1, const string& id2) {
    int i1 = findIndexById(id1);
    int i2 = findIndexById(id2);
    if (i1 == -1 || i2 == -1 || id1 == id2) return false;
    User* u1 = &users[i1];
    User* u2 = &users[i2];
    bool changed = false;
    if (u1->addFriend(id2)) changed = true;
    if (u2->addFriend(id1)) changed = true;
    return changed;
}

void Relation::showAllUsers() const {
    cout << "\n=== Daftar Pengguna ===\n";
    if (totalUsers == 0) {
        cout << "(Belum ada user)\n";
        return;
    }
    for (int i = 0; i < totalUsers; ++i) {
        users[i].showSummary();
    }
}

void Relation::showUserDetails(const string& id) const {
    int idx = findIndexById(id);
    if (idx == -1) {
        cout << "User tidak ditemukan.\n";
        return;
    }
    const User& u = users[idx];
    cout << "Detail user:\n";
    cout << "ID: " << u.getID() << "\n";
    cout << "Name: " << u.getName() << "\n";
    u.showFriends();
    u.showPendingRequests();
}

User* Relation::getUser(const string& id) {
    int idx = findIndexById(id);
    if (idx == -1) return NULL;
    return &users[idx];
}

int Relation::getTotalUsers() const {
    return totalUsers;
}

User* Relation::getUserByIndex(int idx) {
    if (idx<0 || idx>=totalUsers) return NULL;
    return &users[idx];
}

void Relation::showFriendsOf(const string& id) {
    int idx = findIndexById(id);
    if (idx == -1) {
        cout << "User tidak ditemukan.\n";
        return;
    }
    users[idx].showFriends();
}

#endif