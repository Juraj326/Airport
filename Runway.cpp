#include "Runway.h"

bool Runway::assignFlight(std::shared_ptr<Flight> flight) {
    if (status == Status::OCCUPIED || flight == nullptr)
        return false;

    this->flight = flight;
    this->flight->assignRunway(number);
    status = Status::OCCUPIED;
    return true;
}

void Runway::releaseFlight() {
    flight->unassignRunway();
    flight = nullptr;
    status = Status::VACANT;
}