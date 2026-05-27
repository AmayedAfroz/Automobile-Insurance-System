#pragma once
#include "User.h"

class Salesman : public User {
public:
    Salesman(std::string id, std::string n, std::string pass)
        : User(id, n, pass) {
    }

    std::string getRole() const override {
        return "Salesman";
    }
};