#pragma once
#include <string>

class Workshop {
private:
    std::string workshopID;
    std::string name;
    bool isRegistered;

public:
    Workshop(std::string id, std::string n, bool registeredStatus);

    std::string getWorkshopID() const;
    std::string getName() const;

    // The core business rule checker
    bool verifyRegistration() const;
};