#pragma once
#include "main.h"

class ChainBar {
public:
    pros::Motor motor;
    bool isBack = false;

    ChainBar(int motorPort);

    void moveToFront();
    void moveToBack();
    void toggle();
};