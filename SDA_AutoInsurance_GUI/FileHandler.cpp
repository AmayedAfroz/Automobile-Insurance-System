#include "FileHandler.h"
#include <fstream>
#include <iostream>

bool FileHandler::saveRecord(const std::string& filename, const std::string& data) {
    // std::ios::app opens the file in "append" mode so we don't overwrite existing records.
    // If the file doesn't exist, this automatically creates it.
    std::ofstream file(filename, std::ios::app);

    if (file.is_open()) {
        file << data << "\n";
        file.close();
        return true;
    }
    return false; // Returns false if the file is locked or permission is denied
}

std::vector<std::string> FileHandler::loadRecords(const std::string& filename) {
    std::vector<std::string> records;
    std::ifstream file(filename);
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) {
            if (!line.empty())
                records.push_back(line); // Adds each line from the text file into our C++ vector
        }
        file.close();
    }

    // If the file didn't exist, it safely returns an empty vector. Zero crashes.
    return records;
}

bool FileHandler::updateRecord(const std::string& filename,
                               const std::string& matchToken,
                               const std::string& oldValue,
                               const std::string& newValue) {
    // Load every line into memory
    std::vector<std::string> records = loadRecords(filename);
    bool updated = false;

    for (std::string& line : records) {
        // Only touch the first line that contains our target claim ID
        if (!updated && line.find(matchToken) != std::string::npos) {
            size_t pos = line.find(oldValue);
            if (pos != std::string::npos) {
                line.replace(pos, oldValue.length(), newValue);
                updated = true;
            }
        }
    }

    if (!updated) return false;

    // Overwrite the file entirely with the patched records (truncate mode)
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) return false;

    for (const std::string& line : records) {
        file << line << "\n";
    }
    file.close();
    return true;
}