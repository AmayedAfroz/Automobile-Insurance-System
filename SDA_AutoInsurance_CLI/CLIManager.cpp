#include "CLIManager.h"
#include <iostream>
#include <string>
#include <vector>

CLIManager::CLIManager() {}
CLIManager::~CLIManager() {}

void CLIManager::run() {
    bool running = true;
    while (running) {
        std::cout << "\n=======================================================\n";
        std::cout << "        AUTOMOBILE INSURANCE SYSTEM DASHBOARD\n";
        std::cout << "=======================================================\n";
        std::cout << "1. Manager Menu\n";
        std::cout << "2. Salesman Menu\n";
        std::cout << "3. Surveyor Menu\n";
        std::cout << "4. Customer Menu\n";
        std::cout << "5. Exit\n";
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") managerMenu();
        else if (choice == "2") salesmanMenu();
        else if (choice == "3") surveyorMenu();
        else if (choice == "4") customerMenu();
        else if (choice == "5") running = false;
        else std::cout << "Invalid selection. Try again.\n";
    }
}

void CLIManager::managerMenu() {
    bool back = false;
    while (!back) {
        std::cout << "\n--- Manager Actions ---\n";
        std::cout << "1. Fetch New Customers\n";
        std::cout << "2. Fetch Pending Claims\n";
        std::cout << "3. Fetch Inspection Reports\n";
        std::cout << "4. Approve Claim\n";
        std::cout << "5. Reject Claim\n";
        std::cout << "6. Register Workshop\n";
        std::cout << "7. Fetch Customer Claim History\n";
        std::cout << "8. Back to Main Menu\n";
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            auto rep = userCtrl.fetchNewCustomersReport();
            if (rep.empty()) std::cout << "No customers found.\n";
            else {
                std::cout << "Fetched New Customers:\n";
                for (const auto& line : rep) std::cout << line << "\n";
            }
        } else if (choice == "2") {
            auto rep = claimCtrl.fetchPendingClaims();
            if (rep.empty()) std::cout << "No pending claims found.\n";
            else {
                std::cout << "Fetched Pending Claims:\n";
                for (const auto& line : rep) std::cout << line << "\n";
            }
        } else if (choice == "3") {
            auto rep = claimCtrl.fetchAllInspectionReports();
            if (rep.empty()) std::cout << "No reports on file.\n";
            else {
                std::cout << "Fetched " << rep.size() << " report(s):\n";
                for (const auto& line : rep) std::cout << line << "\n";
            }
        } else if (choice == "4") {
            std::cout << "Enter Claim ID to approve: ";
            std::string id;
            std::getline(std::cin, id);
            if (id.empty()) std::cout << "Claim ID cannot be empty.\n";
            else if (claimCtrl.approveClaim(id.c_str()))
                std::cout << "[Approved] Claim '" << id << "' approved.\n";
            else
                std::cout << "[Error] No inspection report found or claim not pending.\n";
        } else if (choice == "5") {
            std::cout << "Enter Claim ID to reject: ";
            std::string id;
            std::getline(std::cin, id);
            if (id.empty()) std::cout << "Claim ID cannot be empty.\n";
            else if (claimCtrl.rejectClaim(id.c_str()))
                std::cout << "[Rejected] Claim '" << id << "' rejected.\n";
            else
                std::cout << "[Error] Claim not found or not in Pending state.\n";
        } else if (choice == "6") {
            std::cout << "Enter Workshop ID: ";
            std::string id;
            std::getline(std::cin, id);
            std::cout << "Enter Workshop Name: ";
            std::string name;
            std::getline(std::cin, name);
            if (id.empty() || name.empty()) std::cout << "ID and Name are required.\n";
            else if (claimCtrl.registerWorkshop(id.c_str(), name.c_str()))
                std::cout << "SUCCESS - Generated ID: " << id << "\n";
            else
                std::cout << "ERROR - Failed to save.\n";
        } else if (choice == "7") {
            std::cout << "Enter Customer ID: ";
            std::string id;
            std::getline(std::cin, id);
            if (id.empty()) std::cout << "Customer ID cannot be empty.\n";
            else {
                auto rep = claimCtrl.fetchCustomerClaimHistory(id.c_str());
                if (rep.empty()) std::cout << "No claims found for customer '" << id << "'.\n";
                else {
                    std::cout << "Fetched " << rep.size() << " claim(s) for customer:\n";
                    for (const auto& line : rep) std::cout << line << "\n";
                }
            }
        } else if (choice == "8") {
            back = true;
        } else {
            std::cout << "Invalid selection. Try again.\n";
        }
    }
}

void CLIManager::salesmanMenu() {
    bool back = false;
    while (!back) {
        std::cout << "\n--- Salesman Actions ---\n";
        std::cout << "1. Register Customer\n";
        std::cout << "2. Register Vehicle\n";
        std::cout << "3. Issue Policy\n";
        std::cout << "4. Back to Main Menu\n";
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "Name *: ";
            std::string name; std::getline(std::cin, name);
            std::cout << "Password *: ";
            std::string pass; std::getline(std::cin, pass);
            std::cout << "Address: ";
            std::string addr; std::getline(std::cin, addr);
            std::cout << "Date (YYYY-MM-DD): ";
            std::string date; std::getline(std::cin, date);

            if (name.empty() || pass.empty()) std::cout << "Name and Password are required.\n";
            else {
                std::string newID = userCtrl.registerCustomer(name.c_str(), pass.c_str(), addr.c_str(), date.c_str());
                if (!newID.empty()) std::cout << "SUCCESS - Generated ID: " << newID << "\n";
                else std::cout << "ERROR - Failed to save.\n";
            }
        } else if (choice == "2") {
            std::cout << "Owner (Cust ID) *: ";
            std::string owner; std::getline(std::cin, owner);
            std::cout << "Make *: ";
            std::string make; std::getline(std::cin, make);
            std::cout << "License Plate *: ";
            std::string plate; std::getline(std::cin, plate);

            if (owner.empty() || make.empty() || plate.empty()) std::cout << "Owner ID, Make, and Plate are required.\n";
            else {
                std::string newID = userCtrl.registerVehicle(owner.c_str(), make.c_str(), plate.c_str());
                if (!newID.empty()) std::cout << "SUCCESS - Generated ID: " << newID << "\n";
                else std::cout << "ERROR - Failed to save.\n";
            }
        } else if (choice == "3") {
            std::cout << "Vehicle ID *: ";
            std::string veh; std::getline(std::cin, veh);
            std::cout << "Premium Amount *: ";
            std::string premStr; std::getline(std::cin, premStr);

            if (veh.empty() || premStr.empty()) std::cout << "Vehicle ID and Premium Amount are required.\n";
            else {
                try {
                    double prem = std::stod(premStr);
                    std::string newID = userCtrl.issuePolicy(veh.c_str(), prem);
                    if (!newID.empty()) std::cout << "SUCCESS - Generated ID: " << newID << "\n";
                    else std::cout << "ERROR - Failed to save.\n";
                } catch (...) {
                    std::cout << "ERROR - Invalid Premium Amount.\n";
                }
            }
        } else if (choice == "4") {
            back = true;
        } else {
            std::cout << "Invalid selection. Try again.\n";
        }
    }
}

void CLIManager::customerMenu() {
    bool back = false;
    while (!back) {
        std::cout << "\n--- Customer Actions ---\n";
        std::cout << "1. File Claim\n";
        std::cout << "2. View Policies\n";
        std::cout << "3. Back to Main Menu\n";
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "Policy ID *: ";
            std::string pol; std::getline(std::cin, pol);
            std::cout << "Description *: ";
            std::string desc; std::getline(std::cin, desc);

            if (pol.empty() || desc.empty()) std::cout << "Policy ID and Description are required.\n";
            else {
                std::string newID = claimCtrl.fileClaim(pol.c_str(), desc.c_str());
                if (!newID.empty()) std::cout << "SUCCESS - Generated ID: " << newID << "\n";
                else std::cout << "ERROR - Failed to save.\n";
            }
        } else if (choice == "2") {
            std::cout << "Customer ID *: ";
            std::string id; std::getline(std::cin, id);

            if (id.empty()) std::cout << "Customer ID is required.\n";
            else {
                auto rep = userCtrl.fetchCustomerPolicies(id.c_str());
                if (rep.empty()) std::cout << "No active policies for ID: " << id << "\n";
                else {
                    std::cout << "Fetched " << rep.size() << " policy/policies:\n";
                    for (const auto& line : rep) std::cout << line << "\n";
                }
            }
        } else if (choice == "3") {
            back = true;
        } else {
            std::cout << "Invalid selection. Try again.\n";
        }
    }
}

void CLIManager::surveyorMenu() {
    bool back = false;
    while (!back) {
        std::cout << "\n--- Surveyor Actions ---\n";
        std::cout << "1. Submit Inspection Report\n";
        std::cout << "2. Back to Main Menu\n";
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            std::cout << "Associated Claim ID *: ";
            std::string claim; std::getline(std::cin, claim);
            std::cout << "Workshop ID *: ";
            std::string work; std::getline(std::cin, work);
            std::cout << "Total Loss? (y/n): ";
            std::string lossStr; std::getline(std::cin, lossStr);
            std::cout << "Estimated Cost: ";
            std::string costStr; std::getline(std::cin, costStr);

            if (claim.empty() || work.empty()) std::cout << "Claim ID and Workshop ID are required.\n";
            else {
                bool loss = (lossStr == "y" || lossStr == "Y");
                double cost = 0.0;
                if (!costStr.empty()) {
                    try { cost = std::stod(costStr); }
                    catch (...) {}
                }

                std::string newID = claimCtrl.submitInspectionReport(claim.c_str(), work.c_str(), loss, cost);
                if (!newID.empty()) std::cout << "SUCCESS - Generated ID: " << newID << "\n";
                else std::cout << "ERROR - Failed to save.\n";
            }
        } else if (choice == "2") {
            back = true;
        } else {
            std::cout << "Invalid selection. Try again.\n";
        }
    }
}
