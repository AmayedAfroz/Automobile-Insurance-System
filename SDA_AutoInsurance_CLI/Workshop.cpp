#include "Workshop.h"

Workshop::Workshop(std::string id, std::string n, bool registeredStatus)
    : workshopID(id), name(n), isRegistered(registeredStatus) {
}

std::string Workshop::getWorkshopID() const {
    return workshopID;
}

std::string Workshop::getName() const {
    return name;
}

bool Workshop::verifyRegistration() const {
    return isRegistered;
}
