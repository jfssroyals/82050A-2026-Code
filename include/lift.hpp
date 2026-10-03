#pragma once

#include "main.h"

class Lift {
private:
    pros::Motor L_liftMotor;
    pros::Motor R_liftMotor;
    pros::Rotation rotationSensor;

    double targetAngle = 0.0;

    bool autoMode = false;

    double kp = 5.0;
    double margin = 0.5;

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

    // Reset / calibration
    void reset();

    // Check whether automatic move is finished
    bool atTarget();
};