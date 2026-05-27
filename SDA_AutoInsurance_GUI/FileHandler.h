#pragma once
#include <string>
#include <vector>

class FileHandler {
public:
    // Saves a single delimited string (e.g., "ID,Name,Status") to a text file
    bool saveRecord(const std::string& filename, const std::string& data);

    // Reads a text file line by line and returns a vector of strings
    std::vector<std::string> loadRecords(const std::string& filename);

    // Finds the first line containing matchToken, replaces oldValue with newValue,
    // and overwrites the entire file. Used for in-place status updates.
    bool updateRecord(const std::string& filename,
                      const std::string& matchToken,
                      const std::string& oldValue,
                      const std::string& newValue);
};