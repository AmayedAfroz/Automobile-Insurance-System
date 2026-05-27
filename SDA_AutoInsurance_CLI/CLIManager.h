#pragma once
#include "UserController.h"
#include "ClaimController.h"
#include <string>

class CLIManager {
private:
    UserController userCtrl;
    ClaimController claimCtrl;

    void managerMenu();
    void salesmanMenu();
    void customerMenu();
    void surveyorMenu();

public:
    CLIManager();
    ~CLIManager();

    void run();
};
