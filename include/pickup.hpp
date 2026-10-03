#pragma once

#include "main.h"

#include "lift.hpp"
#include "intake.hpp"
#include "claw.hpp"
#include "wrist.hpp"


class PickupSystem {

private:

    Lift& lift;

    Intake& intake;

    Claw& claw;

    Wrist& wrist;

    pros::Distance& distanceSensor;


    // Lift positions
    static constexpr double LIFT_UP_ANGLE = 15.0;

    static constexpr double LIFT_DOWN_ANGLE = 12.0;


    // Distance sensor threshold
    static constexpr int DISTANCE_THRESHOLD = 80;


    // Delay before closing claw
    static constexpr int CLAW_CLOSE_DELAY = 300;


    enum class State {

        OFF,

        MOVING_UP,

        WAITING_FOR_OBJECT,

        MOVING_DOWN,

        WAITING_TO_CLOSE,

        MOVING_BACK_UP,

        WAITING_FOR_CLEAR
    };


    State state = State::OFF;


    uint32_t clawCloseTimer = 0;


public:

    PickupSystem(
        Lift& lift,
        Intake& intake,
        Claw& claw,
        Wrist& wrist,
        pros::Distance& distanceSensor
    );


    // Start automatic pickup
    void start();


    // Stop automatic pickup
    void stop();


    // Call every loop
    void update();


    bool isRunning();
};