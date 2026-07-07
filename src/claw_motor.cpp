#include "../include/claw_motor.hpp"

Bar::Bar(int motorPort)
    : motor(motorPort, pros::MotorGearset::green) {}

double Bar::degreesToMotor(double realDegrees) {
    return realDegrees * MOTOR_DEGREES_PER_BAR_DEGREE;
}     

void Bar::moveToAngle(double realDegrees) {
    motor.move_absolute(degreesToMotor(realDegrees), 80);
}

void Bar::moveToFront() {
    moveToAngle(0);
    isBack = false;
}

void Bar::moveToBack() {
    moveToAngle(180); // 180 * 8 = 1440 motor degrees
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
    motor.tare_position();
}