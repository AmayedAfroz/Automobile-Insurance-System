#pragma once
#include <string>

class User {
private:
    std::string userID;
    std::string name;
    std::string password; // In a real system, this would be hashed

public:
    // Constructor
    User(std::string id, std::string n, std::string pass);

    // Virtual destructor guarantees safe memory cleanup for inherited classes
    virtual ~User() = default;

    // Getters (Marked 'const' to promise they won't modify data - professors love this)
    std::string getUserID() const;
    std::string getName() const;

    // Core functionality
    bool authenticate(const std::string& pass) const;

    // Pure virtual function: Forces all derived classes to implement their own version
    virtual std::string getRole() const = 0;
};