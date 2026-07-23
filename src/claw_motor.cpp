#include "../include/claw_motor.hpp"
#include <cmath>

Bar::Bar(int motorPort)
    : motor(motorPort, pros::MotorGearset::green) {}

double Bar::degreesToMotor(double realDegrees) {
    return realDegrees * MOTOR_DEGREES_PER_BAR_DEGREE;
}

void Bar::moveToAngle(double realDegrees) {
    motor.move_absolute(degreesToMotor(realDegrees), 60);
}

void Bar::moveToFront() {
    // Hold the bar at the front position
    motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    // Use your existing angle function
    moveToAngle(180);

    isBack = false;
}

void Bar::moveToBack() {
    // Every time the bar returns to the hard stop,
    // recalibrate the motor position
    calibrateBack();
}

void Bar::calibrateBack() {
    // Let the bar fall toward the hard stop
    motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    motor.move(-60);

    int stableTime = 0;
    double lastPosition = motor.get_position();

    while (stableTime < 300) {
        pros::delay(20);

        double currentPosition = motor.get_position();

        // The bar is no longer moving
        if (std::fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        } else {
            stableTime = 0;
        }

        lastPosition = currentPosition;
    }

    // The bar is now resting on the back hard stop
    motor.move(0);
    motor.tare_position();

    isBack = true;

    pros::lcd::print(7, "Pos: %.2f", motor.get_position());
    pros::lcd::print(6, "Bar Calibrated");
}

void Bar::toggle() {
    if (isBack) {
        moveToFront();
    } else {
        moveToBack();
    }
}

void Bar::reset() {
    // Initial calibration during initialize()
    calibrateBack();
}

void Bar::cometopickup(){
    motor.move(-270);
}

bool Bar::isAtBack() {
    return isBack &&
           std::fabs(
               motor.get_position() - degreesToMotor(0)
           ) < 20;
}

bool Bar::isAtFront() {
    return !isBack &&
           std::fabs(
               motor.get_position() - degreesToMotor(180)
           ) < 20;
}