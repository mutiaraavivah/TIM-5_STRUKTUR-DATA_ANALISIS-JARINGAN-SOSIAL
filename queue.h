#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
using namespace std;

#include "stringlist.h"

//
// ======================
//     QUEUE ARRAY
// ======================
//

const int MAX_Q = 100;

struct QueueArray {
    string data[MAX_Q];
    int front = -1;
    int rear  = -1;

    bool isEmpty() {
        return (front == -1);
    }

    bool isFull() {
        return (rear == MAX_Q - 1);
    }

    void enqueue(const string& msg) {
        if (isFull()) {
            cout << "[QUEUE ARRAY] Antrian notifikasi penuh!\n";
            return;
        }

        if (isEmpty()) {
            front = rear = 0;
        } else {
            rear++;
        }

        data[rear] = msg;
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "[QUEUE ARRAY] Tidak ada notifikasi.\n";
            return;
        }

        cout << "[QUEUE ARRAY] Mengambil notifikasi: " << data[front] << endl;

        if (front == rear) {
            front = rear = -1; // reset queue
        } else {
            front++;
        }
    }

    void show() {
        if (isEmpty()) {
            cout << "[QUEUE ARRAY] Antrian kosong.\n";
            return;
        }

        cout << "\n[QUEUE ARRAY] Daftar Notifikasi:\n";
        for (int i = front; i <= rear; i++) {
            cout << "- " << data[i] << endl;
        }
        cout << endl;
    }
};


//
// ===========================
//     QUEUE LINKED LIST
// ===========================
//

struct Node {
    string msg;
    Node* next;
};

struct QueueLinkedList {
    Node* front = NULL;
    Node* rear  = NULL;

    bool isEmpty() const {
        return front == NULL;
    }

    void enqueue(const string& msg) {
        Node* baru = new Node();
        baru->msg  = msg;
        baru->next = NULL;

        if (isEmpty()) {
            front = rear = baru;
        } else {
            rear->next = baru;
            rear       = baru;
        }
    }

    void dequeue() {
        if (isEmpty()) {
            cout << "[QUEUE LL] Tidak ada notifikasi.\n";
            return;
        }

        cout << "[QUEUE LL] Mengambil notifikasi: " << front->msg << endl;

        Node* hapus = front;
        front       = front->next;

        if (front == NULL) {
            rear = NULL; // queue kosong
        }

        delete hapus;
    }

    void show() const {
        if (isEmpty()) {
            cout << "[QUEUE LL] Antrian kosong.\n";
            return;
        }

        cout << "\n[QUEUE LINKED LIST] Daftar Notifikasi:\n";
        Node* temp = front;

        while (temp != NULL) {
            cout << "- " << temp->msg << endl;
            temp = temp->next;
        }
        cout << endl;
    }

    // ===== Tambahan: buat dipakai sebagai queue permintaan pertemanan =====

    int length() const {
        int cnt = 0;
        Node* temp = front;
        while (temp != NULL) {
            cnt++;
            temp = temp->next;
        }
        return cnt;
    }

    bool contains(const string& value) const {
        Node* temp = front;
        while (temp != NULL) {
            if (temp->msg == value) return true;
            temp = temp->next;
        }
        return false;
    }

    bool removeValue(const string& value) {
        if (isEmpty()) return false;

        Node* cur  = front;
        Node* prev = NULL;

        while (cur != NULL && cur->msg != value) {
            prev = cur;
            cur  = cur->next;
        }

        if (!cur) return false;

        if (cur == front) {
            front = front->next;
            if (front == NULL) rear = NULL;
        } else {
            prev->next = cur->next;
            if (cur == rear) rear = prev;
        }

        delete cur;
        return true;
    }

    // Pengganti toVector(): jadi toList() (tanpa std::vector)
    StringList toList() const {
        StringList v;
        Node* temp = front;
        while (temp != NULL) {
            v.push_back(temp->msg);
            temp = temp->next;
        }
        return v;
    }

    void clear() {
        Node* temp = front;
        while (temp != NULL) {
            Node* del = temp;
            temp = temp->next;
            delete del;
        }
        front = rear = NULL;
    }
};

#endif