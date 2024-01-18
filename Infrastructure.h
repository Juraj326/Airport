#include "Flight.h"
#include <memory>
#include <stdexcept>

#pragma once

const int MIN_INFRASTRUCTURE_NUMBER = 0;
const int MAX_INFRASTRUCTURE_NUMBER = 36;

enum class Status {
    OCCUPIED, VACANT
};

class Infrastructure {
protected:
    int number;
    std::shared_ptr<Flight> flight;
    Status status;
protected:

    /**
     * Konštruktor, ktorý vytvorí voľnú infraštruktúru.
     * @param number Identifikačné číslo.
     */
    Infrastructure(int number) : flight(nullptr), status(Status::VACANT) {
        if (number < MIN_INFRASTRUCTURE_NUMBER || number > MAX_INFRASTRUCTURE_NUMBER)
            throw std::invalid_argument("Gate/Runway number must be an integer ranging from 0 to 36.");

        this->number = number;
    };
public:
    virtual int getNumber() { return number; };
    virtual bool assignFlight(std::shared_ptr<Flight> flight) = 0;
    virtual void releaseFlight() = 0;
    virtual bool isVacant() { return status == Status::VACANT; };

    virtual ~Infrastructure() = default;
};