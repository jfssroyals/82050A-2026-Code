#include "lift.hpp"
#include <algorithm>
#include <cmath>

extern pros::Controller controller;

// Constructor
Lift::Lift(signed char leftPort, signed char rightPort)
    :  L_liftMotor(leftPort, pros::MotorGearset::green),
       R_liftMotor(rightPort, pros::MotorGearset::green)
{
    L_liftMotor.set_brake_mode(pros::MotorBrake::hold);
    R_liftMotor.set_brake_mode(pros::MotorBrake::hold);
}

// ACTUAL CODE _________________ SIMPLE ---------
void Lift::New_LiftControl(double speed) {
    L_liftMotor.move(speed);
    R_liftMotor.move(speed);
}
// moe stuff now -------------------------

void Lift::reset() {

    // Let lift fall toward the hard stop
    L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    L_liftMotor.move(-80);
    R_liftMotor.move(-80);

    int stableTime = 0;

    double lastPosition = 
        (L_liftMotor.get_position() + R_liftMotor.get_position()) / 2;

    while (stableTime < 500) {

        pros::delay(20);

        double currentPosition =
            (L_liftMotor.get_position() + R_liftMotor.get_position()) / 2;

        // Lift is no longer moving
        if (std::fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        } 
        else {
            stableTime = 0;
        }

        lastPosition = currentPosition;
    }

    // Stop motors
    L_liftMotor.move(0);
    R_liftMotor.move(0);

    // Set hard stop as zero
    L_liftMotor.tare_position();
    R_liftMotor.tare_position();

    isUp = false;

    // Return to hold mode after calibration
    L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}


// ---------------UPDATE CODE -------------------------------------------------------
// void Lift::updateComplexLift() {
    
//     // Prevent going past limits
//     liftTargetHeight = std::clamp(liftTargetHeight, 0.0, 2000.0);

//     // Get current lift position
//     double leftPosition = L_liftMotor.get_position();
//     double rightPosition = R_liftMotor.get_position();

//     double currentPosition = (leftPosition + rightPosition) / 2.0;

//     // PID to target
//     double error = liftTargetHeight - currentPosition;

//     if (abs(error) < 3){
//         L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
//         R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
//         L_liftMotor.brake();
//         R_liftMotor.brake();
//         // L_liftMotor.move_voltage(0);
//         // R_liftMotor.move_voltage(0);
//         return;
//     }
    
//     double motorPower = liftPID.update(error);


//     // Limit power
//     motorPower = std::clamp(motorPower, -70.0, 127.0);


//     L_liftMotor.move(motorPower);
//     R_liftMotor.move(motorPower);

//     if (currentPosition > 400)
//     {
//         isUp  = true;
//     }

//     if (currentPosition < 400)
//     {
//         isUp = false;
//     }
    
//     // if (isUp){
//     //     // L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
//     //     // R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
//     //     L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
//     //     R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
//     // } 
//     // if (isUp == false) {
//     //     L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
//     //     R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
//     // }
// }
// -----------------------------------------------------------------------------
