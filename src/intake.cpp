#include "intake.hpp"

// Constructor
Intake::Intake(int motorPort)
    : motor(motorPort, pros::MotorGearset::green) {}

void Intake::spinInward() {
    motor.move(127);
    isRunning = true;
    isSpinningInward = true;
}

void Intake::spinOutward() {
    motor.move(-127);
    isRunning = true;
    isSpinningInward = false;
}

void Intake::stop() {
    motor.move(0);
    isRunning = false;
}

// Same button toggles ON / OFF
void Intake::toggle_state() {
    if (isRunning) {
        stop();
    } else {
        spinInward();
    }
}

void Intake::toggle_direction() {
    isSpinningInward = !isSpinningInward;

    // If intake is already running, immediately change direction
    if (isRunning) {
        if (isSpinningInward) {
            motor.move(127);
        } else {
            motor.move(-127);
        }
    }
}