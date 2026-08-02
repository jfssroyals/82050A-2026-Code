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


    // Current automatic action
    enum ActionState {
        IDLE,

        // Moving bar to front to grab an object
        MOVING_FRONT_PICKUP,

        // Moving bar to front to release an object
        MOVING_FRONT_DROP,

        // Moving bar to back to release an object
        MOVING_BACK_DROP,

        // For Complex Intake Code
        INTAKE_SEQUENCE
    };


    ActionState state;


public:

    Control(Claw& claw, Bar& bar, Lift& lift);


    // L1: pickup from front
    void frontPickup();




    // L2: drop based on toggle
    void drop();


    // Toggle between front drop and back drop
    void toggleDropSide();


    // Runs every loop and finishes actions
    void update();

    // Intaje Sequence Function
    void startIntakeSequence();

    // intake sequence variables
    int step = 0;
    int stepStartTime = 0;

private:

    // false = back drop
    // true = front drop
    bool frontDrop;

};