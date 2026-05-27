#pragma once
#include "User.h"

class Surveyor : public User {
public:
    Surveyor(std::string id, std::string n, std::string pass)
        : User(id, n, pass) {
    }

    std::string getRole() const override {
        return "Surveyor";
    }
};