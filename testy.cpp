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

TEST(Cities, NazvySMedzerami) {
    Cities cities;
    ASSERT_TRUE(cities.add("New York"));
    ASSERT_TRUE(cities.add("Rio de Janeiro"));
    ASSERT_TRUE(cities.add("Nove Mesto nad Vahom"));
    ASSERT_TRUE(cities.add("San Francisco"));
    ASSERT_FALSE(cities.add(" Bratislava"));
    ASSERT_FALSE(cities.add("New  York"));
}

TEST(Cities, NazvySMedzeramiList) {
    Cities cities;
    std::vector<std::string> listOfCities = {"New York", "Rio de Janeiro", "Nove Mesto nad Vahom", "San Francisco", " Bratislava", "New  York", " San  Francisco"};
    ASSERT_EQ(cities.add(listOfCities), 4);
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

TEST(Infrastructure, Konstruktor) {
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
    ASSERT_EQ(airport.getNumberOfFlights(), 0);
    ASSERT_TRUE(airport.addConnection("Prague"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "Prague", "Bratislava"));
    ASSERT_NO_THROW(airport.addFlight(2, FlightStatus::SCHEDULED, "Bratislava", "Prague"));
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

TEST(Airport, Connections) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Prague"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    std::vector<std::string> cities = {"New York", "Prague", "Vienna", "Budapest"};
    ASSERT_EQ(airport.addConnections(cities), 1);
}

TEST(Airport, AddFlight) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_TRUE(airport.addGate(2));
    ASSERT_TRUE(airport.addRunway(2));

    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::SCHEDULED, "Bratislava", "New York"));
    ASSERT_NO_THROW(airport.addFlight(77, FlightStatus::ARRIVING, "New York", "Bratislava"));
    ASSERT_NO_THROW(airport.addFlight(44, FlightStatus::SCHEDULED, "Bratislava", "New York"));
    ASSERT_NO_THROW(airport.addFlight(11, FlightStatus::ARRIVING, "New York", "Bratislava"));

    ASSERT_EQ(airport.getNumberOfFlights(), 4);
}

TEST(Airport, AddFlightBezConnections) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_TRUE(airport.addGate(2));
    ASSERT_TRUE(airport.addRunway(2));

    ASSERT_THROW(airport.addFlight(1, FlightStatus::SCHEDULED, "Bratislava", "Vienna"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "Vienna", "Bratislava"), std::invalid_argument);
}

TEST(Airport, AddFlightBezInfrastruktury) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));

    ASSERT_THROW(airport.addFlight(1, FlightStatus::SCHEDULED, "Bratislava", "Vienna"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "Vienna", "Bratislava"), std::invalid_argument);
}

TEST(Airport, AddFlightDuplikaty) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_TRUE(airport.addGate(2));
    ASSERT_TRUE(airport.addRunway(2));

    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::SCHEDULED, "Bratislava", "New York"));
    ASSERT_NO_THROW(airport.addFlight(77, FlightStatus::ARRIVING, "New York", "Bratislava"));
    ASSERT_NO_THROW(airport.addFlight(44, FlightStatus::SCHEDULED, "Bratislava", "New York"));
    ASSERT_NO_THROW(airport.addFlight(11, FlightStatus::ARRIVING, "New York", "Bratislava"));

    ASSERT_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "Vienna", "Bratislava"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(77, FlightStatus::SCHEDULED, "Vienna", "Bratislava"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(44, FlightStatus::ARRIVING, "Bratislava", "Vienna"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(11, FlightStatus::ARRIVING, "Vienna", "Bratislava"), std::invalid_argument);
}

TEST(Airport, AddFlightZleMesta) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_TRUE(airport.addGate(2));
    ASSERT_TRUE(airport.addRunway(2));

    ASSERT_THROW(airport.addFlight(5, FlightStatus::SCHEDULED, "Bratislava", "Bratislava"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(5, FlightStatus::SCHEDULED, "Vienna", "New York"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(5, FlightStatus::ARRIVING, "San Francisco", "New York"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(5, FlightStatus::ARRIVING, "Bratislava", "Bratislava"), std::invalid_argument);
}

TEST(Airport, AddFlightZlyStatus) {
    Airport airport("Bratislava", 5, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_TRUE(airport.addGate(2));
    ASSERT_TRUE(airport.addRunway(2));

    ASSERT_THROW(airport.addFlight(5, FlightStatus::BOARDED, "Bratislava", "Vienna"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(5, FlightStatus::DISEMBARKED, "Vienna", "Bratislava"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(5, FlightStatus::DEPARTED, "Bratislava", "New York"), std::invalid_argument);
    ASSERT_THROW(airport.addFlight(5, FlightStatus::LANDED, "New York", "Bratislava"), std::invalid_argument);

}

TEST(Airport, Save) {
    Airport airport("Bratislava", 2, 2);
    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addGate(10));
    ASSERT_TRUE(airport.addRunway(0));
    ASSERT_TRUE(airport.addRunway(36));
    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "New York", "Bratislava"));
    ASSERT_NO_THROW(airport.addFlight(10, FlightStatus::SCHEDULED, "Bratislava", "New York"));
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

TEST(Airport, ManageTrafficOutgoing) {
    Airport airport("Bratislava", 3, 2);
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::SCHEDULED, "Bratislava", "Vienna"));

    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.getFlightStatus(1), FlightStatus::BOARDED);

    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.getFlightStatus(1), FlightStatus::DEPARTED);

    ASSERT_NO_THROW(airport.manageFlight(1));

    ASSERT_THROW(airport.manageFlight(1), std::invalid_argument);
}

TEST(Airport, ManageTrafficIncoming) {
    Airport airport("Bratislava", 3, 2);
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "Vienna", "Bratislava"));

    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.getFlightStatus(1), FlightStatus::LANDED);

    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.getFlightStatus(1), FlightStatus::DISEMBARKED);

    ASSERT_NO_THROW(airport.manageFlight(1));

    ASSERT_THROW(airport.manageFlight(1), std::invalid_argument);
}

TEST(Airport, ManageTrafficNoRunways) {
    Airport airport("San Francisco", 3, 2);
    ASSERT_TRUE(airport.addConnection("Berlin"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addGate(2));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::ARRIVING, "Vienna", "San Francisco"));

    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.getFlightStatus(1), FlightStatus::LANDED);
    ASSERT_EQ(airport.countVacantRunways(), 0);

    ASSERT_NO_THROW(airport.addFlight(1234, FlightStatus::SCHEDULED, "San Francisco", "Berlin"));
    ASSERT_NO_THROW(airport.manageFlight(1234));
    ASSERT_EQ(airport.getFlightStatus(1234), FlightStatus::BOARDED);

    ASSERT_EQ(airport.countVacantRunways(), 0);
    ASSERT_EQ(airport.manageFlight(1234), "Flight 1234 could not take off because there isn't a vacant runway.\n");

    ASSERT_TRUE(airport.addRunway(36));
    ASSERT_EQ(airport.manageFlight(1234), "Flight 1234 is taking off from runway 36.\n");
}


TEST(Airport, VacantGatesRunways) {
    Airport airport("Bratislava", 2, 2);
    ASSERT_EQ(airport.countVacantGates(), 0);

    ASSERT_TRUE(airport.addConnection("New York"));
    ASSERT_TRUE(airport.addConnection("Vienna"));
    ASSERT_TRUE(airport.addGate(1));
    ASSERT_TRUE(airport.addRunway(1));
    ASSERT_TRUE(airport.addGate(2));
    ASSERT_TRUE(airport.addRunway(2));
    ASSERT_EQ(airport.countVacantGates(), 2);
    ASSERT_EQ(airport.countVacantRunways(), 2);

    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::SCHEDULED, "Bratislava", "New York"));
    ASSERT_EQ(airport.countVacantGates(), 2);
    ASSERT_EQ(airport.countVacantRunways(), 2);

    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.countVacantGates(), 1);
    ASSERT_EQ(airport.countVacantRunways(), 2);

    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.countVacantGates(), 2);
    ASSERT_EQ(airport.countVacantRunways(), 1);

    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.countVacantGates(), 2);
    ASSERT_EQ(airport.countVacantRunways(), 2);

    ASSERT_NO_THROW(airport.addFlight(1, FlightStatus::SCHEDULED, "Bratislava", "New York"));
    ASSERT_NO_THROW(airport.manageFlight(1));
    ASSERT_EQ(airport.countVacantRunways(), 2);

    ASSERT_NO_THROW(airport.addFlight(44, FlightStatus::ARRIVING, "Vienna", "Bratislava"));
    ASSERT_NO_THROW(airport.manageFlight(44));
    ASSERT_EQ(airport.countVacantRunways(), 1);

    ASSERT_NO_THROW(airport.manageFlight(44));
    ASSERT_EQ(airport.countVacantRunways(), 2);
    ASSERT_EQ(airport.countVacantGates(), 0);
}

TEST(Airport, Everything) {
    Airport airport("Paris", 3, 2);
    for (int i = 0; i < 2; i++)
        ASSERT_TRUE(airport.addGate(i * 10));
    ASSERT_TRUE(airport.addRunway(12));
    ASSERT_TRUE(airport.addRunway(36));
    for (std::string city : {"New York", "Vienna", "Prague", "Budapest", "Shanghai", "San Francisco", "Berlin"})
        ASSERT_TRUE(airport.addConnection(city));

    ASSERT_NO_THROW(airport.addFlight(1111, FlightStatus::SCHEDULED, "Paris", "Vienna"));
    ASSERT_NO_THROW(airport.addFlight(2222, FlightStatus::SCHEDULED, "Paris", "Shanghai"));
    ASSERT_NO_THROW(airport.addFlight(3333, FlightStatus::SCHEDULED, "Paris", "San Francisco"));
    ASSERT_NO_THROW(airport.addFlight(4444, FlightStatus::ARRIVING, "New York", "Paris"));
    ASSERT_NO_THROW(airport.addFlight(5555, FlightStatus::ARRIVING, "Budapest", "Paris"));

    ASSERT_EQ(airport.manageFlight(1111), "Flight 1111 is boarding at gate 0.\n");
    ASSERT_EQ(airport.manageFlight(3333), "Flight 3333 is boarding at gate 10.\n");

    ASSERT_EQ(airport.countVacantGates(), 0);
    ASSERT_EQ(airport.countVacantRunways(), 2);


    ASSERT_EQ(airport.manageFlight(2222), "Flight 2222 could not begin boarding because there isn't a vacant gate.\n");

    ASSERT_EQ(airport.manageFlight(5555), "Flight 5555 is landing on runway 12.\n");
    ASSERT_EQ(airport.manageFlight(4444), "Flight 4444 is landing on runway 36.\n");

    ASSERT_EQ(airport.countVacantGates(), 0);
    ASSERT_EQ(airport.countVacantRunways(), 0);

    ASSERT_EQ(airport.manageFlight(5555), "Flight 5555 could not disembark because there isn't a vacant gate.\n");
    ASSERT_EQ(airport.manageFlight(1111), "Flight 1111 could not take off because there isn't a vacant runway.\n");

    ASSERT_TRUE(airport.addGate(20));

    ASSERT_EQ(airport.countVacantGates(), 1);
    ASSERT_EQ(airport.countVacantRunways(), 0);

    ASSERT_EQ(airport.manageFlight(5555), "Flight 5555 is disembarking at gate 20.\n");

    ASSERT_EQ(airport.countVacantGates(), 0);
    ASSERT_EQ(airport.countVacantRunways(), 1);

    ASSERT_EQ(airport.manageFlight(5555), "Flight 5555 has disembarked at gate 20.\n");
    ASSERT_EQ(airport.countVacantGates(), 1);
    ASSERT_EQ(airport.countVacantRunways(), 1);

    ASSERT_NO_THROW(airport.addFlight(7777, FlightStatus::ARRIVING, "Prague", "Paris"));

    ASSERT_EQ(airport.manageFlight(4444), "Flight 4444 is disembarking at gate 20.\n");
    ASSERT_EQ(airport.countVacantGates(), 0);
    ASSERT_EQ(airport.countVacantRunways(), 2);

    ASSERT_EQ(airport.manageFlight(2222), "Flight 2222 could not begin boarding because there isn't a vacant gate.\n");
    ASSERT_EQ(airport.manageFlight(1111), "Flight 1111 is taking off from runway 12.\n");
    ASSERT_EQ(airport.manageFlight(7777), "Flight 7777 is landing on runway 36.\n");
    ASSERT_EQ(airport.countVacantGates(), 1);
    ASSERT_EQ(airport.countVacantRunways(), 0);

    ASSERT_EQ(airport.manageFlight(4444), "Flight 4444 has disembarked at gate 20.\n");

    ASSERT_EQ(airport.getNumberOfFlights(), 4);

    ASSERT_EQ(airport.manageFlight(7777), "Flight 7777 is disembarking at gate 0.\n");
    ASSERT_EQ(airport.manageFlight(2222), "Flight 2222 is boarding at gate 20.\n");
    ASSERT_EQ(airport.manageFlight(3333), "Flight 3333 is taking off from runway 36.\n");
    ASSERT_EQ(airport.manageFlight(1111), "Flight 1111 has departed for Vienna.\n");


    ASSERT_EQ(airport.countVacantGates(), 1);
    ASSERT_EQ(airport.countVacantRunways(), 1);
}