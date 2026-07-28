#pragma once
#include "main.h"

class Claw {
public:
    pros::adi::DigitalOut piston;
    bool isOpen = false;
;
    Claw(char port);

    void open();
    void close();
    void toggle();
    bool isopen();
};