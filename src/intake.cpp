#include "intake.hpp"

// Constructor: Initializes the motor on the given port
Intake::Intake(int motorPort) 
    : motor(motorPort, pros::MotorGearset::green) {}

void Intake::spinInward() {
    motor.move(127); 
    isRunning = true;
}

void Intake::spinOutward() {
    motor.move(-127); 
    isRunning = true;
}

void Intake::stop() {
    motor.move(0); 
    isRunning = false;
}

void Intake::toggle_state()
{
    isRunning = !isRunning;    
}

void Intake::toggle_direction()
{
    isSpinningInward = !isSpinningInward;
}  


// Handles the toggle logic during driver control
// void Intake::update() {
//     // 3. Apply the states to the physical motor
//     if (isRunning) {
//         if (isSpinningInward) {
//             spinInward();
//         } else {
//             spinOutward();
//         }
//     } else {
//         stop();
//     }

// }