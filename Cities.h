#include <set>
#include <string>
#include <vector>
#include <sstream>

#pragma once

class Cities {
    std::set<std::string> cities;
public:
    Cities() = default;;
    explicit Cities(const std::string &city);
    explicit Cities(const std::vector<std::string> &listOfCities);

    bool add(const std::string &city);
    size_t add(const std::vector<std::string> &listOfCities);
    void remove(const std::string &city);

    bool contains(const std::string &city) const;
    bool isEmpty() const;
    size_t size() const;
    std::string getListOfCities() const;
private:
    bool isValid(const std::string &city) const;
};