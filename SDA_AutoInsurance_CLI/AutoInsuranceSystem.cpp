#include <iostream>
#include <cstdlib>
#include <chrono>
#include "CLIManager.h"
#include "ClaimController.h"

int main() {
    // Seed RNG with nanosecond precision to avoid same-second ID collisions
    auto ns = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(static_cast<unsigned int>(ns));

    // Auto-seed workshops.txt if it is empty or missing (P1-A)
    ClaimController bootstrapCtrl;
    bootstrapCtrl.seedDefaultWorkshops();

    // Instantiate and start the CLI Presentation Layer
    CLIManager cli;
    cli.run();

    return 0;
}