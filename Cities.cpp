#include "Cities.h"

//TODO Kompatibilita pre mesta s medzerami v mene
Cities::Cities(const std::vector<std::string>& listOfCities) {
    for (const std::string& city : listOfCities) {
        for (char letter : city)
            if (!isalpha(letter))
                break;
        this->cities.insert(city);
    }
}

bool Cities::contains(const std::string& city) {
    if (isEmpty())
        return false;

    return cities.find(city) != cities.end();
}

bool Cities::add(const std::string& city) {
    if (city.empty())
        return false;

    for (char letter : city)
        if (!isalpha(letter))
            return false;
    cities.insert(city);
    return true;
}

size_t Cities::add(const std::vector<std::string>& listOfCities) {
    if (listOfCities.empty())
        return 0;

    size_t count = 0;
    for (const std::string& city : listOfCities)
        if (add(city))
            count++;
    return count;
}

void Cities::remove(const std::string& city) {
    if (isEmpty())
        return;

    cities.erase(city);
}

bool Cities::isEmpty() {
    return cities.empty();
}

size_t Cities::size() {
    return cities.size();
}

const std::string Cities::getListOfCities() {
    if (isEmpty())
        return "";

    std::ostringstream cityStream;
    bool firstCity = true;
    for (const std::string& city : cities) {
        if (!firstCity)
            cityStream << ",";
        cityStream << city;
        firstCity = false;
    }
    return cityStream.str();
}
