#include "Gate.h"

bool Gate::assignFlight(std::shared_ptr<Flight> flight) {
    if (status == Status::OCCUPIED || flight == nullptr)
        return false;

    this->flight = flight;
    this->flight->assignGate(number);
    status = Status::OCCUPIED;
    return true;
}

void Gate::releaseFlight() {
    flight->unassignGate();
    flight = nullptr;
    status = Status::VACANT;
}