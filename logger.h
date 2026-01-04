#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>

class Logger {
private:
    std::string filename;

    static std::string nowTimestamp() {
        std::time_t t = std::time(nullptr);
        std::tm tm{};
#if defined(_WIN32)
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif
        char buf[20];
        std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
                      tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                      tm.tm_hour, tm.tm_min, tm.tm_sec);
        return std::string(buf);
    }

public:
    explicit Logger(const std::string& file = "activity_log.txt") : filename(file) {}

    void setFile(const std::string& file) { filename = file; }
    std::string getFile() const { return filename; }

    void log(const std::string& event, const std::string& details) const {
        std::ofstream out(filename.c_str(), std::ios::app);
        if (!out.is_open()) return;
        out << nowTimestamp() << "|" << event << "|" << details << "\n";
    }
};

#endif