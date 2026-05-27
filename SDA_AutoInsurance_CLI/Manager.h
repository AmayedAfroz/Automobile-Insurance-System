#pragma once
#include "User.h"

class Manager : public User {
public:
    Manager(std::string id, std::string n, std::string pass)
        : User(id, n, pass) {
    }

    std::string getRole() const override {
        return "Manager";
    }
};