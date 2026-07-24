#include "../include/claw_motor.hpp"
#include <cmath>

// Creates the bar motor using the green gear cartridge.
Bar::Bar(int motorPort)
    : motor(motorPort, pros::MotorGearset::green) {}


// Converts a real bar angle into motor degrees
// using the mechanism's gear ratio.
double Bar::degreesToMotor(double realDegrees) {
    return realDegrees * MOTOR_DEGREES_PER_BAR_DEGREE;
}


// Moves the bar to a desired angle using the
// motor's built-in position controller.
void Bar::moveToAngle(double realDegrees) {
    motor.move_absolute(degreesToMotor(realDegrees), 60);
}


// Moves the bar to the front hard stop.
// Uses a manual slew so the bar slows down
// before contacting the stop.
void Bar::moveToFront() {

    // Allow the bar to move freely.
    motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    // Fast movement.
    motor.move(70);
    pros::delay(180);

    // Medium speed.
    motor.move(40);
    pros::delay(120);

    // Slow approach.
    motor.move(10);

    int stableTime = 0;
    double lastPosition = motor.get_position();

    // Wait until the encoder stops changing,
    // meaning the bar has reached the hard stop.
    while (stableTime < 100) {

        pros::delay(20);

        double currentPosition = motor.get_position();

        if (std::fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        } else {
            stableTime = 0;
        }

        lastPosition = currentPosition;
    }

    // Stop the motor.
    motor.move(0);

    // Update the software's current bar position.
    isBack = false;
}


// Moves the bar back to the rear hard stop.
// The encoder is re-zeroed every time for accuracy.
void Bar::moveToBack() {

    motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    // Fast movement.
    motor.move(-70);
    pros::delay(180);

    // Medium speed.
    motor.move(-40);
    pros::delay(120);

    // Slow approach.
    motor.move(-10);

    int stableTime = 0;
    double lastPosition = motor.get_position();

    // Wait until the bar has completely stopped.
    while (stableTime < 100) {

        pros::delay(20);

        double currentPosition = motor.get_position();

        if (std::fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        } else {
            stableTime = 0;
        }

        lastPosition = currentPosition;
    }

    // Stop the motor.
    motor.move(0);

    // The back hard stop is our reference position.
    motor.tare_position();

    // Update the software's current bar position.
    isBack = true;
}


// Used during initialize() to home the bar.
// Slowly moves into the back hard stop and
// sets that position as encoder zero.
void Bar::calibrateBack() {

    motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    // Slow calibration movement.
    motor.move(-30);

    int stableTime = 0;
    double lastPosition = motor.get_position();

    // Wait until the bar is no longer moving.
    while (stableTime < 100) {

        pros::delay(20);

        double currentPosition = motor.get_position();

        if (std::fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        } else {
            stableTime = 0;
        }

        lastPosition = currentPosition;
    }

    // Stop the motor.
    motor.move(0);

    // Set the back hard stop as encoder zero.
    motor.tare_position();

    isBack = true;

    // Print debug information.
    pros::lcd::print(7, "Pos: %.2f", motor.get_position());
    pros::lcd::print(6, "Bar Calibrated");
}


// Homes the bar during robot initialization.
void Bar::reset() {
    calibrateBack();
}


// Temporary testing function.
void Bar::cometopickup() {
    motor.move(-270);
}


// Returns true when the bar is at the back
// and the encoder is close to zero.
bool Bar::isAtBack() {

    return isBack &&
           std::fabs(
               motor.get_position() - degreesToMotor(0)
           ) < 20;

}


// Returns true when the bar is at the front
// and the encoder is close to 180 degrees.
bool Bar::isAtFront() {

    return !isBack &&
           std::fabs(
               motor.get_position() - degreesToMotor(180)
           ) < 20;

}