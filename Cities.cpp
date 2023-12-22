#include "Cities.h"


/*
 * Konstruktor, ktory naplni vstupnu mnozinu mestami, nekompatibilne ignoruje
 */
//TODO Kompatibilita pre mesta s medzerami v mene
Cities::Cities(const std::vector<std::string>& listOfCities) {
    for (const std::string& city : listOfCities) {
        for (char c : city)
            if (!isalpha(c))
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

    for (char c : city)
        if (!isalpha(c))
            return false;
    cities.insert(city);
    return true;
}

/*
 * Prida zoznam miest do mnoziny, nekompatibilne ignoruje
 */
void Cities::add(const std::vector<std::string>& listOfCities) {
    if (listOfCities.empty())
        return;

    for (const std::string& city : listOfCities)
        add(city);
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
const std::string Cities::print() {
    if (isEmpty())
        return "";

    std::ostringstream os;
    std::string del = ",";
    bool first = true;
    for (const std::string& city : cities) {
        if (!first)
            os << del;
        os << city;
        first = false;
    }
    return os.str();
}
