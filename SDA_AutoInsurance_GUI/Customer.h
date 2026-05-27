#pragma once
#include "User.h"

class Customer : public User {
private:
    std::string address;
    std::string registrationDate;

public:
    // Constructor passes the id, name, and password up to the base User class
    Customer(std::string id, std::string n, std::string pass, std::string addr, std::string regDate);

    // Getters
    std::string getAddress() const;
    std::string getRegistrationDate() const;

    // Overriding the pure virtual function from User
    std::string getRole() const override;
};