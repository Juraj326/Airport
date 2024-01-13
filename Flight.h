#include "Cities.h"

#include <string>
#include <memory>
#include <stdexcept>

#pragma once

const int MIN_FLIGHT_NUMBER = 0;
const int MAX_FLIGHT_NUMBER = 9999;

enum class FlightStatus {
    CREATING,
    SCHEDULING,
    BOARDING,
    DEPARTING,
    ARRIVING,
    LANDING,
    DISEMBARKING
};


class Flight {
    int number; // cislo letu
    FlightStatus status;    // stav letu
    std::string origin; // povod letu
    std::string destination;    // destinacia letu
    int gateNumber; // cislo prideleneho gate-u, -1 ak nema
    int runwayNumber;   // cislo pridelenej runway-e, -1 ak nema
    std::shared_ptr<Cities> connections;    // mnozina miest kam moze letiet
public:
    Flight() = delete;
    Flight(int number, std::shared_ptr<Cities> connections);

    bool schedule();
    bool board();
    bool depart();
    bool initiateArrival();
    bool land();
    bool disembark();

    bool setOrigin(const std::string &origin);
    bool setDestination(const std::string &destination);
    bool assignGate(int gateNumber);
    bool assignRunway(int runwayNumber);
    void unassignGate() { gateNumber = -1; };
    void unassignRunway() { runwayNumber = -1; };

    int getFlightNumber() const { return number; };
    FlightStatus getFlightStatus() const { return status; };
    const std::string &getOrigin() const { return origin; };
    const std::string &getDestination() const { return destination; };
    int getDesignatedGate() const { return gateNumber; };
    int getDesignatedRunway() const { return runwayNumber; };
    bool hasAssignedGate() const { return gateNumber != -1; };
    bool hasAssignedRunway() const { return runwayNumber != -1; };
};