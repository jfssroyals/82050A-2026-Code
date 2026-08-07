#pragma once
#include "main.h"

class Bar {
private:
    void calibrateBack();

public:
    pros::Motor motor;
    bool isBack = false;
    bool back = false;
    
    // FIX: 60 teeth / 12 teeth = 5.0 motor degrees per 1 arm degree
    const double MOTOR_DEGREES_PER_BAR_DEGREE = 5.0; 

    Bar(int motorPort);
    double degreesToMotor(double realDegrees);
    void moveToAngle(double realDegrees);
    void moveToFront();
    void moveToBack();
    void reset();
    void comeToIntake();
    bool isAtBack();
    bool isAtFront();
    void move_press(int power);

};

extern Bar bar;