#include "Airport.h"

/**
 * Konštruktor, ktorý vytvorí letisko.
 * @param city Mesto, v ktorom je letisko.
 * @param maximumNumberOfGates Maximálny možný počet gate-ov.
 * @param maximumNumberOfRunways Maximálny možný počet runway.
 */
Airport::Airport(const std::string &city, size_t maximumNumberOfGates, size_t maximumNumberOfRunways) :
    city(city), maximumNumberOfGates(maximumNumberOfGates), maximumNumberOfRunways(maximumNumberOfRunways) {
    connections = std::make_shared<Cities>(city);
}

/**
 * Metóda, ktorá vytvorí nový let a pridá ho do prislušného Queue.
 * @param flightNumber Číslo letu.
 * @param status Fáza, v ktorej sa let nachádza.
 * @param origin Pôvod letu.
 * @param destination Destinácia letu.
 */
void Airport::addFlight(int flightNumber, FlightStatus status, const std::string &origin, const std::string &destination) {
    if (getNumberOfGates() == 0 || getNumberOfRunways() == 0) {
        std::ostringstream errorMsg;
        errorMsg << "Flight " << flightNumber << " cannot be added because the airport is missing critical infrastructure.";
        throw std::invalid_argument(errorMsg.str());
    }
    if (countVacantGates() == 0 && isDeparting(status)) {
        std::ostringstream errorMsg;
        errorMsg << "Flight " << flightNumber << " cannot be added because all the gates are full.";
        throw std::invalid_argument(errorMsg.str());
    }

    if (hasFlight(flightNumber)) {
        std::ostringstream errorMsg;
        errorMsg << "Flight " << flightNumber << " already exists.";
        throw std::invalid_argument(errorMsg.str());
    }

    if (isArriving(status) && origin == city) {
        std::ostringstream errorMsg;
        errorMsg << "Invalid status, origin for flight " << flightNumber << ".";
        throw std::invalid_argument(errorMsg.str());
    }

    if (isDeparting(status) && destination == city) {
        std::ostringstream errorMsg;
        errorMsg << "Invalid status, destination for flight " << flightNumber << ".";
        throw std::invalid_argument(errorMsg.str());
    }

    if (origin != city && destination != city) {
        std::ostringstream errorMsg;
        errorMsg << "Flight " << flightNumber << " doesn't interact with this airport.";
        throw std::invalid_argument(errorMsg.str());
    }

   if (!connections->contains(origin))   {
       std::ostringstream errorMsg;
       errorMsg << "Unable to set origin for flight " << flightNumber << ". Origin " << origin << " isn't a connection to this airport.";
       throw std::invalid_argument(errorMsg.str());
   }

   if (!connections->contains(destination)) {
       std::ostringstream errorMsg;
       errorMsg << "Unable to set destination for flight " << flightNumber << ". Destination " << destination << " isn't a connection to this airport.";
       throw std::invalid_argument(errorMsg.str());
   }

    std::shared_ptr<Flight> newFlight = std::make_shared<Flight>(flightNumber, status);

    if (!newFlight->setOrigin(origin)) {
        std::ostringstream errorMsg;
        errorMsg << "Unable to set origin for flight " << flightNumber << ".";
        throw std::invalid_argument(errorMsg.str());
    }
    if (!newFlight->setDestination(destination)) {
        std::ostringstream errorMsg;
        errorMsg << "Unable to set destination for flight " << flightNumber << ".";
        throw std::invalid_argument(errorMsg.str());
    }

    flights.emplace_back(newFlight);
}

/**
 * Metóda, ktorá odstráni let zo zoznamu letov.
 * @param flight Let, ktorý sa má odstrániť.
 */
void Airport::removeFlight(std::shared_ptr<Flight> &flight) {
    flights.erase(std::remove(flights.begin(), flights.end(), flight),flights.end());
}


/**
 * Metóda, ktorá rozhoduje, čo sa má stať s letom.
 * @param flight Let, o ktorom sa rozhoduje.
 */
std::string Airport::manageFlight(int flightNumber) {
    std::shared_ptr<Flight> flight = findFlight(flightNumber);
    std::ostringstream flightInfo;
    switch (flight->getFlightStatus()) {
        case FlightStatus::ARRIVING:
            try {
                std::shared_ptr<Runway> vacantRunway = findVacantRunway();
                vacantRunway->assignFlight(flight);
                runways.at(vacantRunway) = flight;
                flight->land();
                flightInfo << "Flight " << flightNumber << " is landing on runway " << vacantRunway->getNumber() << ".\n";

            } catch (std::runtime_error &e) {
                flightInfo << "Flight " << flightNumber << " could not land because there isn't a vacant runway.\n";
                return flightInfo.str();
            }
            break;
        case FlightStatus::LANDED:
            try {
                std::shared_ptr<Gate> vacantGate = findVacantGate();

                std::shared_ptr<Runway> assignedRunway = findAssignedRunway(flight);
                assignedRunway->releaseFlight();
                runways.at(assignedRunway) = nullptr;

                vacantGate->assignFlight(flight);
                gates.at(vacantGate) = flight;
                flight->disembark();
                flightInfo << "Flight " << flightNumber << " is disembarking at gate " << vacantGate->getNumber() << ".\n";

            } catch (std::runtime_error &e) {
                flightInfo << "Flight " << flightNumber << " could not disembark because there isn't a vacant gate.\n";
                return flightInfo.str();
            }
            break;
        case FlightStatus::DISEMBARKED:
            try {
                auto assignedGate = findAssignedGate(flight);
                assignedGate->releaseFlight();
                gates.at(assignedGate) = nullptr;
                removeFlight(flight);
                flightInfo << "Flight " << flightNumber << " has disembarked at gate " << assignedGate->getNumber() << ".\n";
            } catch (std::invalid_argument &e) {
                flightInfo << "Gate not found.\n";
                return flightInfo.str();
            }
            break;
        case FlightStatus::SCHEDULED:
            try {
                std::shared_ptr<Gate> vacantGate = findVacantGate();
                vacantGate->assignFlight(flight);
                gates.at(vacantGate) = flight;
                flight->board();
                flightInfo << "Flight " << flightNumber << " is boarding at gate " << vacantGate->getNumber() << ".\n";
            } catch (std::runtime_error &e) {
                flightInfo << "Flight " << flightNumber << " could not begin boarding because there isn't a vacant gate.\n";
                return flightInfo.str();
            }
            break;
        case FlightStatus::BOARDED:
            try {
                std::shared_ptr<Runway> vacantRunway = findVacantRunway();

                std::shared_ptr<Gate> assignedGate = findAssignedGate(flight);
                assignedGate->releaseFlight();
                gates.at(assignedGate) = nullptr;

                vacantRunway->assignFlight(flight);
                runways.at(vacantRunway) = flight;
                flight->take_off();
                flightInfo << "Flight " << flightNumber << " is taking off from runway " << vacantRunway->getNumber() << ".\n";
            } catch (std::runtime_error &e) {
                flightInfo << "Flight " << flight->getFlightNumber() << " could not take off because there isn't a vacant runway.\n";
                return flightInfo.str();
            }
            break;
        case FlightStatus::DEPARTED:
            try {
                auto assignedRunway = findAssignedRunway(flight);
                assignedRunway->releaseFlight();
                removeFlight(flight);
                flightInfo << "Flight " << flightNumber << " has departed for " << flight->getDestination() << ".\n";
            } catch (std::invalid_argument &e) {
                flightInfo << "Runway not found.\n";
                return flightInfo.str();
            }
            break;
    }
    return flightInfo.str();
}

/**
 * Metóda, ktorá nájde voľný gate.
 * @return Voľný gate.
 */
std::shared_ptr<Gate> Airport::findVacantGate() const {
    if (countVacantGates() == 0)
        throw std::runtime_error("There are no vacant gates");

    auto gate = std::find_if(gates.begin(), gates.end(), [] (const std::pair<std::shared_ptr<Gate>, std::shared_ptr<Flight>> &gate) { return gate.first->isVacant(); });
    return gate->first;
}

/**
 * Metóda, ktorá nájde voľnú runway.
 * @return Voľná runway.
 */
std::shared_ptr<Runway> Airport::findVacantRunway() const {
    if (countVacantRunways() == 0)
        throw std::runtime_error("There are no vacant runways");

    auto runway = std::find_if(runways.begin(), runways.end(), [] (const std::pair<std::shared_ptr<Runway>, std::shared_ptr<Flight>> &runway) { return runway.first->isVacant(); });
    return runway->first;
}

/**
 * Metóda, ktorá načíta všetky lety zo súboru.
 * @param fileName Názov súboru, z ktorého sa majú lety načítať.
 * @return Počet načítaných letov.
 */
size_t Airport::loadFlightsFromFile(const std::string &fileName) {
    std::ifstream inputFile;
    inputFile.open(fileName);
    if (!inputFile.is_open())
        return 0;


    std::string line;
    size_t count = 0;
    while (std::getline(inputFile, line)) {
        std::istringstream flightStream(line);
        std::string number;
        std::string status;
        std::string origin;
        std::string destination;

        std::getline(flightStream, number, ';');
        std::getline(flightStream, status, ';');
        std::getline(flightStream, origin, ';');
        std::getline(flightStream, destination, '\n');

        if (status != "Scheduled" && status != "Arriving") {
            std::ostringstream errorMsg;
            errorMsg << "Unable to load flight " << number << " from file " << fileName << ". Only scheduled and arriving flights can be loaded.";
            throw std::invalid_argument(errorMsg.str());
        }

        try {
            addFlight(std::stoi(number), stringToFlightStatus(status), origin, destination);
        } catch (const std::invalid_argument &e) {
            std::ostringstream errorMsg;
            errorMsg << "Unable to load flight " << number << " from file " << fileName << ".";
            throw std::invalid_argument(errorMsg.str());
        }
        count++;
    }

    inputFile.close();
    return count;
}

/**
 * Metóda, ktorá uloží všetky aktuálne lety do súboru vo formáte "number;status;origin;destination;gateNumber;runwayNumber".
 * @param fileName Názov súboru, do ktorého sa majú lety uložiť.
 * @return True ak boli lety uložené.
 */
bool Airport::saveFlightsToFile(const std::string &fileName) const {
    if (flights.empty())
        return false;

    std::ofstream outputFile;
    outputFile.open(fileName);
    if (!outputFile.is_open())
        return false;

    for (const std::shared_ptr<Flight> &flight : flights) {
            outputFile << flight->getFlightNumber() << ";";
            outputFile << flightStatusToString(flight->getFlightStatus()) << ";";
            outputFile << flight->getOrigin() << ";";
            outputFile << flight->getDestination() << "\n";
    }

    outputFile.close();
    return true;
}

/**
 * Metóda, ktorá pridá gate na letisko.
 * @param gateNumber Číslo gate-u.
 * @return True ak bol gate pridaný.
 */
bool Airport::addGate(int gateNumber) {
    if (getNumberOfGates() == maximumNumberOfGates || hasGate(gateNumber))
        return false;

    std::shared_ptr<Gate> gate = std::make_shared<Gate>(gateNumber);
    gates.insert(std::make_pair( gate, nullptr));
    return true;
}

/**
 * Metóda, ktorá pridá runway na letisko.
 * @param runwayNumber Číslo runway-e.
 * @return True ak bola runway pridaná.
 */
bool Airport::addRunway(int runwayNumber) {
    if (getNumberOfRunways() == maximumNumberOfRunways || hasRunway(runwayNumber))
        return false;

    std::shared_ptr<Runway> runway = std::make_shared<Runway>(runwayNumber);
    runways.insert(std::make_pair(runway, nullptr));
    return true;
}

/**
 * Metóda, ktorá pridá mesto do množiny spojení.
 * @param connection Mesto, ktoré sa má pridať.
 * @return True ak bolo mesto pridané do množiny spojení.
 */
bool Airport::addConnection(const std::string &connection) {
    return connections->add(connection);
}

/**
 * Metóda, ktorá pridá zoznam miest do množiny spojení.
 * @param listOfCities Vector miest, ktoré sa majú pridať.
 * @return Počet pridaných miest.
 */
size_t Airport::addConnections(const std::vector<std::string> &listOfCities) {
    return connections->add(listOfCities);
}

/**
 * Metóda, ktorá spočíta voľné gate-y na letisku.
 * @return Počet voľných gate-ov.
 */
size_t Airport::countVacantGates() const {
    return std::count_if(gates.begin(), gates.end(), [] (const std::pair<std::shared_ptr<Gate>, std::shared_ptr<Flight>> &gate) { return gate.first->isVacant(); });
}

/**
 * Metóda, ktorá spočíta voľné runway-e na letisku.
 * @return Počet voľných runway.
 */
size_t Airport::countVacantRunways() const {
    return std::count_if(runways.begin(), runways.end(), [] (const std::pair<std::shared_ptr<Runway>, std::shared_ptr<Flight>> &runway) { return runway.first->isVacant(); });
}

std::shared_ptr<Flight> &Airport::findFlight(int flightNumber) {
    auto flight = std::find_if(flights.begin(), flights.end(), [flightNumber] (const std::shared_ptr<Flight> &flight) { return flight->getFlightNumber() == flightNumber; });
    if (flight == flights.end()) {
        std::ostringstream errorMsg;
        errorMsg << "Flight " << flightNumber << " not found.";
        throw std::invalid_argument(errorMsg.str());
    }
    return *flight;
}

/**
 * Metóda, ktorá skontroluje či s letiskom interaguje daný let.
 * @param flightNumber Číslo letu.
 * @return True ak s letiskom interaguje let s číslom number.
 */
bool Airport::hasFlight(int flightNumber) {
    auto flight = std::find_if(flights.begin(), flights.end(), [flightNumber] (std::shared_ptr<Flight> &flight) { return flight->getFlightNumber() == flightNumber; });
    return flight != flights.end();
}

/**
 * Metóda, ktorá skontroluje či sa na letisku nachádza gate s príslušným číslom.
 * @param gateNumber Číslo gate-u.
 * @return True ak je na letisku gate s číslom gateNumber.
 */
bool Airport::hasGate(int gateNumber) const {
    auto gate = std::find_if(gates.begin(), gates.end(), [gateNumber] (const std::pair<std::shared_ptr<Gate>, std::shared_ptr<Flight>> &gate) { return gate.first->getNumber() == gateNumber; });
    return gate != gates.end();
}

/**
 * Metóda, ktorá skontroluje či sa na letisku nachádza runway s príslušným číslom.
 * @param runwayNumber Číslo ruwnay-e.
 * @return True ak je na letisku runway s číslom runwayNumber.
 */
bool Airport::hasRunway(int runwayNumber) const {
    auto runway = std::find_if(runways.begin(), runways.end(), [runwayNumber] (const std::pair<std::shared_ptr<Runway>, std::shared_ptr<Flight>> &runway) { return runway.first->getNumber() == runwayNumber; });
    return runway != runways.end();
}

/**
 * Metóda, ktorá vráti gate, ktorému je priradený let.
 * @param flight Let, ktorý je priradený gate-u.
 * @return Hľadaný gate.
 */
const std::shared_ptr<Gate> &Airport::findAssignedGate(const std::shared_ptr<Flight> &flight) const {
    auto gate = std::find_if(gates.begin(), gates.end(), [flight] (const std::pair<std::shared_ptr<Gate>, std::shared_ptr<Flight>> &gate) {return gate.second == flight;});
    if (gate == gates.end())
        throw std::invalid_argument("There's no such flight assigned to a gate.");
    return gate->first;
}

/**
 * Metóda, ktorá vráti runway, ktorej je priradený let.
 * @param flight Let, ktorý je priradený runway-i.
 * @return Hľadaná runway.
 */
const std::shared_ptr<Runway> &Airport::findAssignedRunway(const std::shared_ptr<Flight> &flight) const {
    auto runway = std::find_if(runways.begin(), runways.end(), [flight] (const std::pair<std::shared_ptr<Runway>, std::shared_ptr<Flight>> &runway) {return runway.second == flight;});
    if (runway == runways.end())
        throw std::invalid_argument("There's no such flight assigned to a runway.");
    return runway->first;
}