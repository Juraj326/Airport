#include "Infrastructure.h"

#pragma once

class Gate : public Infrastructure {
public:
    explicit Gate(int number) : Infrastructure(number) {}

    bool assignFlight(std::shared_ptr<Flight> flight) override;
    void releaseFlight() override;
};
