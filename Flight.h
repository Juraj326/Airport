#include "Cities.h"

#include <string>
#include <map>
#include <memory>
#include <stdexcept>

#pragma once

const int MIN_FLIGHT_NUMBER = 0;
const int MAX_FLIGHT_NUMBER = 9999;

/**
 * Symbolizuje fázu letu.
 */
enum class FlightStatus {
    CREATING, // ešte neexistujúci let, najnižšia priorita
    SCHEDULED,   // prvý kontakt s odchádzajúcim letom
    ARRIVING,   // prvý kontakt s prichádzajúcim letom
    DEPARTED,  // koniec odchádzajúceho letu
    DISEMBARKED,   // koniec prichádzajúceho letu
    BOARDED, // let je pripravený odletieť
    LANDED // let je pripravený pristáť, najvyššia priorita
};

FlightStatus stringToFlightStatus(const std::string &status);
std::string flightStatusToString(FlightStatus status);

class Flight {
    int number;
    FlightStatus status;
    std::string origin;
    std::string destination;

public:
    explicit Flight(int number, FlightStatus status = FlightStatus::CREATING);

    bool schedule();
    bool board();
    bool take_off();
    bool initiateArrival();
    bool land();
    bool disembark();

    bool setOrigin(const std::string &origin);
    bool setDestination(const std::string &destination);

    int getFlightNumber() const { return number; };
    FlightStatus getFlightStatus() const { return status; };
    const std::string &getOrigin() const { return origin; };
    const std::string &getDestination() const { return destination; };
};