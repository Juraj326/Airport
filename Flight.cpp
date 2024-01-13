#include "Flight.h"

/*
 * nastavi cislo letu a mnozinu miest a status na vytvara sa
 */
Flight::Flight(int number, std::shared_ptr<Cities> connections) : connections(std::move(connections)), status(FlightStatus::CREATING),
                                                                                        gateNumber(-1), runwayNumber(-1) {
    if (number < MIN_FLIGHT_NUMBER || number > MAX_FLIGHT_NUMBER)
        throw std::invalid_argument("Flight number must be an integer ranging from 0 to 9999.");

    this->number = number;
}

/*
 * prirpava na letu
 */
bool Flight::schedule() {
    if (status != FlightStatus::CREATING || origin.empty() || destination.empty())
        return false;

    status = FlightStatus::SCHEDULING;
    return true;
}

/*
 * boardovanie letu, gate uz musi byt prideleny, lebo na nom caka lietadlo
 */
bool Flight::board() {
    if (status != FlightStatus::SCHEDULING)
        return false;
    if (!hasAssignedGate())
        return false;

    status = FlightStatus::BOARDING;
    return true;
}

/*
 * odlet z letiska
 */
bool Flight::depart() {
    if (status != FlightStatus::BOARDING)
        return false;
    if (!hasAssignedRunway())
        return false;

    status = FlightStatus::DEPARTING;
    return true;
}

/*
 * priprava letu na prilet, prvy kontakt s letiskom
 */
bool Flight::initiateArrival() {
    if (status != FlightStatus::CREATING || destination.empty() || origin.empty())
        return false;

    status = FlightStatus::ARRIVING;
    return true;
}

/*
 * pristavanie, musi mat runway lebo uz na nu ide pristat
 */
bool Flight::land() {
    if (status != FlightStatus::ARRIVING)
        return false;
    if (!hasAssignedRunway())
        return false;

    status = FlightStatus::LANDING;
    return true;
}

/*
 * koniec letu, musi mat gate lebo chce ukoncit let
 */
bool Flight::disembark() {
    if (status != FlightStatus::LANDING)
        return false;
    if (!hasAssignedGate())
        return false;

    status = FlightStatus::DISEMBARKING;
    return true;
}

/*
 * Let nemoze menit origin, origin a destinacia nemozu byt rovnake a origin musi patrit do mnoziny miest.
 * V tejto mnozine musi byt teda aj Airport.city, pretoze je destinaciou pre prichadzajuce lety.
 */

bool Flight::setOrigin(const std::string &origin) {
    if (!this->origin.empty())
        return false;
    if (origin.empty())
        return false;
    if (connections->isEmpty())
        return false;
    if (origin == destination)
        return false;
    if (!connections->contains(origin))
        return false;

    this->origin = origin;
    return true;
}

/*
 * Let nemoze menit destination, origin a destinacia nemozu byt rovnake a destination musi patrit do mnoziny miest, do ktorych sa lieta
 */
bool Flight::setDestination(const std::string &destination) {
    if (!this->destination.empty())
        return false;
    if (destination.empty())
        return false;
    if (connections->isEmpty())
        return false;
    if (destination == origin)
        return false;
    if (!connections->contains(destination))
        return false;

    this->destination = destination;
    return true;
}

/*
 * Gate sa moze pridelovat az potom ako bol let naplanovany alebo let uz pristal.
 */
//TODO Kontrola gate/runway number sa musi vykonavat v airport.cpp. Asi by bolo dobre nejako upravit.
bool Flight::assignGate(int gateNumber) {
    if (status != FlightStatus::SCHEDULING && status != FlightStatus::LANDING)
        return false;

    this->gateNumber = gateNumber;
    return true;
}

/*
 * Runway sa prideluje letom, ktore uz boarduju alebo su ohlasene na prilet.
 */
bool Flight::assignRunway(int runwayNumber) {
    if (status != FlightStatus::BOARDING && status != FlightStatus::ARRIVING)
        return false;

    this->runwayNumber = runwayNumber;
    return true;
}