#pragma once

#include "main.h"

class Wrist;

class Lift {
private:
    pros::Motor L_liftMotor;
    pros::Motor R_liftMotor;
    pros::Rotation rotationSensor;

    double targetAngle = 0.0;
    
    bool autoMode = false;

    //double kp = 5.69;
    double kpUp = 5.69;
    double kpDown = 3.71;  // Starting value to test
    // Minimum outputs can differ because the lift's load differs by direction.
    double minUpOutput = 80.0;
    // Keep enough downward power near the target to overcome static friction.
    double minDownOutput = 60.0;
    // Allow for sensor noise and the lift settling short under load (degrees).
    double margin = 1.0; //7.25;

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

    // Blocking, timed fold sequence. Does not read or zero any sensor.
    void foldInSize(Wrist& wrist);

    // Blocking unfold and home sequence. Returns false on homing timeout.
    // Only zeroes the motor encoders and rotation sensor after detecting a stop.
    bool unfoldAndHome(Wrist& wrist);
};
