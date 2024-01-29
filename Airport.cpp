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

    // Nie je najšťastnejšie riešenie ale nič lepšie mi nenapadlo.
    for (FlightStatus status : {FlightStatus::SCHEDULED, FlightStatus::BOARDED, FlightStatus::DEPARTED,
                                FlightStatus::ARRIVING, FlightStatus::LANDED, FlightStatus::DISEMBARKED})
        flights[status];
}

/**
 * Metóda, ktorá vytvorí nový let a pridá ho do prislušného Queue.
 * @param number Číslo letu.
 * @param status Fáza, v ktorej sa let nachádza.
 * @param origin Pôvod letu.
 * @param destination Destinácia letu.
 */
void Airport::addFlight(int number, FlightStatus status, const std::string &origin, const std::string &destination) {
    if (countVacantGates() == 0 && isDeparting(status)) {
        std::ostringstream errorMsg;
        errorMsg << "Flight " << number << " cannot be added because all the gates are full.";
        throw std::invalid_argument(errorMsg.str());
    }

    if (hasFlight(number)) {
        std::ostringstream errorMsg;
        errorMsg << "Flight " << number << " already exists.";
        throw std::invalid_argument(errorMsg.str());
    }

    if (isArriving(status) && origin == city) {
        std::ostringstream errorMsg;
        errorMsg << "Invalid status, origin for flight " << number << ".";
        throw std::invalid_argument(errorMsg.str());
    }

    if (isDeparting(status) && destination == city) {
        std::ostringstream errorMsg;
        errorMsg << "Invalid status, destination for flight " << number << ".";
        throw std::invalid_argument(errorMsg.str());
    }

    if (origin != city && destination != city) {
        std::ostringstream errorMsg;
        errorMsg << "Flight " << number << " doesn't interact with this airport.";
        throw std::invalid_argument(errorMsg.str());
    }

   if (!connections->contains(origin))   {
       std::ostringstream errorMsg;
       errorMsg << "Unable to set origin for flight " << number << ". Origin " << origin << " isn't a connection to this airport.";
       throw std::invalid_argument(errorMsg.str());
   }

   if (!connections->contains(destination)) {
       std::ostringstream errorMsg;
       errorMsg << "Unable to set destination for flight " << number << ". Destination " << destination << " isn't a connection to this airport.";
       throw std::invalid_argument(errorMsg.str());
   }

    std::shared_ptr<Flight> newFlight = std::make_shared<Flight>(number, status);

    if (!newFlight->setOrigin(origin)) {
        std::ostringstream errorMsg;
        errorMsg << "Unable to set origin for flight " << number << ".";
        throw std::invalid_argument(errorMsg.str());
    }
    if (!newFlight->setDestination(destination)) {
        std::ostringstream errorMsg;
        errorMsg << "Unable to set destination for flight " << number << ".";
        throw std::invalid_argument(errorMsg.str());
    }

    flights.at(status).push(newFlight);
    manageFlight(newFlight);
}

/**
 * Metóda, ktorá rozhoduje, čo sa má stať s letom.
 * @param flight Let, o ktorom sa rozhoduje.
 */
void Airport::manageFlight(const std::shared_ptr<Flight> &flight) {
    switch (flight->getFlightStatus()) {
        case FlightStatus::ARRIVING:
            break; //TODO assignRunway(Flight), flight.land()
        case FlightStatus::LANDED:
            break; //TODO unassignRunway(), assignGate(Flight), flight.disembark()
        case FlightStatus::DISEMBARKED:
            break; //TODO unassignGate(), remove(flight)
        case FlightStatus::SCHEDULED:
            break; //TODO assignGate(), flight.board()
        case FlightStatus::BOARDED:
            break; //TODO unassignGate(), assignRunway(Flight), flight.take_off()
        case FlightStatus::DEPARTED:
            break; //TODO unassignRunway() remove(flight)
    }
}

/**
 * Metóda, ktorá priraďuje runway-e letom podľa viacerých faktorov:
 * 1. Počet voľných gate-ov (limit 80%)
 * 2. Počet prichádzajúcich letov a počet odchádzajúcich letov
 * 3. Pristávajúce lety majú vyššiu prioritu ako odlietajúce.
 * 4. Poradie letov podľa priority.
 */
void Airport::manageTraffic() {
    std::ostringstream trafficChanges;
    if (100 * countVacantGates() / getNumberOfGates() < 20) {
        return;
    }
    if (getNumberOfDepartingFlights() > getNumberOfArrivingFlights()) {
        return;
    }

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
    //TODO pridat metodu, ktora priradi gates a runways
    return count;
}

/**
 * Metóda, ktorá uloží všetky aktuálne lety do súboru vo formáte "number;status;origin;destination;gateNumber;runwayNumber".
 * @param fileName Názov súboru, do ktorého sa majú lety uložiť.
 * @return True ak boli lety uložené.
 */
bool Airport::saveFlightsToFile(const std::string &fileName) {
    if (flights.empty())
        return false;

    std::ofstream outputFile;
    outputFile.open(fileName);
    if (!outputFile.is_open())
        return false;

    for (auto &statusQueue : flights) {
        std::queue<std::shared_ptr<Flight>> tmpQueue;
        while (!statusQueue.second.empty()) {
            std::shared_ptr<Flight> flight = statusQueue.second.front();
            statusQueue.second.pop();
            outputFile << flight->getFlightNumber() << ";";
            outputFile << flightStatusToString(flight->getFlightStatus()) << ";";
            outputFile << flight->getOrigin() << ";";
            outputFile << flight->getDestination() << "\n";

            tmpQueue.push(flight);
        }
        statusQueue.second = tmpQueue;
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
    gates.emplace_back(gate);
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

    std::shared_ptr<Runway> runway =  std::make_shared<Runway>(runwayNumber);
    runways.emplace_back(runway);
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
    return std::count_if(gates.begin(), gates.end(), [] (const std::shared_ptr<Gate> &gate) { return gate->isVacant(); });
}

/**
 * Metóda, ktorá spočíta voľné runway-e na letisku.
 * @return Počet voľných runway.
 */
size_t Airport::countVacantRunways() const {
    return std::count_if(runways.begin(), runways.end(), [] (const std::shared_ptr<Runway> &runway) { return runway->isVacant(); });
}

/**
 * Metóda, ktorá skontroluje či s letiskom interaguje daný let.
 * @param number Číslo letu.
 * @return True ak s letiskom interaguje let s číslom number.
 */
bool Airport::hasFlight(int number) {
    for (auto &statusQueue : flights) {
        std::queue<std::shared_ptr<Flight>> tmpQueue;
        while (!statusQueue.second.empty()) {
            std::shared_ptr<Flight> flight = statusQueue.second.front();
            statusQueue.second.pop();

            if (flight->getFlightNumber() == number) {
                tmpQueue.push(flight);
                statusQueue.second = tmpQueue;
                return true;
            }

            tmpQueue.push(flight);
        }
        statusQueue.second = tmpQueue;
    }
    return false;
}

/**
 * Metóda, ktorá skontroluje či sa na letisku nachádza gate s príslušným číslom.
 * @param gateNumber Číslo gate-u.
 * @return True ak je na letisku gate s číslom gateNumber.
 */
bool Airport::hasGate(int gateNumber) const {
    auto gate = std::find_if(gates.begin(), gates.end(), [gateNumber] (const std::shared_ptr<Gate> &gate) { return gate->getNumber() == gateNumber; });
    return gate != gates.end();
}

/**
 * Metóda, ktorá skontroluje či sa na letisku nachádza runway s príslušným číslom.
 * @param runwayNumber Číslo ruwnay-e.
 * @return True ak je na letisku runway s číslom runwayNumber.
 */
bool Airport::hasRunway(int runwayNumber) const {
    auto runway = std::find_if(runways.begin(), runways.end(), [runwayNumber] (const std::shared_ptr<Runway> &runway) { return runway->getNumber() == runwayNumber; });
    return runway != runways.end();
}

/**
 * Metóda, ktorá vráti počet letov, ktoré majú letisko ako destináciu
 * @return Počet letov.
 */
size_t Airport::getNumberOfArrivingFlights() const {
    size_t count = 0;
    count += flights.at(FlightStatus::ARRIVING).size();
    count += flights.at(FlightStatus::LANDED).size();
    return count;
}

/**
 Metóda, ktorá vráti počet letov, ktoré majú letisko ako pôvod
 * @return Počet letov.
 */
size_t Airport::getNumberOfDepartingFlights() const {
    size_t count = 0;
    count += flights.at(FlightStatus::SCHEDULED).size();
    count += flights.at(FlightStatus::BOARDED).size();
    return count;
}
