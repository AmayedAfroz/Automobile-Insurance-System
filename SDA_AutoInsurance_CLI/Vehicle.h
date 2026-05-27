#pragma once
#include <string>

class Vehicle {
private:
    std::string vehicleID;
    std::string ownerID;      // Links vehicle to its Customer owner
    std::string make;
    std::string licensePlate;

public:
    Vehicle(std::string id, std::string owner, std::string m, std::string lp)
        : vehicleID(id), ownerID(owner), make(m), licensePlate(lp) {
    }

    std::string getVehicleID() const { return vehicleID; }
    std::string getOwnerID() const { return ownerID; }
    std::string getMake() const { return make; }
    std::string getLicensePlate() const { return licensePlate; }
};