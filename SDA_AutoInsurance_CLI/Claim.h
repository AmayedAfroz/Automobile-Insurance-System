#pragma once
#include <string>

class Claim {
private:
    std::string claimID;
    std::string policyID;
    std::string status; // "Pending", "Approved", "Rejected"
    std::string incidentDescription;

public:
    Claim(std::string cID, std::string pID, std::string desc)
        : claimID(cID), policyID(pID), status("Pending"), incidentDescription(desc) {
    }

    std::string getClaimID() const { return claimID; }
    std::string getStatus() const { return status; }

    // This allows the Manager to change the status later
    void updateStatus(std::string newStatus) { status = newStatus; }
};