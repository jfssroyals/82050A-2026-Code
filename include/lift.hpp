#pragma once

#include "main.h"

class Wrist;

class Lift {
private:
    pros::MotorGroup liftMotors;

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
    
    pros::Rotation rotationSensor;

    Lift(
        signed char leftPort,
        signed char rightPort,
        signed char rotationPort
    );

    // Motor control
    const pros::MotorGroup& getMotorGroup() const;
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

    // Raises for clearance, then commands the wrist up. Returns true on completion.
    // Does not home or zero any sensor, or verify wrist movement.
    bool unfold(Wrist& wrist);
};
