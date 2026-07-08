#pragma once
#include "main.h"
#include "lemlib/api.hpp"

class Lift {
    double liftTargetHeight;
    pros::MotorGroup liftMotors;
    lemlib::PID liftPID{0.018, 0.0, 0.0, 0, false};

    double startHeight;
    double stageGap;
    int totalStages;

    int currentStage;

public:
    Lift(
        signed char leftPort,
        signed char rightPort,
        double startHeight,
        double stageGap,
        int totalStages
    );

    void setLiftStage(int stage);
    void updateLiftController(pros::Controller& controller);
};