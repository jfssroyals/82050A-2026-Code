#include "lift.hpp"

// constructor
Lift::Lift(signed char leftPort, signed char rightPort) : liftTargetHeight{0}, liftMotors{{leftPort, rightPort}, pros::MotorGearset::green} {}

// Controller control - call this inside your main opcontrol while(true) loop
void Lift::updateLiftController(pros::Controller& controller) {

    // R2 = Increase target height (move up)
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        liftTargetHeight += 50; // Adjust this to change how fast the target moves
    }
    
    // R1 = Decrease target height (move down)
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        liftTargetHeight -= 50; // Adjust this to change how fast the target moves
    }

    // move the lift
    double currentPosition = liftMotors.get_position();
    double error = liftTargetHeight - currentPosition;

    // Calculate PID output and apply it directly
    double motorPower = liftPID.update(error);
    liftMotors.move(motorPower);
}