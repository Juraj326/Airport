#include "Gate.h"

/*
 * Pridelenie letu gate-u.
 */
bool Gate::assignFlight(std::shared_ptr<Flight> flight) {
    if (status == Status::OCCUPIED || flight == nullptr)
        return false;

    this->flight = flight;
    this->flight->assignGate(number);
    status = Status::OCCUPIED;
    return true;
}


/*
 * Uvolnenie letu z Gate-u.
 */
void Gate::releaseFlight() {
    flight->unassignGate();
    flight = nullptr;
    status = Status::VACANT;
}