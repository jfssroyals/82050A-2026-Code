#pragma once
#include "main.h"

class Claw {
public:
    pros::adi::DigitalOut piston;
    bool isExtended = false; // DigitalOut starts false (claw closed).
    Claw(char port);

    void open();
    void close();
};
