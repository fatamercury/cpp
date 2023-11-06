#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

struct LogEntry {
    std::string level;
    std::string dateTime;
    std::string source;
    std::string eventID;
    std::string description;
};

int main() {
    std::ifstream logFile("log.txt");
    if (!logFile) {
        std::cerr << "Gagal membuka file log.txt" << std::endl;
        return 1;
    }

    std::map<std::string, std::ofstream> levelToFile;

    std::string line;
    while (std::getline(logFile, line)) {
        // Pisahkan baris menjadi bagian-bagian yang sesuai
        size_t pos = 0;
        std::vector<std::string> parts;
        while ((pos = line.find(',')) != std::string::npos) {
            parts.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        parts.push_back(line);

        if (parts.size() < 5) {
            std::cerr << "Baris log tidak valid: " << line << std::endl;
            continue;
        }

        LogEntry entry;
        entry.level = parts[0];
        entry.dateTime = parts[1];
        entry.source = parts[2];
        entry.eventID = parts[3];
        entry.description = parts[4];

        // Membuat atau membuka file keluaran sesuai dengan level
        if (levelToFile.find(entry.level) == levelToFile.end()) {
            levelToFile[entry.level].open(entry.level + "_log.txt");
        }

        // Menyimpan log ke file sesuai dengan level
        levelToFile[entry.level] << entry.dateTime << "," << entry.source << "," << entry.eventID << "," << entry.description << std::endl;
    }

    // Tutup semua file keluaran
    for (auto& pair : levelToFile) {
        pair.second.close();
    }

    logFile.close();
    return 0;
}
