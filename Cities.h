#include <set>
#include <string>
#include <vector>
#include <sstream>

#pragma once

class Cities {
    std::set<std::string> cities;   // mnozina miest (origin a destinacie)
public:
    Cities() {};
    Cities(const std::vector<std::string>& listOfCities);



    bool contains(const std::string& city);
    bool add(const std::string& city);
    void add(const std::vector<std::string>& listOfCities);
    void remove(const std::string& city);
    bool isEmpty();
    size_t size();
    const std::string print();
};