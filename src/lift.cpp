#include "lift.hpp"
#include <algorithm>
#include <cmath>


// Constructor
Lift::Lift(signed char leftPort, signed char rightPort)
    : liftTargetHeight{0},
      L_liftMotor(leftPort, pros::MotorGearset::green),
      R_liftMotor(rightPort, pros::MotorGearset::green)
{
    L_liftMotor.set_brake_mode(pros::MotorBrake::hold);
    R_liftMotor.set_brake_mode(pros::MotorBrake::hold);
}

// void Lift::updateLiftController(pros::Controller& controller) {

//     if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
//         liftTargetHeight += 3;
//         pros::lcd::print(5, "Target: %d", (int)liftTargetHeight);
//     }

//     if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
//         liftTargetHeight -= 3;
//         pros::lcd::print(5, "Target: %d", (int)liftTargetHeight);
//     }


//     // Limit height
//     if(liftTargetHeight > 1000)
//         liftTargetHeight = 1000;

//     if(liftTargetHeight < 0)
//         liftTargetHeight = 0;


//     // Get both motor positions
//     double leftPosition = L_liftMotor.get_position();
//     double rightPosition = R_liftMotor.get_position();


//     // Average position for height control
//     double currentPosition = (leftPosition + rightPosition) / 2;


//     // Height PID
//     double error = liftTargetHeight - currentPosition;

//     double motorPower = liftPID.update(error);


//     // Synchronization correction
//     double difference = leftPosition - rightPosition;

//     double syncCorrection = difference * 0.05;


//     // Apply correction
//     double leftPower = motorPower - syncCorrection;
//     double rightPower = motorPower + syncCorrection;


//     // Limit power
//     leftPower = std::clamp(leftPower, -20.0, 60.0);
//     rightPower = std::clamp(rightPower, -20.0, 60.0);


//     // Move motors separately
//     L_liftMotor.move(leftPower);
//     R_liftMotor.move(rightPower);
// }

// Lift controller
void Lift::updateLiftController(pros::Controller& controller) {

    double currentPosition = L_liftMotor.get_position();

    // Move up
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {

        // Only allow movement if below max height
        if (currentPosition < 1600) {
            L_liftMotor.move(80);
            R_liftMotor.move(80);
        }
        else {
            L_liftMotor.move(0);
            R_liftMotor.move(0);
        }
    }


    // Move down
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {

        // Only allow movement if above minimum height
        if (currentPosition > 0) {
            L_liftMotor.move(-20);
            R_liftMotor.move(-20);
        }
        else {
            L_liftMotor.move(0);
            R_liftMotor.move(0);
        }
    }

    
    // Stop when no button pressed
    else {
        L_liftMotor.move(0);
        R_liftMotor.move(0);
    }

    // // Limit lift range
    // if (liftTargetHeight > 1000) {
    //     liftTargetHeight = 1000;
    // }

    // if (liftTargetHeight < 0) {
    //     liftTargetHeight = 0;
    // }


    


    // Debug information
    pros::lcd::print(5, "Target: %d", (int)liftTargetHeight);
    // pros::lcd::print(6, "Pos: %d", (int)currentPosition);
    // pros::lcd::print(7, "Power: %d", (int)motorPower);
}


// // Placeholder for future preset heights
// void Lift::setLiftStage(int Stage) {

// }