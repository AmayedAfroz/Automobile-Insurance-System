#include "UserController.h"
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cctype>

// ─── Helpers ─────────────────────────────────────────────────────────────────

static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace((unsigned char)*start)) ++start;
    auto end = s.end();
    do { --end; } while (end != start && std::isspace((unsigned char)*end));
    return (start <= end) ? std::string(start, end + 1) : "";
}

std::string UserController::generateID(const std::string& prefix) const {
    long long ts = static_cast<long long>(std::time(nullptr));
    int rnd = std::rand() % 9000 + 1000;
    return prefix + std::to_string(ts) + "-" + std::to_string(rnd);
}

// ─── User ─────────────────────────────────────────────────────────────────────

std::string UserController::registerCustomer(std::string name, std::string pass, std::string addr, std::string date) {
    std::string id = generateID("CUST-");
    // Format: ID|Name|Password|Role|Address|Date
    std::string record = id + "|" + name + "|" + pass + "|Customer|" + addr + "|" + date;
    if (fileHandler.saveRecord(usersFile, record))
        return id;
    return "";
}

bool UserController::authenticateUser(std::string id, std::string pass) {
    std::vector<std::string> users = fileHandler.loadRecords(usersFile);
    for (const std::string& line : users) {
        std::stringstream ss(line);
        std::string storedId, storedName, storedPass;
        std::getline(ss, storedId, '|');
        std::getline(ss, storedName, '|');
        std::getline(ss, storedPass, '|');
        if (trim(storedId) == id && trim(storedPass) == pass)
            return true;
    }
    return false;
}

std::vector<std::string> UserController::fetchNewCustomersReport() {
    std::vector<std::string> report;
    std::vector<std::string> users = fileHandler.loadRecords(usersFile);
    for (const std::string& line : users) {
        if (line.find("|Customer|") != std::string::npos)
            report.push_back(line);
    }
    return report;
}

// ─── Vehicle & Policy ─────────────────────────────────────────────────────────

std::string UserController::registerVehicle(std::string ownerID, std::string make, std::string plate) {
    std::string vID = generateID("VEH-");
    std::string record = vID + "|" + ownerID + "|" + make + "|" + plate;
    if (fileHandler.saveRecord(vehiclesFile, record))
        return vID;
    return "";
}

std::string UserController::issuePolicy(std::string vID, double premiumAmount) {
    std::string pID = generateID("POL-");
    std::string record = pID + "|" + vID + "|" + std::to_string(premiumAmount) + "|1";
    if (fileHandler.saveRecord(policiesFile, record))
        return pID;
    return "";
}

std::vector<std::string> UserController::fetchCustomerPolicies(std::string customerID) {
    std::vector<std::string> result;
    std::vector<std::string> vehicles = fileHandler.loadRecords(vehiclesFile);
    std::vector<std::string> policies = fileHandler.loadRecords(policiesFile);

    // Step 1: Find all vehicles owned by this customer (col 1 = ownerID)
    std::vector<std::string> ownedVehicles;
    for (const std::string& vLine : vehicles) {
        std::stringstream ss(vLine);
        std::string vID, oID;
        std::getline(ss, vID, '|');
        std::getline(ss, oID, '|');
        if (trim(oID) == customerID)
            ownedVehicles.push_back(trim(vID));
    }

    // Step 2: Find all policies attached to those vehicles
    for (const std::string& pLine : policies) {
        std::stringstream ss(pLine);
        std::string pID, vID;
        std::getline(ss, pID, '|');
        std::getline(ss, vID, '|');
        for (const std::string& ov : ownedVehicles) {
            if (trim(vID) == ov) {
                result.push_back(pLine);
                break;
            }
        }
    }
    return result;
}





