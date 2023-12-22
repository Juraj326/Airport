// Copyright 2005, Google Inc.
// All rights reserved.

#include <iostream>
#include "gtest/gtest.h"

#include "Flight.h"
#include "Gate.h"
#include "Runway.h"

using namespace ::testing;

TEST(Cities, Konstruktory) {
    Cities city;
    ASSERT_TRUE(city.isEmpty());

    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Prague", "Budapest", "London"};
    Cities cities(listOfCities);
    ASSERT_FALSE(cities.isEmpty());
    ASSERT_EQ(cities.size(), 5);
}

TEST(Cities, NespravnyNazov) {
    Cities cities;
    ASSERT_FALSE(cities.add(""));
    ASSERT_FALSE(cities.add("1"));
    ASSERT_FALSE(cities.add("Bratisla234va"));
    ASSERT_FALSE(cities.add(" "));
    ASSERT_FALSE(cities.add("."));
}

TEST(Cities, NespravneNazvy) {
    Cities cities;
    std::vector<std::string> listOfCities = {"Bratislava", " ", "", ".", "London"};
    cities.add(listOfCities);
    ASSERT_EQ(cities.size(), 2);
}

TEST(Cities, Add) {
    Cities city;
    ASSERT_TRUE(city.add("Bratislava"));
    ASSERT_FALSE(city.isEmpty());
    ASSERT_TRUE(city.contains("Bratislava"));

    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Prague", "Budapest", "London"};
    Cities cities;
    cities.add(listOfCities);
    ASSERT_FALSE(cities.isEmpty());
    ASSERT_EQ(cities.size(), 5);
}

TEST(Cities, SekvenciaAddov) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Prague", "Budapest", "London"};
    Cities cities;
    ASSERT_TRUE(cities.isEmpty());
    ASSERT_TRUE(cities.add("Bratislava"));
    ASSERT_FALSE(cities.isEmpty());
    cities.add(listOfCities);
    ASSERT_EQ(cities.size(), 5);
    ASSERT_TRUE(cities.add("Paris"));
    ASSERT_EQ(cities.size(), 6);
    std::vector<std::string> listOfCities2 = {"Innsbruck", "Brno", "Singapore", "Tokyo", "Sydney"};
    cities.add(listOfCities2);
    ASSERT_EQ(cities.size(), 11);
}

TEST(Cities, Remove) {
    Cities cities;
    cities.remove("Bratislava");
    cities.add("Bratislava");
    cities.add("Vienna");
    cities.add("Budapest");
    ASSERT_EQ(cities.size(), 3);
    cities.remove("Vienna");
    ASSERT_EQ(cities.size(), 2);
    ASSERT_EQ(cities.print(), "Bratislava,Budapest");
}

TEST(Cities, Print) {
    Cities cities;
    ASSERT_EQ(cities.print(), "");
    cities.add("Bratislava");
    ASSERT_EQ(cities.print(), "Bratislava");
    cities.add("Budapest");
    cities.add("Vienna");
    ASSERT_EQ(cities.print(), "Bratislava,Budapest,Vienna");
    cities.remove("Bratislava");
    cities.remove("Budapest");
    cities.remove("Vienna");
    ASSERT_TRUE(cities.isEmpty());
    ASSERT_EQ(cities.print(), "");
}

TEST(Flight, CislaLetov) {
    std::shared_ptr<Cities> connections;

    ASSERT_THROW({Flight flight(-1234, connections);}, std::invalid_argument);
    ASSERT_THROW({Flight flight(10000, connections);}, std::invalid_argument);
    ASSERT_NO_THROW(Flight flight(1234, connections));
    Flight flight(1234, connections);
    ASSERT_EQ(flight.getFlightNumber(), 1234);
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::CREATING);
}

TEST(Flight, PrichadzajuciLet) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, connections);
    ASSERT_TRUE(flight.setOrigin("Bratislava"));
    ASSERT_TRUE(flight.setDestination("Vienna"));
    ASSERT_TRUE(flight.initiateArrival());
    ASSERT_TRUE(flight.assignRunway(11));
    ASSERT_TRUE(flight.land());
    ASSERT_TRUE(flight.assignGate(1));
    ASSERT_TRUE(flight.disembark());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DISEMBARKING);
}

TEST(Flight, OdchadzajuciLet) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, connections);
    ASSERT_TRUE(flight.setOrigin("Bratislava"));
    ASSERT_TRUE(flight.setDestination("Vienna"));
    ASSERT_TRUE(flight.schedule());
    ASSERT_TRUE(flight.assignGate(1));
    ASSERT_TRUE(flight.board());
    ASSERT_TRUE(flight.assignRunway(36));
    ASSERT_TRUE(flight.depart());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DEPARTING);
}

TEST(Flight, NespravneMesta) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, connections);
    ASSERT_FALSE(flight.setOrigin("New York"));
    ASSERT_FALSE(flight.setDestination("New York"));
    ASSERT_FALSE(flight.setOrigin(""));
    ASSERT_FALSE(flight.setOrigin("1234"));
    ASSERT_FALSE(flight.setDestination(""));
    ASSERT_FALSE(flight.setDestination("1234"));
    ASSERT_EQ(flight.getOrigin(), "");
    ASSERT_EQ(flight.getDestination(), "");
}

TEST(Flight, RovnakyOriginDestination) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, connections);
    ASSERT_TRUE(flight.setOrigin("Bratislava"));
    ASSERT_FALSE(flight.setDestination("Bratislava"));
    ASSERT_EQ(flight.getOrigin(), "Bratislava");
    ASSERT_EQ(flight.getDestination(), "");
    ASSERT_TRUE(flight.setDestination("Prague"));
    ASSERT_EQ(flight.getOrigin(), "Bratislava");
    ASSERT_EQ(flight.getDestination(), "Prague");
}

TEST(Flight, ZmenaOriginDestination) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, connections);
    ASSERT_TRUE(flight.setOrigin("Bratislava"));
    ASSERT_TRUE(flight.setDestination("Prague"));
    ASSERT_EQ(flight.getOrigin(), "Bratislava");
    ASSERT_EQ(flight.getDestination(), "Prague");
    ASSERT_FALSE(flight.setOrigin("Paris"));
    ASSERT_FALSE(flight.setDestination("Budapest"));
    ASSERT_EQ(flight.getOrigin(), "Bratislava");
    ASSERT_EQ(flight.getDestination(), "Prague");
}

TEST(Flight, Sekvencia) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, connections);
    ASSERT_FALSE(flight.schedule());
    ASSERT_FALSE(flight.initiateArrival());
    flight.setOrigin("Bratislava");
    flight.setDestination("Prague");
    ASSERT_TRUE(flight.schedule());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::SCHEDULING);
    ASSERT_FALSE(flight.initiateArrival());
    ASSERT_FALSE(flight.depart());
    ASSERT_FALSE(flight.land());
    ASSERT_FALSE(flight.board());
    ASSERT_TRUE(flight.assignGate(1));
    ASSERT_TRUE(flight.board());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::BOARDING);
    ASSERT_FALSE(flight.depart());
    ASSERT_TRUE(flight.assignRunway(36));
    ASSERT_TRUE(flight.depart());
    ASSERT_EQ(flight.getDesignatedGate(), 1);
    ASSERT_EQ(flight.getDesignatedRunway(), 36);
    flight.unassignGate();
    flight.unassignRunway();

    ASSERT_EQ(flight.getOrigin(), "Bratislava");
    ASSERT_EQ(flight.getDestination(), "Prague");
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DEPARTING);
    ASSERT_EQ(flight.getDesignatedGate(), -1);
    ASSERT_EQ(flight.getDesignatedRunway(), -1);
}

TEST(Infrastructure , Konstruktor) {
    ASSERT_THROW({Gate gate(-1);}, std::invalid_argument);
    ASSERT_THROW({Gate gate(37);}, std::invalid_argument);
    ASSERT_NO_THROW(Gate gate(36));
    Gate gate(36);
    ASSERT_EQ(gate.getNumber(), 36);
    ASSERT_TRUE(gate.isVacant());

    ASSERT_THROW({Runway runway(-1);}, std::invalid_argument);
    ASSERT_THROW({Runway runway(37);}, std::invalid_argument);
    ASSERT_NO_THROW(Runway runway(36));
    Runway runway(36);
    ASSERT_EQ(runway.getNumber(), 36);
    ASSERT_TRUE(gate.isVacant());
}

TEST(Gate, VolnyGate) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234, connections);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->initiateArrival();
    flight->land();

    Gate gate(1);
    ASSERT_TRUE(gate.assignFlight(flight));
}

TEST(Gate, ObsadenyGate) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234, connections);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->schedule();

    Gate gate(1);
    gate.assignFlight(flight);

    std::shared_ptr<Flight> flight2 = std::make_shared<Flight>(2, connections);
    flight2->setOrigin("Bratislava");
    flight2->setDestination("Prague");
    ASSERT_FALSE(gate.assignFlight(flight2));
}

TEST(Runway, VolnaRunway) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234, connections);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->initiateArrival();

    Runway runway(36);
    ASSERT_TRUE(runway.assignFlight(flight));
}

TEST(Runway, ObsadenaRunway) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234, connections);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->schedule();
    flight->board();

    Runway runway(36);
    runway.assignFlight(flight);

    std::shared_ptr<Flight> flight2 = std::make_shared<Flight>(2, connections);
    flight2->setOrigin("Bratislava");
    flight2->setDestination("Prague");
    ASSERT_FALSE(runway.assignFlight(flight2));
}