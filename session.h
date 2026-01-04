#ifndef SESSION_H
#define SESSION_H

#include <iostream>
#include <string>
#include "queue.h"

class SessionManager {
private:
    QueueLinkedList loginQueue;
    QueueLinkedList logoutQueue;

public:
    void onLogin(const std::string& userId) { loginQueue.enqueue(userId); }
    void onLogout(const std::string& userId) { logoutQueue.enqueue(userId); }

    void showLoginQueue() const {
        std::cout << "\n===== ANTRIAN MASUK (LOGIN) =====\n";
        loginQueue.show();
    }
    void showLogoutQueue() const {
        std::cout << "\n===== ANTRIAN KELUAR (LOGOUT) =====\n";
        logoutQueue.show();
    }

    int loginCount() const { return loginQueue.length(); }
    int logoutCount() const { return logoutQueue.length(); }
};

#endif