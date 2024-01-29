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
    SCHEDULED,
    ARRIVING,
    BOARDED,
    LANDED,
    DEPARTED,
    DISEMBARKED
};

FlightStatus stringToFlightStatus(const std::string &status);
std::string flightStatusToString(FlightStatus status);

class Flight {
    int number;
    FlightStatus status;
    std::string origin;
    std::string destination;

public:
    explicit Flight(int number, FlightStatus status);

    bool board();
    bool take_off();
    bool land();
    bool disembark();

    bool setOrigin(const std::string &origin);
    bool setDestination(const std::string &destination);

    int getFlightNumber() const { return number; };
    FlightStatus getFlightStatus() const { return status; };
    const std::string &getOrigin() const { return origin; };
    const std::string &getDestination() const { return destination; };
};