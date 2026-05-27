#pragma once
#include <string>
#include <vector>
#include "FileHandler.h"

class UserController {
private:
    FileHandler fileHandler;
    std::string usersFile = "users.txt";
    std::string vehiclesFile = "vehicles.txt";
    std::string policiesFile = "policies.txt";

    // Helper: auto-generate a unique ID with a given prefix
    std::string generateID(const std::string& prefix) const;

public:
    // User logic
    // Returns the auto-generated customerID, or "" on failure
    std::string registerCustomer(std::string name, std::string pass, std::string addr, std::string date);
    bool authenticateUser(std::string id, std::string pass);
    std::vector<std::string> fetchNewCustomersReport();

    // Vehicle and Policy logic
    // Returns the auto-generated vehicleID, or "" on failure
    std::string registerVehicle(std::string ownerID, std::string make, std::string plate);
    // Returns the auto-generated policyID, or "" on failure
    std::string issuePolicy(std::string vID, double premiumAmount);
    std::vector<std::string> fetchCustomerPolicies(std::string customerID);
};