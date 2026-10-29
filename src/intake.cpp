#include "intake.hpp"
#include <algorithm>

// Constructor: Initializes the motor on the given port
Intake::Intake(int motorPort) 
    : motor(motorPort, pros::MotorGearset::green) {}

void Intake::spinInward(int speed) {
    // if (isSpinningInward && isRunning){
    //     motor.move(0); 
    //     isRunning = false;
    // }
    // else{
        motor.move(std::clamp(speed, 0, 127));
        // isRunning = true;
        // isSpinningInward = true;
    // }
}

void Intake::spinOutward() {
    motor.move(-127);
}

void Intake::stop() {
    motor.move(0);
    // if (!isSpinningInward && isRunning){
    //     motor.move(0); 
    //     isRunning = false;
    // }
    // else {
    //     motor.move(-127); 
    //     isRunning = true;
    //     isSpinningInward = false;
    // }
}
