#include "../include/chainbar.hpp"

ChainBar::ChainBar(int motorPort)
    : motor(motorPort, pros::MotorGearset::green) {}

void ChainBar::moveToFront() {
    motor.move_absolute(0, 80);
    isBack = false;
}

void ChainBar::moveToBack() {
    motor.move_absolute(180, 80);
    isBack = true;
}

void ChainBar::toggle() {
    if (isBack) {
        moveToFront();
    } else {
        moveToBack();
    }
}