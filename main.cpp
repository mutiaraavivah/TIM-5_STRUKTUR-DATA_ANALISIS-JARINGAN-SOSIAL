#include <iostream>
#include <string>
#include <fstream>
#include "relation.h"
#include "recursive.h"
#include "modular_utils.h"
#include "message.h"
#include "queue.h"
#include "stringlist.h"

#include "ktp.h"
#include "logger.h"
#include "session.h"
#include "undo.h"
#include "crypto_utils.h"
#include "avl_tree.h"
#include "huffman_utils.h"
#include "report.h"

using namespace std;

void showMenu() {
    cout << "======================================\n";
    cout << "    SISTEM ANALISIS JARINGAN SOSIAL  \n";
    cout << "======================================\n\n";
    cout << "Menu:\n";
    cout << "1. Admin: Kelola Database KTP\n";
    cout << "2. Registrasi User (Wajib Verifikasi NIK)\n";
    cout << "3. Login (Masuk Antrian)\n";
    cout << "4. Logout (Masuk Antrian Keluar)\n";
    cout << "5. Kirim Permintaan Pertemanan\n";
    cout << "6. Lihat Pending Requests (User)\n";
    cout << "7. Konfirmasi Permintaan (Terima/Tolak)\n";
    cout << "8. Batalkan Aksi Terakhir (STACK Undo)\n";
    cout << "9. Tampilkan Semua Pengguna\n";
    cout << "10. Tampilkan Daftar Teman (User)\n";
    cout << "11. Lihat Koneksi (User)\n";
    cout << "12. Tampilkan Antrian Masuk/Keluar\n";
    cout << "13. Generate Laporan (Reporting)\n";
    cout << "14. Menu Fitur Tambahan\n";
    cout << "15. Keluar\n\n";
    cout << "Pilih menu: ";
}

static void ensureKTPSeed(KTPDatabase& ktp, const string& filename) {
    bool ok = ktp.loadFromFile(filename);
    if (!ok || ktp.size() < 2) {
        ktp.addRecord("3175092901010001", "AHMAD PRATAMA", "2001-01-29");
        ktp.addRecord("3175092901010002", "SITI AMALIA", "2001-01-29");
        ktp.saveToFile(filename);
    }
}

static string readAllText(const string& filename) {
    ifstream in(filename.c_str());
    if (!in.is_open()) return "";
    string out, line;
    while (getline(in, line)) {
        out += line;
        out += '\n';
    }
    return out;
}

static void adminKTPMenu(KTPDatabase& ktp, Logger& logger) {
    while (true) {
        cout << "\n===== ADMIN KTP =====\n";
        cout << "1. Tampilkan Database KTP\n";
        cout << "2. Tambah Data KTP\n";
        cout << "3. Simpan ke File\n";
        cout << "0. Kembali\n";
        cout << "Pilih: ";
        int c;
        if (!(cin >> c)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (c == 1) {
            ktp.showAll();
        } else if (c == 2) {
            string nik, nama, tgl;
            cout << "NIK: "; getline(cin, nik);
            cout << "Nama: "; getline(cin, nama);
            cout << "Tgl lahir (YYYY-MM-DD): "; getline(cin, tgl);

            if (ktp.addRecord(nik, nama, tgl)) {
                cout << "Data KTP ditambahkan.\n";
                logger.log("KTP_ADD", nik + ";" + nama);
            } else {
                cout << "Gagal menambah (mungkin NIK duplikat / kapasitas).\n";
            }
        } else if (c == 3) {
            if (ktp.saveToFile("ktp_data.txt")) {
                cout << "Berhasil disimpan ke ktp_data.txt\n";
                logger.log("KTP_SAVE", "ktp_data.txt");
            } else {
                cout << "Gagal menyimpan file.\n";
            }
        } else if (c == 0) {
            return;
        }

        cout << "\n(Press ENTER to continue)";
        cin.get();
    }
}

int main() {
    Relation net;
    ActivityLog log;                    // existing extended log
    Logger audit("activity_log.txt");   // wajib: semua aktivitas tersimpan file
    KTPDatabase ktp;                    // wajib: verifikasi KTP
    SessionManager sessions;            // wajib: 2 queue masuk/keluar
    ActionUndoStack undo;               // wajib: stack pembatalan
    AVLTree ktpIndexAVL;                // bonus: AVL untuk NIK terenkripsi

    ensureKTPSeed(ktp, "ktp_data.txt");

    int choice = 0;

    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            cout << "Input tidak valid.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            adminKTPMenu(ktp, audit);
        }
        else if (choice == 2) {
            string nik, id, name;
            cout << "Masukkan NIK (harus terdaftar di database KTP): ";
            getline(cin, nik);

            const KTPRecord* rec = ktp.findByNik(nik);
            if (!rec) {
                cout << "Registrasi ditolak: NIK tidak ditemukan di database KTP.\n";
                audit.log("REGISTER_FAIL", "NIK_NOT_FOUND;" + nik);
            } else {
                cout << "NIK valid untuk: " << rec->nama << "\n";
                cout << "Masukkan ID Pengguna (unik, mis: lala01): ";
                getline(cin, id);
                cout << "Masukkan Nama Pengguna (boleh sama dengan KTP): ";
                getline(cin, name);

                if (net.addUser(id, name)) {
                    string encNik = xorEncrypt(nik);
                    string note = ktpIndexAVL.insert(encNik);
                    if (!note.empty()) {
                        cout << "[AVL] " << note << "\n";
                        audit.log("AVL_ROTATION", note);
                    }
                    audit.log("REGISTER_OK", id + ";" + name + ";NIK_ENC=" + encNik);
                } else {
                    audit.log("REGISTER_FAIL", "ADD_USER_FAILED;" + id);
                }
            }
        }
        else if (choice == 3) {
            string id;
            cout << "Masukkan ID user untuk login: ";
            getline(cin, id);

            if (!net.userExists(id)) {
                cout << "Login gagal: user tidak ditemukan.\n";
                audit.log("LOGIN_FAIL", id);
            } else {
                sessions.onLogin(id);
                cout << "Login berhasil, masuk antrian login.\n";
                audit.log("LOGIN", id);
            }
        }
        else if (choice == 4) {
            string id;
            cout << "Masukkan ID user untuk logout: ";
            getline(cin, id);

            if (!net.userExists(id)) {
                cout << "Logout gagal: user tidak ditemukan.\n";
                audit.log("LOGOUT_FAIL", id);
            } else {
                sessions.onLogout(id);
                cout << "Logout tercatat, masuk antrian logout.\n";
                audit.log("LOGOUT", id);
            }
        }
        else if (choice == 5) {
            string fromId, toId;
            cout << "Masukkan ID pengirim: ";
            getline(cin, fromId);
            cout << "Masukkan ID penerima: ";
            getline(cin, toId);

            if (net.sendFriendRequest(fromId, toId)) {
                undo.push(ACT_SEND_REQUEST, fromId, toId);
                audit.log("FRIEND_REQUEST", fromId + "->" + toId);
            } else {
                audit.log("FRIEND_REQUEST_FAIL", fromId + "->" + toId);
            }
        }
        else if (choice == 6) {
            string id;
            cout << "Masukkan ID user: ";
            getline(cin, id);
            User* u = net.getUser(id);
            if (!u) cout << "User tidak ditemukan.\n";
            else u->showPendingRequests();
        }
        else if (choice == 7) {
            string toId, fromId;
            cout << "Masukkan ID penerima: ";
            getline(cin, toId);
            cout << "Masukkan ID pengirim (yang dikonfirmasi): ";
            getline(cin, fromId);
            cout << "Pilih: 1 = Terima, 2 = Tolak : ";
            int opt; cin >> opt; cin.ignore();

            if (opt == 1) {
                if (net.acceptFriendRequest(toId, fromId)) {
                    undo.push(ACT_ACCEPT_REQUEST, fromId, toId);
                    audit.log("FRIEND_ACCEPT", fromId + "->" + toId);
                } else {
                    audit.log("FRIEND_ACCEPT_FAIL", fromId + "->" + toId);
                }
            } else {
                if (net.rejectFriendRequest(toId, fromId)) {
                    audit.log("FRIEND_REJECT", fromId + "->" + toId);
                } else {
                    audit.log("FRIEND_REJECT_FAIL", fromId + "->" + toId);
                }
            }
        }
        else if (choice == 8) {
            ActionRecord last{};
            if (!undo.pop(last)) {
                cout << "Tidak ada aksi yang bisa dibatalkan.\n";
            } else {
                if (last.type == ACT_SEND_REQUEST) {
                    bool ok = net.cancelFriendRequest(last.a, last.b);
                    audit.log("UNDO_SEND_REQUEST", last.a + "->" + last.b + (ok ? ";OK" : ";FAIL"));
                } else if (last.type == ACT_ACCEPT_REQUEST) {
                    // last.a = fromId, last.b = toId
                    bool ok = net.undoAcceptFriendRequest(last.b, last.a);
                    audit.log("UNDO_ACCEPT", last.a + "->" + last.b + (ok ? ";OK" : ";FAIL"));
                }
            }
        }
        else if (choice == 9) {
            net.showAllUsers();
        }
        else if (choice == 10) {
            string id;
            cout << "Masukkan ID user: ";
            getline(cin, id);
            net.showFriendsOf(id);
        }
        else if (choice == 11) {
            string id;
            cout << "Masukkan ID user: ";
            getline(cin, id);
            cout << "Struktur koneksi:\n";
            showConnectionsRecursive(net, id);
        }
        else if (choice == 12) {
            sessions.showLoginQueue();
            sessions.showLogoutQueue();
        }
        else if (choice == 13) {
            const string logFile = audit.getFile();
            const string reportFile = "report.txt";

            if (generateActivityReport(logFile, reportFile)) {
                cout << "Laporan berhasil dibuat: " << reportFile << "\n";
            } else {
                cout << "Gagal membuat laporan. Pastikan file log ada.\n";
            }

            string text = readAllText(logFile);
            auto hf = Huffman::analyze(text);
            cout << "\n[Huffman] Original bits: " << hf.originalBits
                 << " | Encoded bits: " << hf.encodedBits
                 << " | Ratio: " << hf.compressionRatio << "\n";

            audit.log("REPORT", "Generated report.txt; HuffmanRatio=" + to_string(hf.compressionRatio));
        }
        else if (choice == 14) {
            showExtendedMenu(net, log); // menu tambahan lama tetap dipakai
        }
        else if (choice == 15) {
            cout << "Terima Kasih\nSampai Jumpa Lagi :D\n";
            break;
        }
        else {
            cout << "Pilihan tidak tersedia.\n";
        }

        cout << "\n(Press ENTER to continue)";
        cin.get();
        cout << "\n\n";
    }

    return 0;
}