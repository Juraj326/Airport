#include "Cities.h"


/*
 * Konstruktor, ktory naplni vstupnu mnozinu mestami, nekompatibilne ignoruje
 */
//TODO Kompatibilita pre mesta s medzerami v mene
Cities::Cities(const std::vector<std::string>& listOfCities) {
    for (const std::string& city : listOfCities) {
        for (char letter : city)
            if (!isalpha(letter))
                break;
        this->cities.insert(city);
    }
}

/*
 * Zistuje vyskyt mesta city v mnozine
 */
bool Cities::contains(const std::string& city) {
    if (isEmpty())
        return false;

    return cities.find(city) != cities.end();
}

/*
 * Prida mesto do mnoziny, ak uz v nej je, tak ignoruje
 */
bool Cities::add(const std::string& city) {
    if (city.empty())
        return false;

    for (char letter : city)
        if (!isalpha(letter))
            return false;
    cities.insert(city);
    return true;
}

/*
 * Prida zoznam miest do mnoziny, nekompatibilne ignoruje
 */
size_t Cities::add(const std::vector<std::string>& listOfCities) {
    if (listOfCities.empty())
        return 0;

    size_t count = 0;
    for (const std::string& city : listOfCities)
        if (add(city))
            count++;
    return count;
}

/*
 * odstrani mesto z mnoziny
 */
void Cities::remove(const std::string& city) {
    if (isEmpty())
        return;

    cities.erase(city);
}

/*
 * vrati ci je mnozina prazdna
 */
bool Cities::isEmpty() {
    return cities.empty();
}

size_t Cities::size() {
    return cities.size();
}

/*
 * Vrati string v tvare mesto1,mesto2,...,mestoN
 */
std::string Cities::getListOfCities() {
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
