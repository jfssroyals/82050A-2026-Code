#pragma once
#include "main.h"
#include "lemlib/api.hpp"

class Lift {
private:
    // Motor group for lift
    pros::Motor L_liftMotor;
    pros::Motor R_liftMotor;

    // Rotation sensor for lift position
    pros::Rotation rotationSensor;

    double targetAngle = 0;
    
    double margin = 0.5; // margin of error for lift position

    double kp = 2.5; // Proportional gain for lift control

public:
    Lift(signed char leftPort, signed char rightPort, signed char rotationPort);

    // manual motor control
    void manual(double speed);

    // Tell the lift where we want it to go according to rotation sensor 
    void moveAngle(double angle);

    // Call this repeatedly from opcontrol()
    void update();

    // Get current physical lift angle
    double getAngle();

    // Stop lift motors
    void stop();

    // Zero rotation sensor
    void tare();

    // Mechanical hard-stop calibration
    void reset();
};