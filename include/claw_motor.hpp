#pragma once
#include "main.h"

class Bar {
public:
    pros::Motor motor;
    bool isBack = false;

    const double MOTOR_DEGREES_PER_BAR_DEGREE = 7.8;

    Bar(int motorPort);

    double degreesToMotor(double realDegrees);
    void moveToAngle(double realDegrees);

    void moveToFront();
    void moveToBack();
    void toggle();
    void reset();

    bool isAtBack();
    bool isAtFront();
};