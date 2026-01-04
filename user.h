#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include "queue.h"         // QueueLinkedList untuk pending request
#include "stringlist.h"    // pengganti vector
using namespace std;

class User {
private:
    string id;
    string name;

    StringList friends;   // pengganti vector<string>
    StringList sentReq;   // pengganti vector<string>

    QueueLinkedList recvQueue; // pending request (FIFO)

public:
    User();
    User(const string& uid, const string& uname);

    // getters
    string getID() const;
    string getName() const;
    StringList getFriends() const;
    StringList getSentRequests() const;

    // pending requests dalam bentuk list
    StringList getReceivedRequests() const;

    // basic mutators
    bool addFriend(const string& friendId);
    bool removeFriend(const string& friendId);

    bool addSentRequest(const string& toId);
    bool removeSentRequest(const string& toId);

    bool addReceivedRequest(const string& fromId);
    bool removeReceivedRequest(const string& fromId);

    void showSummary() const;
    void showFriends() const;
    void showPendingRequests() const;
};

// Implementasi inline
User::User() : id(""), name("") { }
User::User(const string& uid, const string& uname) : id(uid), name(uname) { }

string User::getID() const { return id; }
string User::getName() const { return name; }
StringList User::getFriends() const { return friends; }
StringList User::getSentRequests() const { return sentReq; }

StringList User::getReceivedRequests() const {
    return recvQueue.toList();
}

bool User::addFriend(const string& friendId) {
    if (friends.contains(friendId)) return false;
    friends.push_back(friendId);
    return true;
}

bool User::removeFriend(const string& friendId) {
    for (int i = 0; i < friends.size(); ++i) {
        if (friends[i] == friendId) {
            friends.eraseAt(i);
            return true;
        }
    }
    return false;
}

bool User::addSentRequest(const string& toId) {
    if (sentReq.contains(toId)) return false;
    sentReq.push_back(toId);
    return true;
}

bool User::removeSentRequest(const string& toId) {
    for (int i = 0; i < sentReq.size(); ++i) {
        if (sentReq[i] == toId) {
            sentReq.eraseAt(i);
            return true;
        }
    }
    return false;
}

bool User::addReceivedRequest(const string& fromId) {
    if (recvQueue.contains(fromId)) return false;
    recvQueue.enqueue(fromId);
    return true;
}

bool User::removeReceivedRequest(const string& fromId) {
    return recvQueue.removeValue(fromId);
}

void User::showSummary() const {
    cout << "- ID: " << id << " | Name: " << name
         << " | Friends: " << friends.size()
         << " | Pending: " << recvQueue.length() << endl;
}

void User::showFriends() const {
    cout << "Friends of " << name << " (" << id << "): ";
    if (friends.empty()) {
        cout << "(no friends)";
    } else {
        for (int i = 0; i < friends.size(); ++i) {
            if (i) cout << ", ";
            cout << friends[i];
        }
    }
    cout << endl;
}

void User::showPendingRequests() const {
    cout << "Pending requests for " << name << " (" << id << "):" << endl;
    if (recvQueue.isEmpty()) {
        cout << "  (none)" << endl;
    } else {
        StringList pending = recvQueue.toList();
        for (int i = 0; i < pending.size(); ++i) {
            cout << "  - " << pending[i] << endl;
        }
    }
}

#endif // USER_H