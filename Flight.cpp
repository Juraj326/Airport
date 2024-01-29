#include "Flight.h"

/**
 * Konštruktor, ktorý vytvorí let na plánovanie.
 * @param number Číslo letu.
 * @param status Fáza letu.
 */
Flight::Flight(int number, FlightStatus status) :  status(status) {
    if (number < MIN_FLIGHT_NUMBER || number > MAX_FLIGHT_NUMBER)
        throw std::invalid_argument("Flight number must be an integer ranging from 0 to 9999.");

    this->number = number;
}

/**
 * Metóda, ktorá simuluje plánovanie letu.
 * @return True ak má let priradený pôvod a destináciu.
 */
bool Flight::schedule() {
    if (status != FlightStatus::SCHEDULING || origin.empty() || destination.empty())
        return false;

    status = FlightStatus::BOARDING;
    return true;
}

/**
 * Metóda, ktorá simuluje boardovanie letu.
 * @return True ak je let naplánovaný a bol mu priradený gate.
 */
bool Flight::board() {
    if (status != FlightStatus::BOARDING)
        return false;

    status = FlightStatus::TAKING_OFF;
    return true;
}

/**
 * Metóda, ktorá simuluje odlet letu.
 * @return True ak je let vo fáze boardovania a bola mu priradená runway.
 */
bool Flight::depart() {
    if (status != FlightStatus::TAKING_OFF)
        return false;

    status = FlightStatus::DEPARTING;
    return true;
}

/**
 * Metóda, ktorá simuluje oznam o prílete letu.
 * @return True ak let má destináciu a pôvod.
 */
bool Flight::initiateArrival() {
    if (status != FlightStatus::SCHEDULING || destination.empty() || origin.empty())
        return false;

    status = FlightStatus::ARRIVING;
    return true;
}

/**
 * Metóda, ktorá simuluje pristátie letu.
 * @return True ak let prichádza a má priradenú runway.
 */
bool Flight::land() {
    if (status != FlightStatus::ARRIVING)
        return false;

    status = FlightStatus::LANDING;
    return true;
}

/**
 * Metóda, ktorá simuluje koniec letu.
 * @return True ak let pristál a má priradený gate.
 */
bool Flight::disembark() {
    if (status != FlightStatus::LANDING)
        return false;

    status = FlightStatus::DISEMBARKING;
    return true;
}

/**
 * Metóda, ktorá priradí letu pôvod.
 * @param destination Mesto z množiny spojení.
 * @return True ak je mesto platné.
 */
bool Flight::setOrigin(const std::string &origin) {
    if (!this->origin.empty() || origin.empty() || origin == destination)
        return false;

    this->origin = origin;
    return true;
}

/**
 * Metóda, ktorá priradí letu destináciu.
 * @param destination Mesto z množiny spojení.
 * @return True ak je mesto platné.
 */
bool Flight::setDestination(const std::string &destination) {
    if (!this->destination.empty() || destination.empty() || destination == origin)
        return false;

    this->destination = destination;
    return true;
}

/**
 * Metóda, ktorá vracia stringovú reprezentáciu fázy letu.
 * @return Stringová reprezentácia fázy letu.
 */
std::string Flight::getFlightStatusString() const {
    switch (status) {
        case FlightStatus::BOARDING:
            return "Boarding";
        case FlightStatus::TAKING_OFF:
            return "Taking off";
        case FlightStatus::DEPARTING:
            return "Departing";
        case FlightStatus::ARRIVING:
            return "Arriving";
        case FlightStatus::LANDING:
            return "Landing";
        case FlightStatus::DISEMBARKING:
            return "Disembarking";
        default:
            return "Scheduling";
    }
}

/**
 * Porovnávanie podľa priority fázy letu. Priorita je podľa poradia definície fáz letu.
 * @param flight2 Let, s ktorým sa porovnáva priorita.
 * @return True ak má flight (this) nižšiu prioritu ako flight2.
 */
bool Flight::operator<(const Flight &flight2) const {
    return status < flight2.status;
}