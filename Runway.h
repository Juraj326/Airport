#include "Infrastructure.h"

#pragma once

class Runway : public Infrastructure {
public:
    Runway() = delete;
    Runway(int number) : Infrastructure(number) {}

    bool assignFlight(std::shared_ptr<Flight> flight) override;
    void releaseFlight() override;
};