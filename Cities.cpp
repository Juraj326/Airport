#include "Cities.h"

/**
 * Konštruktor, ktorý vytvorí množinu miest, ktorá obsahuje mesto.
 * @param city Mesto, ktoré má byť v množine.
 */
Cities::Cities(const std::string &city) {
    add(city);
}

/**
 * Konštruktor, ktorý vytvorí množinu miest zo zoznamu.
 * @param listOfCities Vector miest, z ktorých má vytvoriť množinu miest.
 */
Cities::Cities(const std::vector<std::string>& listOfCities) {
    for (const std::string& city : listOfCities) {
        add(city);
    }
}

/**
 * Metóda, ktorá overuje, či je mesto platné.
 * @param city Mesto na overenie.
 * @return True ak sa mesto skladá iba z písmen a medzier.
 */
bool Cities::isValid(const std::string &city) const {
    if (city.empty())
        return false;

    char previousLetter = '0';
    for (char letter : city) {
        if (!isalpha(letter) && !isspace(letter))
            return false;
        if (isspace(letter) && isspace(previousLetter))
            return false;
        if (isspace(letter) && previousLetter == '0') // city začína medzerou
            return false;
        previousLetter = letter;
    }
    return true;
}

/**
 * Metóda, ktorá pridá mesto do možiny miest.
 * @param city Mesto, ktoré sa má pridať.
 * @return True ak mesto je správneho formátu.
 */
bool Cities::add(const std::string& city) {
    if (city.empty())
        return false;
    if (!isValid(city))
        return false;

    cities.insert(city);
    return true;
}

/**
 * Metóda, ktorá pridá mestá do množiny miest.
 * @param listOfCities Vector miest, ktoré sa majú pridať.
 * @return Počet pridaných miest.
 */
size_t Cities::add(const std::vector<std::string>& listOfCities) {
    if (listOfCities.empty())
        return 0;

    size_t count = 0;
    for (const std::string& city : listOfCities)
        if (!contains(city))
            if (add(city))
                count++;
    return count;
}

/**
 * Metóda, ktorá odstráni mesto z množiny miest.
 * @param city Mesto, ktoré sa má odstrániť.
 */
void Cities::remove(const std::string& city) {
    if (isEmpty() || city.empty())
        return;

    cities.erase(city);
}

/**
 * Metóda, ktorá overuje, či mesto patrí do množiny miest.
 * @param city Mesto na overenie.
 * @return True ak množina miest obsahuje city.
 */
bool Cities::contains(const std::string& city) const {
    if (isEmpty())
        return false;

    return cities.find(city) != cities.end();
}

/**
 * Metóda, ktorá overuje, či je množina miest prázdna.
 * @return True ak je množina miest prázdna.
 */
bool Cities::isEmpty() const {
    return cities.empty();
}

/**
 * Metóda, ktorá vracia veľkosť množiny miest.
 * @return Veľkosť množiny miest.
 */
size_t Cities::size() const {
    return cities.size();
}

/**
 * Metóda, ktorá vypíše všetky mestá z množiny miest.
 * @return Obsah množiny miest v tvare "Mesto1,Mesto2,...".
 */
std::string Cities::getListOfCities() const {
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
