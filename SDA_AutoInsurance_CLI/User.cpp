#include "User.h"

// Constructor Initialization
User::User(std::string id, std::string n, std::string pass)
    : userID(id), name(n), password(pass) {
}

std::string User::getUserID() const {
    return userID;
}

std::string User::getName() const {
    return name;
}

bool User::authenticate(const std::string& pass) const {
    return password == pass;
}