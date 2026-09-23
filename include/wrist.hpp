#pragma once
#include "main.h"
class Wrist {
public:
    pros::adi::DigitalOut piston;
    Wrist(char port);
    bool isUp = true;

    void moveUP();
    void moveDOWN();

};
