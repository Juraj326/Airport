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

    ASSERT_THROW({Flight flight(-1234, FlightStatus::SCHEDULED);}, std::invalid_argument);
    ASSERT_THROW({Flight flight(10000, FlightStatus::SCHEDULED);}, std::invalid_argument);
    ASSERT_NO_THROW(Flight flight(1234, FlightStatus::SCHEDULED));
    Flight flight(1234, FlightStatus::SCHEDULED);
    ASSERT_EQ(flight.getFlightNumber(), 1234);
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::SCHEDULED);
}

TEST(Flight, PrichadzajuciLet) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, FlightStatus::ARRIVING);
    ASSERT_TRUE(flight.setOrigin("Vienna"));
    ASSERT_TRUE(flight.setDestination("Bratislava"));
    ASSERT_TRUE(flight.land());
    ASSERT_TRUE(flight.disembark());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DISEMBARKED);
}

TEST(Flight, OdchadzajuciLet) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, FlightStatus::SCHEDULED);
    ASSERT_TRUE(flight.setOrigin("Bratislava"));
    ASSERT_TRUE(flight.setDestination("Vienna"));
    ASSERT_TRUE(flight.board());
    ASSERT_TRUE(flight.take_off());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DEPARTED);
}

TEST(Flight, RovnakyOriginDestination) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);

    Flight flight(1234, FlightStatus::SCHEDULED);
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

    Flight flight(1234, FlightStatus::SCHEDULED);
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

    Flight flight(1234, FlightStatus::SCHEDULED);
    flight.setOrigin("Bratislava");
    flight.setDestination("Prague");
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::SCHEDULED);
    ASSERT_FALSE(flight.take_off());
    ASSERT_FALSE(flight.land());
    ASSERT_TRUE(flight.board());
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::BOARDED);
    ASSERT_TRUE(flight.take_off());

    ASSERT_EQ(flight.getOrigin(), "Bratislava");
    ASSERT_EQ(flight.getDestination(), "Prague");
    ASSERT_EQ(flight.getFlightStatus(), FlightStatus::DEPARTED);
}

TEST(Flight, StatusString) {
    ASSERT_EQ(flightStatusToString(FlightStatus::SCHEDULED), "Scheduled");
    ASSERT_EQ(flightStatusToString(FlightStatus::BOARDED), "Boarded");
    ASSERT_EQ(flightStatusToString(FlightStatus::DEPARTED), "Departed");
    ASSERT_EQ(flightStatusToString(FlightStatus::ARRIVING), "Arriving");
    ASSERT_EQ(flightStatusToString(FlightStatus::LANDED), "Landed");
    ASSERT_EQ(flightStatusToString(FlightStatus::DISEMBARKED), "Disembarked");

    ASSERT_EQ(stringToFlightStatus("Scheduled"), FlightStatus::SCHEDULED);
    ASSERT_EQ(stringToFlightStatus("Boarded"), FlightStatus::BOARDED);
    ASSERT_EQ(stringToFlightStatus("Departed"), FlightStatus::DEPARTED);
    ASSERT_EQ(stringToFlightStatus("Arriving"), FlightStatus::ARRIVING);
    ASSERT_EQ(stringToFlightStatus("Landed"), FlightStatus::LANDED);
    ASSERT_EQ(stringToFlightStatus("Disembarked"), FlightStatus::DISEMBARKED);
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
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234, FlightStatus::SCHEDULED);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->land();

    Gate gate(1);
    ASSERT_TRUE(gate.assignFlight(flight));
}

TEST(Gate, ObsadenyGate) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234, FlightStatus::SCHEDULED);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");

    Gate gate(1);
    gate.assignFlight(flight);

    std::shared_ptr<Flight> flight2 = std::make_shared<Flight>(2, FlightStatus::SCHEDULED);
    flight2->setOrigin("Bratislava");
    flight2->setDestination("Prague");
    ASSERT_FALSE(gate.assignFlight(flight2));
}

TEST(Runway, VolnaRunway) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234, FlightStatus::SCHEDULED);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");

    Runway runway(36);
    ASSERT_TRUE(runway.assignFlight(flight));
}

TEST(Runway, ObsadenaRunway) {
    std::vector<std::string> listOfCities = {"Bratislava", "Vienna", "Budapest", "Prague", "Paris"};
    std::shared_ptr<Cities> connections = std::make_shared<Cities>(listOfCities);
    std::shared_ptr<Flight> flight = std::make_shared<Flight>(1234, FlightStatus::SCHEDULED);
    flight->setOrigin("Bratislava");
    flight->setDestination("Paris");
    flight->board();

    Runway runway(36);
    runway.assignFlight(flight);

    std::shared_ptr<Flight> flight2 = std::make_shared<Flight>(2, FlightStatus::SCHEDULED);
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
    ASSERT_NO_THROW(airport.addFlight(2, FlightStatus::SCHEDULED, "Bratislava", "Prague"));
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
    ASSERT_THROW(airport.addFlight(1, FlightStatus::SCHEDULED, "Bratislava", "Vienna"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "Vienna", "Bratislava"), std::invalid_argument);

    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));

    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));

    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::BOARDED, "Bratislava", "New York"));

    ASSERT_THROW(airport.addFlight(5, FlightStatus::DEPARTED, "Bratislava", "Bratislava"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(5, FlightStatus::DEPARTED, "Vienna", "New York"), std::invalid_argument);

    ASSERT_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "New York", "Bratislava"), std::invalid_argument);

    ASSERT_THROW(airport.addFlight(77, FlightStatus::SCHEDULED, "New York", "Bratislava"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(44, FlightStatus::ARRIVING, "Bratislava", "New York"), std::invalid_argument);

    ASSERT_NO_THROW(airport.addFlight(77, FlightStatus::ARRIVING, "New York", "Bratislava"));
    ASSERT_NO_THROW(airport.addFlight(44, FlightStatus::SCHEDULED, "Bratislava", "New York"));
    ASSERT_NO_THROW(airport.addFlight(11, FlightStatus::LANDED, "New York", "Bratislava"));

    ASSERT_EQ(airport.getNumberOfArrivingFlights(), 2);
    ASSERT_EQ(airport.getNumberOfDepartingFlights(), 2);

}

TEST(Airport, Save) {
    Airport airport("Bratislava", 2, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addGate(10));
    ASSERT_TRUE(airport.addRunway(0));
    ASSERT_TRUE(airport.addRunway(36));
    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::LANDED, "New York", "Bratislava"));
    ASSERT_NO_THROW(airport.addFlight(10, FlightStatus::BOARDED, "Bratislava", "New York"));
    ASSERT_NO_THROW(airport.addFlight(2, FlightStatus::SCHEDULED, "Bratislava", "Vienna"));
    ASSERT_NO_THROW(airport.addFlight(77, FlightStatus::ARRIVING, "New York", "Bratislava"));
    ASSERT_TRUE(airport.saveFlightsToFile("Flights1.txt"));
}

TEST(Airport, Load) {
    Airport airport("Bratislava", 2, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addGate(10));
    ASSERT_TRUE(airport.addRunway(0));
    ASSERT_TRUE(airport.addRunway(36));

    ASSERT_EQ(airport.loadFlightsFromFile("Flights1.txt"), 4);
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

TEST(Airport, ManageFlight) {
    Airport airport("Bratislava", 10, 2);
    for (int i = 0; i < 10; i++)
        ASSERT_TRUE(airport.addGate(i * 3));
    ASSERT_TRUE(airport.addRunway(11));
    ASSERT_TRUE(airport.addRunway(25));
    for (std::string city : {"New York", "Vienna", "Prague", "Budapest", "Shanghai", "San Francisco", "Paris", "Berlin"})
        ASSERT_TRUE(airport.addConnection(city));

}

TEST(Airport, ManageTraffic) {
    Airport airport("Bratislava", 10, 2);
    for (int i = 0; i < 10; i++)
        ASSERT_TRUE(airport.addGate(i * 3));
    ASSERT_TRUE(airport.addRunway(11));
    ASSERT_TRUE(airport.addRunway(25));
    for (std::string city : {"New York", "Vienna", "Prague", "Budapest", "Shanghai", "San Francisco", "Paris", "Berlin"})
        ASSERT_TRUE(airport.addConnection(city));
}