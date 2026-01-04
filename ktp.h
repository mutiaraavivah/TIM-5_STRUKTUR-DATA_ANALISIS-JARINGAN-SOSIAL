#ifndef KTP_H
#define KTP_H

#include <fstream>
#include <iostream>
#include <string>

struct KTPRecord {
    std::string nik;
    std::string nama;
    std::string tglLahir;
};

class KTPDatabase {
private:
    static const int MAX_KTP = 500;
    KTPRecord data[MAX_KTP];
    int n;

public:
    KTPDatabase() : n(0) {}

    int size() const { return n; }

    bool loadFromFile(const std::string& filename) {
        std::ifstream in(filename.c_str());
        if (!in.is_open()) return false;
        n = 0;
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            std::size_t p1 = line.find(';');
            if (p1 == std::string::npos) continue;
            std::size_t p2 = line.find(';', p1 + 1);
            if (p2 == std::string::npos) continue;
            if (n >= MAX_KTP) break;
            data[n].nik = line.substr(0, p1);
            data[n].nama = line.substr(p1 + 1, p2 - (p1 + 1));
            data[n].tglLahir = line.substr(p2 + 1);
            ++n;
        }
        return true;
    }

    bool saveToFile(const std::string& filename) const {
        std::ofstream out(filename.c_str(), std::ios::trunc);
        if (!out.is_open()) return false;
        for (int i = 0; i < n; ++i) {
            out << data[i].nik << ";" << data[i].nama << ";" << data[i].tglLahir << "\n";
        }
        return true;
    }

    bool addRecord(const std::string& nik, const std::string& nama, const std::string& tglLahir) {
        if (n >= MAX_KTP) return false;
        if (findByNik(nik) != nullptr) return false;
        data[n++] = KTPRecord{nik, nama, tglLahir};
        return true;
    }

    const KTPRecord* findByNik(const std::string& nik) const {
        for (int i = 0; i < n; ++i) {
            if (data[i].nik == nik) return &data[i];
        }
        return nullptr;
    }

    void showAll() const {
        std::cout << "\n===== DATABASE KTP =====\n";
        if (n == 0) { std::cout << "(Kosong)\n"; return; }
        for (int i = 0; i < n; ++i) {
            std::cout << i + 1 << ". " << data[i].nik << " | " << data[i].nama
                      << " | " << data[i].tglLahir << "\n";
        }
    }
};

#endif