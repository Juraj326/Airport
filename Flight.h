#include "Cities.h"

#include <string>
#include <memory>
#include <stdexcept>

#pragma once

const int MIN_FLIGHT_NUMBER = 0;
const int MAX_FLIGHT_NUMBER = 9999;
const int UNASSIGNED = -1;

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


class Flight {
    int number;
    FlightStatus status;
    std::string origin;
    std::string destination;
    int gateNumber;
    int runwayNumber;
public:
    explicit Flight(int number, FlightStatus status = FlightStatus::SCHEDULING);

    bool schedule();
    bool board();
    bool depart();
    bool initiateArrival();
    bool land();
    bool disembark();

    bool setOrigin(const std::string &origin);
    bool setDestination(const std::string &destination);
    void assignGate(int gateNumber) { this->gateNumber = gateNumber; };
    void assignRunway(int runwayNumber) { this->runwayNumber = runwayNumber; };
    void setStatus(FlightStatus status) { this->status = status; };

    int getFlightNumber() const { return number; };
    FlightStatus getFlightStatus() const { return status; };
    std::string getFlightStatusString() const;
    const std::string &getOrigin() const { return origin; };
    const std::string &getDestination() const { return destination; };
    int getDesignatedGate() const { return gateNumber; };
    int getDesignatedRunway() const { return runwayNumber; };
    bool hasAssignedGate() const { return gateNumber != UNASSIGNED; };
    bool hasAssignedRunway() const { return runwayNumber != UNASSIGNED; };

    bool operator <(const Flight &flight2) const;
};