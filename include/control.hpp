#pragma once

#include "claw.hpp"
#include "claw_motor.hpp"
#include "lift.hpp"

enum ClawMode {
    PICKUP,
    DROP
};

class Control {
public:
    Control(Claw& claw, Bar& bar, Lift& lift);

    void toggleClawMode();

    void frontSideAction();
    void backSideAction();

private:
    Claw& claw;
    Bar& bar;
    Lift& lift;

    ClawMode clawMode;
    
    void frontSidePickup();
    void backSidePickup();

    void frontSideDrop();
    void backSideDrop();
};