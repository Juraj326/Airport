#include "Gate.h"

/**
 * Metóda, ktorá priradí ku gate-u let.
 * @param flight Let, ktorí sa má priradiť.
 * @return True ak je gate voľný.
 */
bool Gate::assignFlight(std::shared_ptr<Flight> flight) {
    if (status == Status::OCCUPIED || flight == nullptr)
        return false;

    this->flight = flight;
    this->flight->assignGate(number);
    status = Status::OCCUPIED;
    return true;
}

/**
 * Metóda, ktorá uvoľní gate.
 */
void Gate::releaseFlight() {
    flight->unassignGate();
    flight = nullptr;
    status = Status::VACANT;
}