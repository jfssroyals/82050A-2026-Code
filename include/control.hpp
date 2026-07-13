#pragma once
#include "main.h"
#include "claw.hpp"
#include "claw_motor.hpp"
#include "lift.hpp"

class Control {

private:
    Claw& claw;
    Bar& bar;
    Lift& lift;

    enum ActionState {
        IDLE,
        MOVING_FRONT_PICKUP,
        MOVING_BACK_PICKUP,
        MOVING_FRONT_DROP,
        MOVING_BACK_DROP
    };

    ActionState state;

    enum ClawMode {
        PICKUP,
        DROP
    };

    ClawMode clawMode;


public:

    Control(Claw& claw, Bar& bar, Lift& lift);

    void toggleClawMode();

    void frontSideAction();
    void backSideAction();

    void update();

};