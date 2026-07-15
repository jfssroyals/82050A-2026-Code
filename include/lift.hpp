#pragma once
#include "main.h"
#include "lemlib/api.hpp"

class Lift {
private:
    // Motor group for lift
    pros::Motor L_liftMotor;
    pros::Motor R_liftMotor;
    
    // double liftTargetHeight = 0;
    // int currentStage = 0;
    // const int totalStages = 5;
    // const double stageGap = 500.0;


    // PID controller (kP, kI, kD, anti-windup, derivative filter)
    lemlib::PID liftPID{0.5, 0.0, 0.0, 0, false};

public:
    Lift(signed char leftPort, signed char rightPort);

   // void setLiftStage(int Stage);
   // void stepStageUp();
   // void stepStageDown();
   // void updateComplexLift(pros::Controller& controller);
    void updateLiftController(pros::Controller& controller);
    void reset();
};