#pragma once
#include "main.h"

class Claw {
public:
    pros::adi::DigitalOut piston;
    bool isExtended = true;
;
    Claw(char port);

    void open();
    void close();
    void toggle();
};