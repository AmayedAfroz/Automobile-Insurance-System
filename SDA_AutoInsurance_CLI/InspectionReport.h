#pragma once
#include <string>

class InspectionReport {
private:
    std::string reportID;
    std::string claimID; // Links this report to a specific claim
    std::string workshopID; // The registered workshop where it was inspected
    bool isTotalLoss;
    double estimatedCost;

public:
    InspectionReport(std::string rID, std::string cID, std::string wID, bool totalLoss, double cost)
        : reportID(rID), claimID(cID), workshopID(wID), isTotalLoss(totalLoss), estimatedCost(cost) {
    }

    std::string getReportID() const { return reportID; }
    std::string getClaimID() const { return claimID; }
    std::string getWorkshopID() const { return workshopID; }
};