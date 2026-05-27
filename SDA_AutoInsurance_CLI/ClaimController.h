#pragma once
#include <string>
#include <vector>
#include "FileHandler.h"

class ClaimController {
private:
    FileHandler fileHandler;
    std::string claimsFile    = "claims.txt";
    std::string reportsFile   = "reports.txt";
    std::string workshopsFile = "workshops.txt";
    std::string vehiclesFile  = "vehicles.txt";
    std::string policiesFile  = "policies.txt";

    std::string generateID(const std::string& prefix) const;

public:
    // ── Customer ──────────────────────────────────────────────────────────────
    // Returns auto-generated claimID, or "" on failure
    std::string fileClaim(std::string policyID, std::string desc);

    // ── Surveyor ──────────────────────────────────────────────────────────────
    bool validateWorkshopRegistration(std::string workshopID);
    // Returns auto-generated reportID, or "" on failure
    std::string submitInspectionReport(std::string claimID, std::string workshopID, bool isTotalLoss, double cost);

    // ── Manager ───────────────────────────────────────────────────────────────
    std::vector<std::string> fetchPendingClaims();
    bool approveClaim(std::string claimID);
    bool rejectClaim(std::string claimID);                        // NEW (P2-A)
    std::vector<std::string> fetchAllInspectionReports();         // NEW (P2-B)
    std::vector<std::string> fetchCustomerClaimHistory(std::string customerID);

    // ── Workshop Admin ────────────────────────────────────────────────────────
    bool registerWorkshop(std::string id, std::string name);      // NEW (P1-C)
    void seedDefaultWorkshops();                                   // NEW (P1-A)
};
