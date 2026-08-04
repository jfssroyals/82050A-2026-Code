#pragma once
#include "main.h"

class Intake {
private:
    pros::Motor motor;
    
    // State variables to track the toggle behavior

public:
    Intake(int motorPort);
    bool isRunning = false;
    bool isSpinningInward = true;

    void spinInward();
    void spinOutward();
    void stop();
    
    void update();
    void toggle_state();
    void toggle_direction();
    void toggle_inward();
    void toggle_outward();
};