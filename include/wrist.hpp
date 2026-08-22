#pragma once
#include "main.h"

class Wrist {
private:
    pros::adi::DigitalOut piston;
    bool isUp = false;

public:
    Wrist(char port);

    void up();
    void down();
    void toggle();

    bool getState() const;
};