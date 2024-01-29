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
 * @return True ak bol let vo fáze CREATING a letu bol priradený pôvod a destinácia.
 */
bool Flight::schedule() {
    if (status != FlightStatus::CREATING || origin.empty() || destination.empty())
        return false;

    status = FlightStatus::SCHEDULED;
    return true;
}

/**
 * Metóda, ktorá simuluje boardovanie letu.
 * @return True ak bol let vo fáze SCHEDULED.
 */
bool Flight::board() {
    if (status != FlightStatus::SCHEDULED)
        return false;

    status = FlightStatus::BOARDED;
    return true;
}

/**
 * Metóda, ktorá simuluje odlet letu.
 * @return True ak bol let vo fáze SCHEDULED.
 */
bool Flight::take_off() {
    if (status != FlightStatus::BOARDED)
        return false;

    status = FlightStatus::DEPARTED;
    return true;
}

/**
 * Metóda, ktorá simuluje oznam o prílete letu.
 * @return True ak bol let vo fáze CREATING a letu bol priradený pôvod a destinácia.
 */
bool Flight::initiateArrival() {
    if (status != FlightStatus::CREATING || destination.empty() || origin.empty())
        return false;

    status = FlightStatus::ARRIVING;
    return true;
}

/**
 * Metóda, ktorá simuluje pristátie letu.
 * @return True ak bol let vo fáze ARRIVING.
 */
bool Flight::land() {
    if (status != FlightStatus::ARRIVING)
        return false;

    status = FlightStatus::LANDED;
    return true;
}

/**
 * Metóda, ktorá simuluje koniec letu.
 * @return True ak bol let vo fáze LANDED.
 */
bool Flight::disembark() {
    if (status != FlightStatus::LANDED)
        return false;

    status = FlightStatus::DISEMBARKED;
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
 * @param status Fáza letu.
 * @return Stringová reprezentácia fázy letu.
 */
std::string flightStatusToString(FlightStatus status) {
    switch (status) {
        case FlightStatus::SCHEDULED:
            return "Scheduled";
        case FlightStatus::BOARDED:
            return "Boarded";
        case FlightStatus::DEPARTED:
            return "Departed";
        case FlightStatus::ARRIVING:
            return "Arriving";
        case FlightStatus::LANDED:
            return "Landed";
        case FlightStatus::DISEMBARKED:
            return "Disembarked";
        default:
            return "Creating";
    }
}

/**
 * Metóda, ktorá vracia stringovú reprezentáciu fázy letu ako enumerátor.
 * @param status Fáza letu.
 * @return Enumerátor.
 */
FlightStatus stringToFlightStatus(const std::string &status) {
    std::map<std::string, FlightStatus> flightStates = {
            {"Creating", FlightStatus::CREATING},
            {"Scheduled", FlightStatus::SCHEDULED},
            {"Boarded", FlightStatus::BOARDED},
            {"Departed", FlightStatus::DEPARTED},
            {"Arriving", FlightStatus::ARRIVING},
            {"Landed", FlightStatus::LANDED},
            {"Disembarked", FlightStatus::DISEMBARKED}
    };

    auto flightStatus = flightStates.find(status);
    if (flightStatus == flightStates.end()) {
        std::ostringstream errorMsg;
        errorMsg << status << " is not a valid flight status.";
        throw std::invalid_argument(errorMsg.str());
    }
    return flightStatus->second;
}
