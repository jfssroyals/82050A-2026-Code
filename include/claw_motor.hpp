#pragma once
#include "main.h"

class ChainBar {
public:
    pros::Motor motor;
    bool isBack = false;

    // Tested conversion:
    // 1 chainbar degree = 8 motor degrees
    const double MOTOR_DEGREES_PER_CHAINBAR_DEGREE = 7.25;

    ChainBar(int motorPort);

    double degreesToMotor(double realDegrees);
    void moveToAngle(double realDegrees);

    void moveToFront();
    void moveToBack();
    void toggle();
};