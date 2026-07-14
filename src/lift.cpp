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

void Lift::reset() {

    // Move lift down toward hard stop
    L_liftMotor.move(-40);
    R_liftMotor.move(-40);

    double lastPosition = (L_liftMotor.get_position() + R_liftMotor.get_position()) / 2;
    int stoppedTime = 0;

    while (true) {

        pros::delay(20);

        double currentPosition = (L_liftMotor.get_position() + R_liftMotor.get_position()) / 2;

        // If motor barely moved
        if (fabs(currentPosition - lastPosition) < 0.5) {
            stoppedTime += 20;
        } 
        else {
            stoppedTime = 0;
        }

        // If it has not moved for 1 second
        if (stoppedTime >= 1000) {
            break;
        }

        lastPosition = currentPosition;
    }

    // Stop motors
    L_liftMotor.move(0);
    R_liftMotor.move(0);

    // Set bottom position as zero
    L_liftMotor.tare_position();
    R_liftMotor.tare_position();

    liftTargetHeight = 0;
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
//     leftPower = std::clamp(leftPower, -20.0, 80.0);
//     rightPower = std::clamp(rightPower, -20.0, 80.0);


//     // Move motors separately
//     L_liftMotor.move(leftPower);
//     R_liftMotor.move(rightPower);
// }








// Lift controller
void Lift::updateLiftController(pros::Controller& controller) {

    // Move up
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        // Only allow movement if below max height
        L_liftMotor.move(127);
        R_liftMotor.move(127);
    }


    // Move down
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        L_liftMotor.move(-80);
        R_liftMotor.move(-80);
    }


    // Stop when no button pressed
    else {
        L_liftMotor.move(0);
        R_liftMotor.move(0);
    }
}


// // Placeholder for future preset heights
// void Lift::setLiftStage(int Stage) {

// }