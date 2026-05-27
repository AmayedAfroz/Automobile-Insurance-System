#include "ClaimController.h"
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <fstream>

// ─── Helpers ─────────────────────────────────────────────────────────────────

static std::string trimCC(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace((unsigned char)*start)) ++start;
    auto end = s.end();
    do { --end; } while (end != start && std::isspace((unsigned char)*end));
    return (start <= end) ? std::string(start, end + 1) : "";
}

std::string ClaimController::generateID(const std::string& prefix) const {
    long long ts = static_cast<long long>(std::time(nullptr));
    int rnd = std::rand() % 9000 + 1000;
    return prefix + std::to_string(ts) + "-" + std::to_string(rnd);
}

// ─── Workshop Admin ───────────────────────────────────────────────────────────

bool ClaimController::registerWorkshop(std::string id, std::string name) {
    // Format: ID|Name|1  (1 = registered)
    std::string record = id + "|" + name + "|1";
    return fileHandler.saveRecord(workshopsFile, record);
}

void ClaimController::seedDefaultWorkshops() {
    // Only seed if the file is empty or missing
    std::vector<std::string> existing = fileHandler.loadRecords(workshopsFile);
    if (!existing.empty()) return;

    fileHandler.saveRecord(workshopsFile, "W-001|Stark Auto Repairs|1");
    fileHandler.saveRecord(workshopsFile, "W-002|Rhodes Collision Center|1");
    fileHandler.saveRecord(workshopsFile, "W-003|Pepper's Panel Beaters|1");
}

// ─── Customer ─────────────────────────────────────────────────────────────────

std::string ClaimController::fileClaim(std::string policyID, std::string desc) {
    std::string claimID = generateID("CLM-");
    // Format: ClaimID|PolicyID|Status|Description
    std::string record = claimID + "|" + policyID + "|Pending|" + desc;
    if (fileHandler.saveRecord(claimsFile, record))
        return claimID;
    return "";
}

// ─── Surveyor ─────────────────────────────────────────────────────────────────

bool ClaimController::validateWorkshopRegistration(std::string workshopID) {
    std::vector<std::string> workshops = fileHandler.loadRecords(workshopsFile);
    for (const std::string& line : workshops) {
        std::stringstream ss(line);
        std::string id, name, isRegistered;
        std::getline(ss, id, '|');
        std::getline(ss, name, '|');
        std::getline(ss, isRegistered, '|');

        if (trimCC(id) == workshopID && trimCC(isRegistered) == "1")
            return true;
    }
    return false;
}

std::string ClaimController::submitInspectionReport(std::string claimID, std::string workshopID, bool isTotalLoss, double cost) {
    // BUSINESS RULE: damaged vehicle can only be repaired at a registered workshop
    if (!validateWorkshopRegistration(workshopID))
        return "";

    std::string reportID = generateID("RPT-");
    std::string lossStr = isTotalLoss ? "1" : "0";
    // Format: ReportID|ClaimID|WorkshopID|TotalLoss|Cost
    std::string record = reportID + "|" + claimID + "|" + workshopID + "|" + lossStr + "|" + std::to_string(cost);
    if (fileHandler.saveRecord(reportsFile, record))
        return reportID;
    return "";
}

// ─── Manager ──────────────────────────────────────────────────────────────────

std::vector<std::string> ClaimController::fetchPendingClaims() {
    std::vector<std::string> pending;
    std::vector<std::string> claims = fileHandler.loadRecords(claimsFile);
    for (const std::string& line : claims) {
        if (line.find("|Pending|") != std::string::npos)
            pending.push_back(line);
    }
    return pending;
}

bool ClaimController::approveClaim(std::string claimID) {
    // BUSINESS RULE: manager can only approve a claim that has an inspection report
    std::vector<std::string> reports = fileHandler.loadRecords(reportsFile);
    bool hasReport = false;
    for (const std::string& line : reports) {
        std::stringstream ss(line);
        std::string rID, cID;
        std::getline(ss, rID, '|');
        std::getline(ss, cID, '|');
        if (trimCC(cID) == claimID) { hasReport = true; break; }
    }
    if (!hasReport) return false;

    return fileHandler.updateRecord(claimsFile, claimID, "|Pending|", "|Approved|");
}

bool ClaimController::rejectClaim(std::string claimID) {
    // Only Pending claims can be rejected
    std::vector<std::string> claims = fileHandler.loadRecords(claimsFile);
    bool isPending = false;
    for (const std::string& line : claims) {
        if (line.find(claimID) != std::string::npos &&
            line.find("|Pending|") != std::string::npos) {
            isPending = true;
            break;
        }
    }
    if (!isPending) return false;
    return fileHandler.updateRecord(claimsFile, claimID, "|Pending|", "|Rejected|");
}

std::vector<std::string> ClaimController::fetchAllInspectionReports() {
    return fileHandler.loadRecords(reportsFile);
}

std::vector<std::string> ClaimController::fetchCustomerClaimHistory(std::string customerID) {
    std::vector<std::string> result;

    // Step 1: Find all vehicles owned by this customer
    std::vector<std::string> vehicles = fileHandler.loadRecords(vehiclesFile);
    std::vector<std::string> ownedVehicleIDs;
    for (const std::string& vLine : vehicles) {
        std::stringstream ss(vLine);
        std::string vID, oID;
        std::getline(ss, vID, '|');
        std::getline(ss, oID, '|');
        if (trimCC(oID) == customerID)
            ownedVehicleIDs.push_back(trimCC(vID));
    }

    // Step 2: Find all policy IDs linked to those vehicles
    std::vector<std::string> policies = fileHandler.loadRecords(policiesFile);
    std::vector<std::string> linkedPolicyIDs;
    for (const std::string& pLine : policies) {
        std::stringstream ss(pLine);
        std::string pID, vID;
        std::getline(ss, pID, '|');
        std::getline(ss, vID, '|');
        for (const std::string& ov : ownedVehicleIDs) {
            if (trimCC(vID) == ov) { linkedPolicyIDs.push_back(trimCC(pID)); break; }
        }
    }

    // Step 3: Find all claims whose policyID matches
    std::vector<std::string> claims = fileHandler.loadRecords(claimsFile);
    for (const std::string& cLine : claims) {
        std::stringstream ss(cLine);
        std::string claimID, policyID;
        std::getline(ss, claimID, '|');
        std::getline(ss, policyID, '|');
        for (const std::string& lp : linkedPolicyIDs) {
            if (trimCC(policyID) == lp) { result.push_back(cLine); break; }
        }
    }

    return result;
}