#pragma once
#include "main.h"
#include "lemlib/api.hpp"

class Lift {
    // Stores the target sensor value the lift is currently trying to reach
    double liftTargetHeight;
    // motor group for lift
    pros::MotorGroup liftMotors;
    lemlib::PID liftPID{0.018, 0.0, 0.0, 0, false};

    public:
    Lift(signed char leftPort, signed char rightPort);
    // Shared functions
    void setLiftStage(int Stage);
    void updateLiftController(pros::Controller& controller);
};