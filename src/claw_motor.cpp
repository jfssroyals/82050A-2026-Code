#include "../include/claw_motor.hpp"

ChainBar::ChainBar(int motorPort)
    : motor(motorPort, pros::MotorGearset::green) {}

double ChainBar::degreesToMotor(double realDegrees) {
    return realDegrees * MOTOR_DEGREES_PER_CHAINBAR_DEGREE;
}

void ChainBar::moveToAngle(double realDegrees) {
    motor.move_absolute(degreesToMotor(realDegrees), 80);
}

void ChainBar::moveToFront() {
    moveToAngle(0);
    isBack = false;
}

void ChainBar::moveToBack() {
    moveToAngle(180); // 180 * 8 = 1440 motor degrees
    isBack = true;
}

void ChainBar::toggle() {
    if (isBack) {
        moveToFront();
    } else {
        moveToBack();
    }
}