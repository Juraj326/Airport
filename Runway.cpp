#include "Runway.h"

/**
 * Metóda, ktorá priradí ku runway let.
 * @param flight Let, ktorí sa má priradiť.
 * @return True ak je runway prázdna.
 */
bool Runway::assignFlight(std::shared_ptr<Flight> flight) {
    if (status == Status::OCCUPIED || flight == nullptr)
        return false;

    this->flight = flight;
    status = Status::OCCUPIED;
    return true;
}

/**
 * Metóda, ktorá uvoľní runway.
 */
void Runway::releaseFlight() {
    flight = nullptr;
    status = Status::VACANT;
}