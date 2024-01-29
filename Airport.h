#include <vector>
#include <queue>
#include <map>
#include <memory>
#include <fstream>

#include "Flight.h"
#include "Gate.h"
#include "Runway.h"

#pragma once

class Airport {
    const std::string city;
    std::vector<std::shared_ptr<Flight>> flights;
    std::priority_queue<std::shared_ptr<Flight>> arrivingFlights;
    std::priority_queue<std::shared_ptr<Flight>> departingFlights;
    const size_t maximumNumberOfGates;
    std::vector<std::shared_ptr<Gate>> gates;
    const size_t maximumNumberOfRunways;
    std::vector<std::shared_ptr<Runway>> runways;
    std::shared_ptr<Cities> connections;

    bool hasFlight(int number) const;
    bool hasGate(int gateNumber) const;
    bool hasRunway(int runwayNumber) const;
    bool isArriving(FlightStatus status) const { return status == FlightStatus::ARRIVING || status == FlightStatus::LANDING || status == FlightStatus::DISEMBARKING; };
    bool isDeparting(FlightStatus status) const { return status == FlightStatus::BOARDING || status == FlightStatus::TAKING_OFF || status == FlightStatus::DEPARTING; };
public:
    Airport(const std::string &city, size_t maximumNumberOfGates, size_t maximumNumberOfRunways);

    int manageTraffic();

    size_t loadFlightsFromFile(const std::string &fileName);
    bool saveFlightsToFile(const std::string &fileName);

    void addFlight(int number, FlightStatus status, const std::string &origin, const std::string &destination, int gateNumber = UNASSIGNED, int runwayNumber = UNASSIGNED);
    bool addGate(int gateNumber);
    bool addRunway(int runwayNumber);
    bool addConnection(const std::string &connection);
    size_t addConnections(const std::vector<std::string> &listOfCities);

    size_t countVacantGates() const ;
    size_t countVacantRunways() const ;
    size_t getNumberOfFlights() const { return flights.size(); };
    size_t getNumberOfArrivingFlights() const { return arrivingFlights.size(); };
    size_t getNumberOfDepartingFlights() const { return departingFlights.size(); };
    size_t getNumberOfGates() const { return gates.size(); };
    size_t getNumberOfRunways() const { return runways.size(); };
};