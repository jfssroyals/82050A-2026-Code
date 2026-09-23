#include "intake.hpp"

// Constructor: Initializes the motor on the given port
Intake::Intake(int motorPort) 
    : motor(motorPort, pros::MotorGearset::green) {}

void Intake::spinInward() {
    if (isSpinningInward && isRunning){
        motor.move(0); 
        isRunning = false;
    }
    else{
        motor.move(127); 
        isRunning = true;
        isSpinningInward = true;
    }
}

void Intake::spinOutward() {
    if (!isSpinningInward && isRunning){
        motor.move(0); 
        isRunning = false;
    }
    else {
        motor.move(-127); 
        isRunning = true;
        isSpinningInward = false;
    }
}