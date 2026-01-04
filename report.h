#ifndef REPORT_H
#define REPORT_H

#include <fstream>
#include <map>
#include <string>

inline bool generateActivityReport(const std::string& logFile, const std::string& outFile) {
    std::ifstream in(logFile.c_str());
    if (!in.is_open()) return false;

    long long total = 0;
    std::map<std::string, long long> byDate;
    std::map<std::string, long long> byEvent;

    std::string line;
    while (std::getline(in, line)) {
        if (line.size() < 10) continue;
        std::string date = line.substr(0, 10);

        std::size_t p1 = line.find('|');
        if (p1 == std::string::npos) continue;
        std::size_t p2 = line.find('|', p1 + 1);
        if (p2 == std::string::npos) continue;

        std::string event = line.substr(p1 + 1, p2 - (p1 + 1));

        total++;
        byDate[date]++;
        byEvent[event]++;
    }

    std::string topEvent;
    long long topCount = -1;
    for (auto& kv : byEvent) {
        if (kv.second > topCount) {
            topCount = kv.second;
            topEvent = kv.first;
        }
    }

    std::ofstream out(outFile.c_str(), std::ios::trunc);
    if (!out.is_open()) return false;

    out << "===== REPORT AKTIVITAS (JARINGAN SOSIAL) =====\n";
    out << "Sumber log : " << logFile << "\n";
    out << "Total event: " << total << "\n\n";

    out << "Event per tanggal:\n";
    if (byDate.empty()) out << "(Tidak ada data)\n";
    for (auto& kv : byDate) out << "- " << kv.first << " : " << kv.second << "\n";

    out << "\nEvent paling sering: " << (topEvent.empty() ? "(none)" : topEvent)
        << " (" << (topCount < 0 ? 0 : topCount) << ")\n";

    out << "\nDistribusi event:\n";
    for (auto& kv : byEvent) out << "- " << kv.first << " : " << kv.second << "\n";

    return true;
}

#endif