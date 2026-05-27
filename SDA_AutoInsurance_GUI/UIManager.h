#pragma once
#include "UserController.h"
#include "ClaimController.h"
#include <string>
#include <vector>

struct GLFWwindow;

class UIManager {
private:
    UserController userCtrl;
    ClaimController claimCtrl;

    GLFWwindow* window;

    // Output messages
    std::vector<std::string> reportOutput;
    std::string systemMessage;

    // ── Salesman Tab ─────────────────────────────────────────────────────
    char regName[128] = "";
    char regPass[128] = "";
    char regAddr[128] = "";
    char regDate[128] = "";

    char vehOwner[128] = "";
    char vehMake[128] = "";
    char vehPlate[128] = "";

    char polVeh[128] = "";
    double polPremium = 0.0;

    // ── Customer Tab ──────────────────────────────────────────────────────
    char claimPol[128] = "";
    char claimDesc[256] = "";
    char custViewId[128] = "";
    std::vector<std::string> custPolicies;

    // ── Surveyor Tab ──────────────────────────────────────────────────────
    char repClaim[128] = "";
    char repWork[128] = "";
    bool repTotalLoss = false;
    double repCost = 0.0;

    // ── Manager Tab ───────────────────────────────────────────────────────
    char mgrApproveId[128] = "";
    char mgrRejectId[128] = "";
    char mgrHistoryCustId[128] = "";
    std::vector<std::string> claimHistoryOutput;

    char wshpRegId[64] = "";
    char wshpRegName[128] = "";
    std::vector<std::string> inspReportsOutput;

    std::string validationError;

    // Specific actor tabs
    void renderManagerTab();
    void renderSalesmanTab();
    void renderCustomerTab();
    void renderSurveyorTab();

    void renderStyles();

public:
    UIManager();
    ~UIManager();

    // Setup and Main Loop
    bool init();
    void run();
    void shutdown();
};