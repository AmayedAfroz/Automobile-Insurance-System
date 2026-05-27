#include <iostream>
#include <cstdlib>
#include <chrono>
#include "UIManager.h"
#include "ClaimController.h"

int main() {
    // Seed RNG with nanosecond precision to avoid same-second ID collisions
    auto ns = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(static_cast<unsigned int>(ns));

    // Auto-seed workshops.txt if it is empty or missing (P1-A)
    ClaimController bootstrapCtrl;
    bootstrapCtrl.seedDefaultWorkshops();

    // Instantiate and start the Presentation Layer
    UIManager ui;
    if (ui.init()) {
        ui.run();
    } else {
        std::cerr << "Failed to initialize the UI." << std::endl;
        return -1;
    }

    ui.shutdown();
    return 0;
}