#pragma once
#include "main.h"

class Bar {
public:
    pros::Motor motor;
    bool isBack = false;

    // Tested conversion:
    // 1 bar degree = 8 motor degrees
    const double MOTOR_DEGREES_PER_BAR_DEGREE = 7.25;

    Bar(int motorPort);

    double degreesToMotor(double realDegrees);
    void moveToAngle(double realDegrees);

    void moveToFront();
    void moveToBack();
    void toggle();
    void reset();
};