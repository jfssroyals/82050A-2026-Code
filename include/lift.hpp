#pragma once

#include "main.h"

class Lift {
private:
    pros::Motor L_liftMotor;
    pros::Motor R_liftMotor;
    pros::Rotation rotationSensor;

    double targetAngle = 0.0;

    bool autoMode = false;

    double kp = 5.69;
    // Allow for sensor noise and the lift settling short under load (degrees).
    double margin = 1.0;

public:
    Lift(
        signed char leftPort,
        signed char rightPort,
        signed char rotationPort
    );

    // Motor control
    void stop();
    void manual(double speed);

    // Sensor
    double getAngle();

    // Automatic movement
    void moveAngle(double angle);
    void update();
    void cancelAuto();

    // Check whether automatic control is active
    bool isAuto();

    // Check whether target angle has been reached
    bool atTarget();

    // Reset / calibration
    void reset();
};
