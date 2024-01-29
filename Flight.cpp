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
 * @return True ak bol let vo fáze SCHEDULING a letu bol priradený pôvod a destinácia.
 */
bool Flight::schedule() {
    if (status != FlightStatus::SCHEDULING || origin.empty() || destination.empty())
        return false;

    status = FlightStatus::BOARDING;
    return true;
}

/**
 * Metóda, ktorá simuluje boardovanie letu.
 * @return True ak bol let vo fáze BOARDING.
 */
bool Flight::board() {
    if (status != FlightStatus::BOARDING)
        return false;

    status = FlightStatus::TAKE_OFF;
    return true;
}

/**
 * Metóda, ktorá simuluje odlet letu.
 * @return True ak bol let vo fáze BOARDING.
 */
bool Flight::take_off() {
    if (status != FlightStatus::TAKE_OFF)
        return false;

    status = FlightStatus::DEPARTING;
    return true;
}

/**
 * Metóda, ktorá simuluje oznam o prílete letu.
 * @return True ak bol let vo fáze SCHEDULING a letu bol priradený pôvod a destinácia.
 */
bool Flight::initiateArrival() {
    if (status != FlightStatus::SCHEDULING || destination.empty() || origin.empty())
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

    status = FlightStatus::LANDING;
    return true;
}

/**
 * Metóda, ktorá simuluje koniec letu.
 * @return True ak bol let vo fáze LANDING.
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
 * @param status Fáza letu.
 * @return Stringová reprezentácia fázy letu.
 */
std::string flightStatusToString(FlightStatus status) {
    switch (status) {
        case FlightStatus::BOARDING:
            return "Boarding";
        case FlightStatus::TAKE_OFF:
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
 * Metóda, ktorá vracia stringovú reprezentáciu fázy letu ako enumerátor.
 * @param status Fáza letu.
 * @return Enumerátor.
 */
FlightStatus stringToFlightStatus(const std::string &status) {
    std::map<std::string, FlightStatus> flightStates = {
            {"Scheduling", FlightStatus::SCHEDULING},
            {"Boarding", FlightStatus::BOARDING},
            {"Taking off", FlightStatus::TAKE_OFF},
            {"Departing", FlightStatus::DEPARTING},
            {"Arriving", FlightStatus::ARRIVING},
            {"Landing", FlightStatus::LANDING},
            {"Disembarking", FlightStatus::DISEMBARKING}
    };

    auto flightStatus = flightStates.find(status);
    if (flightStatus == flightStates.end()) {
        std::ostringstream errorMsg;
        errorMsg << status << " is not a valid flight status.";
        throw std::invalid_argument(errorMsg.str());
    }
    return flightStatus->second;
}
