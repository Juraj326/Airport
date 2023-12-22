#include <vector>
#include <memory>

#include "Flight.h"
#include "Gate.h"
#include "Runway.h"

#pragma once

class Airport {
    std::string city;
    std::vector<Flight> flights;
    std::vector<Gate> gates;
    std::vector<Runway> runways;
    std::shared_ptr<Cities> cities;
public:
};