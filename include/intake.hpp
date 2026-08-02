#pragma once
#include "main.h"

class Intake {
private:
    pros::Motor motor;
    
    // State variables to track the toggle behavior
    bool isRunning = false;
    bool isSpinningInward = true; // True = inward, False = outward

public:
    Intake(int motorPort);

    void spinInward();
    void spinOutward();
    void stop();
    
    void update();
    void toggle_state();
    void toggle_direction();
    void toggle_inward();
    void toggle_outward();
};