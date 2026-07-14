#include "intake.hpp"

// Constructor: Initializes the motor on the given port
Intake::Intake(int motorPort) 
    : motor(motorPort, pros::MotorGearset::green) {}

void Intake::spinInward() {
    motor.move(127); 
}

void Intake::spinOutward() {
    motor.move(-127); 
}

void Intake::stop() {
    motor.move(0); 
}

// Handles the toggle logic during driver control
void Intake::update(pros::Controller& controller) {
    // 1. Toggle ON/OFF (Button Y)
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
        isRunning = !isRunning; // Flip the running state
    }

    // 2. Toggle DIRECTION (Button A)
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
        isSpinningInward = !isSpinningInward; // Flip the direction state
    }

    // 3. Apply the states to the physical motor
    if (isRunning) {
        if (isSpinningInward) {
            spinInward();
        } else {
            spinOutward();
        }
    } else {
        stop();
    }
}