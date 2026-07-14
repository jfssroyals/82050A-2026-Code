#pragma once
#include "main.h"
#include "lemlib/api.hpp"

class Lift {
    // Stores the target sensor value the lift is currently trying to reach
    double liftTargetHeight;

    // Motor group for lift
    pros::Motor L_liftMotor;

    pros::Motor R_liftMotor;
    
    // PID controller (kP, kI, kD, anti-windup, derivative filter)
    lemlib::PID liftPID{0.5, 0.0, 0.0, 0, false};

public:
    Lift(signed char leftPort, signed char rightPort);

    void setLiftStage(int Stage);
    void updateLiftController(pros::Controller& controller);
    void reset();
};