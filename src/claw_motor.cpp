#include "../include/claw_motor.hpp"
#include <cmath>

Bar::Bar(int motorPort)
    : motor(motorPort, pros::MotorGearset::green) {}

double Bar::degreesToMotor(double realDegrees) {
    return realDegrees * MOTOR_DEGREES_PER_BAR_DEGREE;
}     

void Bar::moveToAngle(double realDegrees) {
    motor.move_absolute(degreesToMotor(realDegrees), 80);
}

void Bar::moveToFront() {
    moveToAngle(180);
    isBack = false;
}

void Bar::moveToBack() {
    moveToAngle(0); // 180 * 8 = 1440 motor degrees
    isBack = true;
}

void Bar::toggle() {
    if (isBack) {
        moveToFront();
    } else {
        moveToBack();
    }
}

void Bar::reset() {

    motor.move(-40);

    int stableTime = 0;
    double lastPosition = motor.get_position();

    while (stableTime < 500) {

        pros::delay(20);

        double currentPosition = motor.get_position();

        // Motor stopped moving
        if (fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        } 
        else {
            stableTime = 0;
        }

        lastPosition = currentPosition;
    }

    motor.move(0);

    // Set hard stop as zero
    motor.tare_position();

    isBack = true;

    pros::lcd::print(7, "Pos: %.2f", motor.get_position());
    pros::lcd::print(6, "Bar Calibrated");
}

bool Bar::isAtBack() {
    return fabs(motor.get_position() - degreesToMotor(0)) < 20;
}

bool Bar::isAtFront() {
    return fabs(motor.get_position() - degreesToMotor(180)) < 20;
}