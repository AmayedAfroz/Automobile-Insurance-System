#pragma once
#include <string>

class InsurancePolicy {
private:
    std::string policyID;
    std::string vehicleID;
    double premiumAmount;
    bool activeStatus;

public:
    InsurancePolicy(std::string pID, std::string vID, double premium, bool status)
        : policyID(pID), vehicleID(vID), premiumAmount(premium), activeStatus(status) {
    }

    std::string getPolicyID() const { return policyID; }
    std::string getVehicleID() const { return vehicleID; }
    bool isValid() const { return activeStatus; }
};