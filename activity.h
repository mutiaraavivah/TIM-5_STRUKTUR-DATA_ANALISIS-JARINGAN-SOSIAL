#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <string>
#include <iostream>
#include "stringlist.h"
using namespace std;

class ActivityLog {
private:
    StringList logs;
public:
    void add(const string& msg) {
        logs.push_back(msg);
    }

    void show() const {
        cout << "\n=====RIWAYAT AKTIVITAS =====\n";
        if (logs.empty()) cout << "(Belum ada aktivitas)\n";
        else {
            for (int i = 0; i < logs.size(); ++i)
                cout << i + 1 << ". " << logs[i] << endl;
        }
    }
};

#endif