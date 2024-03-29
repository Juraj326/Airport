#include <vector>
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
    const size_t maximumNumberOfGates;
    std::map<std::shared_ptr<Gate>, std::shared_ptr<Flight>> gates;
    const size_t maximumNumberOfRunways;
    std::map<std::shared_ptr<Runway>, std::shared_ptr<Flight>> runways;
    std::shared_ptr<Cities> connections;
public:
    Airport(const std::string &city, size_t maximumNumberOfGates, size_t maximumNumberOfRunways);

    std::string manageFlight(int flightNumber);
    std::shared_ptr<Gate> findVacantGate() const;
    std::shared_ptr<Runway> findVacantRunway() const;

    size_t loadFlightsFromFile(const std::string &fileName);
    bool saveFlightsToFile(const std::string &fileName) const;

    void addFlight(int flightNumber, FlightStatus status, const std::string &origin, const std::string &destination);
    bool addGate(int gateNumber);
    bool addRunway(int runwayNumber);
    bool addConnection(const std::string &connection);
    size_t addConnections(const std::vector<std::string> &listOfCities);

    FlightStatus getFlightStatus(int flightNumber) { return findFlight(flightNumber)->getFlightStatus(); };

    size_t countVacantGates() const;
    size_t countVacantRunways() const;

    size_t getNumberOfFlights() const { return flights.size(); };
    size_t getNumberOfGates() const { return gates.size(); };
    size_t getNumberOfRunways() const { return runways.size(); };
private:
    std::shared_ptr<Flight> &findFlight(int flightNumber);
    void removeFlight(std::shared_ptr<Flight> &flight);
    bool hasFlight(int flightNumber);
    bool hasGate(int gateNumber) const;
    bool hasRunway(int runwayNumber) const;
    bool isArriving(FlightStatus status) const { return status == FlightStatus::ARRIVING || status == FlightStatus::LANDED || status == FlightStatus::DISEMBARKED; };
    bool isDeparting(FlightStatus status) const { return status == FlightStatus::SCHEDULED || status == FlightStatus::BOARDED || status == FlightStatus::DEPARTED; };
    const std::shared_ptr<Gate> &findAssignedGate(const std::shared_ptr<Flight> &flight) const;
    const std::shared_ptr<Runway> &findAssignedRunway(const std::shared_ptr<Flight> &flight) const;
};