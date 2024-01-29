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
    SCHEDULING, // ešte neexistujúci let, najnižšia priorita
    BOARDING,   // prvý kontakt s odchádzajúcim letom
    ARRIVING,   // prvý kontakt s prichádzajúcim letom
    DEPARTING,  // koniec odchádzajúceho letu
    DISEMBARKING,   // koniec prichádzajúceho letu
    TAKING_OFF, // let je pripravený odletieť
    LANDING // let je pripravený pristáť, najvyššia priorita
};

FlightStatus stringToFlightStatus(const std::string &status);
std::string flightStatusToString(FlightStatus status);

class Flight {
    int number;
    FlightStatus status;
    std::string origin;
    std::string destination;

public:
    explicit Flight(int number, FlightStatus status = FlightStatus::SCHEDULING);

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

    bool operator <(const Flight &flight2) const;
};