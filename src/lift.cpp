#include "lift.hpp"

// constructor
Lift::Lift(
    signed char leftPort, 
    signed char rightPort, 
    double start_height, 
    double gap, 
    int total_stages
    ) 
    : liftTargetHeight{0}, 
    liftMotors{{leftPort, rightPort}, 
    pros::MotorGearset::green},
    
    // ensure the variables and values can be used
    startHeight{start_height},

    stageGap{gap},

    // might switch it for more
    totalStages{total_stages},

    currentStage{0}

    {}

// Controller control - call this inside your main opcontrol while(true) loop
void Lift::updateLiftController(pros::Controller& controller) {

    // R2 = Increase target height (move up)

    // yo change ts later if needed
    // just keep like this for initial prototype
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        setLiftStage(currentStage + 1); // Adjust this to change how fast the target moves
    }
    
    // R1 = Decrease target height (move down)
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        setLiftStage(currentStage - 1); // Adjust this to change how fast the target moves
    }

    // move the lift
    double currentPosition = liftMotors.get_position();
    double error = liftTargetHeight - currentPosition;

    // Calculate PID output and apply it directly
    double motorPower = liftPID.update(error);
    liftMotors.move(motorPower);
}

// Note: when you use this you can change name but it wont intefere with odometry
// under Lift namespace

// sets the stage of the lift

// test this out if it doesn't work, message me on discord, available to fix from 12pm onwards
void Lift::setLiftStage (int newStage)
{
    if (newStage < 0)
    {
        newStage = 0;
    }

    if (newStage >= totalStages)
    {
        newStage = totalStages - 1;
    }

    // keeps track of stage that lift is at
    currentStage = newStage;

    // changes the target height to 
    liftTargetHeight = startHeight + (newStage * stageGap);

}

// we gotta add on more to this