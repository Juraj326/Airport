// Copyright 2005, Google Inc.
// All rights reserved.

#include <iostream>
#include "gtest/gtest.h"

#include "Airport.h"

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
    ASSERT_TRUE(city.add("New York"));
    ASSERT_FALSE(city.add(" New York"));
    ASSERT_FALSE(city.add("New  York"));


    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Prague", "Budapest", "London", "New York", " S hanghai"};
    Cities cities;
    cities.add(listOfCities);
    ASSERT_FALSE(cities.isEmpty());
    ASSERT_EQ(cities.size(), 6);
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
    ASSERT_EQ(cities.getListOfCities(), "Bratislava,Budapest");
}

TEST(Cities, List) {
    Cities cities;
    ASSERT_EQ(cities.getListOfCities(), "");
    cities.add("Bratislava");
    ASSERT_EQ(cities.getListOfCities(), "Bratislava");
    cities.add("Budapest");
    cities.add("Vienna");
    ASSERT_EQ(cities.getListOfCities(), "Bratislava,Budapest,Vienna");
    cities.remove("Bratislava");
    cities.remove("Budapest");
    cities.remove("Vienna");
    ASSERT_TRUE(cities.isEmpty());
    ASSERT_EQ(cities.getListOfCities(), "");
}

TEST(Flight, CislaLetov) {
    std::shared_ptr<Cities> connections;

    ASSERT_THROW({Flight flight(-1234);}, std::invalid_argument);
    ASSERT_THROW({Flight flight(10000);}, std::invalid_argument);
    ASSERT_NO_THROW(Flight flight(1234));
    Flight flight(1234);
    ASSERT_EQ(flight.getFlightNumber(), 1234);
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::SCHEDULING);
}

TEST(Flight, PrichadzajuciLet) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234);
    ASSERT_TRUE(flight.setOrigin("Bratislava"));
    ASSERT_TRUE(flight.setDestination("Vienna"));
    ASSERT_TRUE(flight.initiateArrival());
    ASSERT_TRUE(flight.land());
    ASSERT_TRUE(flight.disembark());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DISEMBARKING);
}

TEST(Flight, OdchadzajuciLet) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234);
    ASSERT_TRUE(flight.setOrigin("Bratislava"));
    ASSERT_TRUE(flight.setDestination("Vienna"));
    ASSERT_TRUE(flight.schedule());
    ASSERT_TRUE(flight.board());
    ASSERT_TRUE(flight.depart());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DEPARTING);
}

TEST(Flight, RovnakyOriginDestination) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234);
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

    Flight flight(1234);
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

    Flight flight(1234);
    ASSERT_FALSE(flight.schedule());
    ASSERT_FALSE(flight.initiateArrival());
    flight.setOrigin("Bratislava");
    flight.setDestination("Prague");
    ASSERT_TRUE(flight.schedule());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::BOARDING);
    ASSERT_FALSE(flight.initiateArrival());
    ASSERT_FALSE(flight.depart());
    ASSERT_FALSE(flight.land());
    ASSERT_TRUE(flight.board());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::TAKING_OFF);
    ASSERT_TRUE(flight.depart());

    ASSERT_EQ(flight.getOrigin(), "Bratislava");
    ASSERT_EQ(flight.getDestination(), "Prague");
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DEPARTING);
}

TEST(Flight, StatusString) {
    Flight flight0(0);
    Flight flight1(1, FlightStatus::BOARDING);
    Flight flight2(2, FlightStatus::TAKING_OFF);
    Flight flight3(3, FlightStatus::DEPARTING);
    Flight flight4(4, FlightStatus::ARRIVING);
    Flight flight5(5, FlightStatus::LANDING);
    Flight flight6(6, FlightStatus::DISEMBARKING);

    ASSERT_EQ(flight0.getFlightStatusString(), "Scheduling");
    ASSERT_EQ(flight1.getFlightStatusString(), "Boarding");
    ASSERT_EQ(flight2.getFlightStatusString(), "Taking off");
    ASSERT_EQ(flight3.getFlightStatusString(), "Departing");
    ASSERT_EQ(flight4.getFlightStatusString(), "Arriving");
    ASSERT_EQ(flight5.getFlightStatusString(), "Landing");
    ASSERT_EQ(flight6.getFlightStatusString(), "Disembarking");
}

TEST(Flight, PorovnaniePrichadzajucich) {
    Flight flight0(0, FlightStatus::BOARDING);
    Flight flight1(1, FlightStatus::TAKING_OFF);
    Flight flight2(2, FlightStatus::DEPARTING);
    ASSERT_TRUE(flight0 < flight1);
    ASSERT_TRUE(flight0 < flight2);
    ASSERT_FALSE(flight1 < flight2);
}

TEST(Flight, PorovnaniePrichadzajucieOdchadzajuce) {
    Flight flight0(0);
    Flight flight1(1, FlightStatus::BOARDING);
    Flight flight2(2, FlightStatus::TAKING_OFF);
    Flight flight3(3, FlightStatus::DEPARTING);
    Flight flight4(4, FlightStatus::ARRIVING);
    Flight flight5(5, FlightStatus::LANDING);
    Flight flight6(6, FlightStatus::DISEMBARKING);

    std::set<Flight> flights = {flight0, flight1, flight2, flight3, flight4, flight5, flight6};
    ASSERT_TRUE(flight0 < flight1);
    ASSERT_TRUE(flight0 < flight2);
    ASSERT_TRUE(flight0 < flight5);
    ASSERT_TRUE(flight0 < flight6);

    ASSERT_FALSE(flight5 < flight0);
    ASSERT_FALSE(flight5 < flight1);
    ASSERT_FALSE(flight5 < flight2);
    ASSERT_FALSE(flight5 < flight3);
    ASSERT_FALSE(flight5 < flight4);
    ASSERT_FALSE(flight5 < flight6);
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
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234);
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
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->schedule();

    Gate gate(1);
    gate.assignFlight(flight);

    std::shared_ptr<Flight> flight2 = std::make_shared<Flight>(2);
    flight2->setOrigin("Bratislava");
    flight2->setDestination("Prague");
    ASSERT_FALSE(gate.assignFlight(flight2));
}

TEST(Runway, VolnaRunway) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->initiateArrival();

    Runway runway(36);
    ASSERT_TRUE(runway.assignFlight(flight));
}

TEST(Runway, ObsadenaRunway) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->schedule();
    flight->board();

    Runway runway(36);
    runway.assignFlight(flight);

    std::shared_ptr<Flight> flight2 = std::make_shared<Flight>(2);
    flight2->setOrigin("Bratislava");
    flight2->setDestination("Prague");
    ASSERT_FALSE(runway.assignFlight(flight2));
}

TEST(Airport, Konstruktor) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_EQ(airport.getNumberOfGates(), 0);
    ASSERT_EQ(airport.getNumberOfRunways(), 0);
    ASSERT_EQ(airport.getNumberOfDepartingFlights(), 0);
    ASSERT_EQ(airport.getNumberOfArrivingFlights(), 0);
    ASSERT_TRUE(airport.addConnection("Prague"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "Prague", "Bratislava"));
    ASSERT_NO_THROW(airport.addFlight(2, FlightStatus::BOARDING, "Bratislava", "Prague"));
}

TEST(Airport, Connections) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Prague"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    std::vector<std::string> cities = {"New York", "Prague", "Vienna", "Budapest"};
    ASSERT_EQ(airport.addConnections(cities), 1);
}

TEST(Airport, AddGateRunway) {
    Airport airport("Bratislava", 5, 2);
    for (int i = 1; i < 6; i++)
        ASSERT_TRUE(airport.addGate(i));
    ASSERT_FALSE(airport.addGate(6));
    ASSERT_FALSE(airport.addGate(2));

    for (int i = 1; i < 3; i++)
        ASSERT_TRUE(airport.addRunway(i));
    ASSERT_FALSE(airport.addRunway(3));
    ASSERT_FALSE(airport.addRunway(1));
}

TEST(Airport, AddFlight) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_THROW(airport.addFlight(1, FlightStatus::SCHEDULING, "Bratislava", "Vienna"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(1, FlightStatus::SCHEDULING, "Vienna", "Bratislava"), std::invalid_argument);

    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));

    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));

    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::SCHEDULING, "Bratislava", "New York"));

    ASSERT_THROW(airport.addFlight(5, FlightStatus::DEPARTING, "Bratislava", "Bratislava"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(5, FlightStatus::DEPARTING, "Vienna", "New York"), std::invalid_argument);

    ASSERT_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "New York", "Bratislava"), std::invalid_argument);
}

TEST(Airport, Load) {
    Airport airport("Bratislava", 2, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addGate(10));
    ASSERT_TRUE(airport.addRunway(0));
    ASSERT_TRUE(airport.addRunway(36));

    ASSERT_EQ(airport.loadFlightsFromFile("Flights1.txt"), 3);
}

TEST(Airport, LoadNeuspesne) {
    Airport airport("Bratislava", 2, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addGate(10));
    ASSERT_TRUE(airport.addRunway(0));
    ASSERT_TRUE(airport.addRunway(36));

    ASSERT_THROW(airport.loadFlightsFromFile("Flights2.txt"), std::invalid_argument);
}

TEST(Airport, Save) {
    Airport airport("Bratislava", 2, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addGate(10));
    ASSERT_TRUE(airport.addRunway(0));
    ASSERT_TRUE(airport.addRunway(36));
    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::SCHEDULING, "Bratislava", "New York"));
    ASSERT_NO_THROW(airport.addFlight(10, FlightStatus::TAKING_OFF, "New York", "Bratislava"));
    ASSERT_NO_THROW(airport.addFlight(2, FlightStatus::BOARDING, "Bratislava", "Vienna"));
    ASSERT_TRUE(airport.saveFlightsToFile("Flights1.txt"));
}

TEST(Airport, Krok) {
    Airport airport("Bratislava", 5, 2);

}

TEST(Airport, ManageTraffic) {
    Airport airport("Bratislava", 5, 2);

}

TEST(Airport, Full) {
    Airport airport("Bratislava", 5, 2);

}