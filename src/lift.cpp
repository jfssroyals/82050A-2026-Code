#include "lift.hpp"

#include <algorithm>
#include <cmath>


// =====================================================
// CONSTRUCTOR
// =====================================================

Lift::Lift(
    signed char leftPort,
    signed char rightPort,
    signed char rotationPort
)
    : L_liftMotor(leftPort, pros::MotorGearset::green),
      R_liftMotor(rightPort, pros::MotorGearset::green),
      rotationSensor(rotationPort) {

    L_liftMotor.set_brake_mode(pros::MotorBrake::hold);
    R_liftMotor.set_brake_mode(pros::MotorBrake::hold);
}


// =====================================================
// STOP
// =====================================================

void Lift::stop() {

    L_liftMotor.brake();
    R_liftMotor.brake();
}


// =====================================================
// MANUAL CONTROL
// =====================================================

void Lift::manual(double speed) {

    // Manual control cancels any automatic movement
    autoMode = false;

    L_liftMotor.move(speed);
    R_liftMotor.move(speed);
}


// =====================================================
// GET ANGLE
// =====================================================

double Lift::getAngle() {

    return rotationSensor.get_position() / 100.0;
}


// =====================================================
// MOVE TO ANGLE
// =====================================================

void Lift::moveAngle(double angle) {

    targetAngle = angle;

    autoMode = true;
}


// =====================================================
// CHECK TARGET
// =====================================================

bool Lift::atTarget() {
    
    return std::abs(targetAngle - getAngle()) < margin;
}


// =====================================================
// AUTOMATIC UPDATE
// =====================================================

void Lift::update() {

    // Don't do anything if we aren't automatically moving
    if (!autoMode) {
        return;
    }


    double currentAngle = getAngle();

    double error = targetAngle - currentAngle;


    // Reached target
    if (std::abs(error) < margin) {

        stop();

        autoMode = false;

        return;
    }


    // Proportional control
    double speed = kp * error;


    // Limit motor power
    speed = std::clamp(
        speed,
        -127.0,
        127.0
    );


    L_liftMotor.move(speed);
    R_liftMotor.move(speed);
}


// =====================================================
// CANCEL AUTOMATIC MOVEMENT
// =====================================================

void Lift::cancelAuto() {

    autoMode = false;

    stop();
}


// =====================================================
// RESET LIFT
// =====================================================

void Lift::reset() {

    // Allow the lift to move freely into hard stop
    L_liftMotor.set_brake_mode(
        pros::E_MOTOR_BRAKE_COAST
    );

    R_liftMotor.set_brake_mode(
        pros::E_MOTOR_BRAKE_COAST
    );


    // Move downward into physical hard stop
    L_liftMotor.move(-80);
    R_liftMotor.move(-80);


    int stableTime = 0;
    int totalTime = 0;


    double lastPosition =
        (
            L_liftMotor.get_position()
            +
            R_liftMotor.get_position()
        ) / 2.0;


    // Wait until motors stop moving
    while (
        stableTime < 500
        &&
        totalTime < 3000
    ) {

        pros::delay(20);

        totalTime += 20;


        double currentPosition =
            (
                L_liftMotor.get_position()
                +
                R_liftMotor.get_position()
            ) / 2.0;


        if (
            std::fabs(
                currentPosition - lastPosition
            ) < 0.5
        ) {

            stableTime += 20;
        }
        else {

            stableTime = 0;
        }


        lastPosition = currentPosition;
    }


    // Stop motors
    L_liftMotor.move(0);
    R_liftMotor.move(0);


    // Physical bottom is now 0 degrees
    rotationSensor.reset_position();


    targetAngle = 0.0;

    autoMode = false;


    // Restore hold mode
    L_liftMotor.set_brake_mode(
        pros::E_MOTOR_BRAKE_HOLD
    );

    R_liftMotor.set_brake_mode(
        pros::E_MOTOR_BRAKE_HOLD
    );


    stop();
}