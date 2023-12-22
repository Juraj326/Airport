#include "Flight.h"
#include <memory>
#include <stdexcept>

#pragma once

enum class Status {
    OCCUPIED, VACANT
};

class Infrastructure {
protected:
    int number;
    std::shared_ptr<Flight> flight; // let, ktory bude pouzivat danu infrastrukturu
    Status status;  // status infrastruktury
protected:
    Infrastructure() = delete;
    Infrastructure(int number) : flight(nullptr), status(Status::VACANT) {
        if (number < 0 || number > 36)
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