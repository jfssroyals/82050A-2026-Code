#include "lift.hpp"
#include <algorithm>
#include <cmath>


// Constructor
Lift::Lift(signed char leftPort, signed char rightPort)
    :  L_liftMotor(leftPort, pros::MotorGearset::green),
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
        } else {
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

    // liftTargetHeight = 0;
    // currentStage = 0;
}

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

        // 2. Toggle DIRECTION (Button A)
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
        L_liftMotor.move(80);
        R_liftMotor.move(80);
    }


    // Stop when no button pressed
    else {
        L_liftMotor.move(0);
        R_liftMotor.move(0);
    } 
}


// === COMPLEX LIFT FUNCTIONS (Commented Out / Prepared) ===

/*
void Lift::setLiftStage(int stage) {
    // Clamp the target stage between 0 and the max stages index
    currentStage = std::clamp(stage, 0, totalStages - 1);
    liftTargetHeight = currentStage * stageGap;
}

void Lift::stepStageUp() {
    setLiftStage(currentStage + 1);
}

void Lift::stepStageDown() {
    setLiftStage(currentStage - 1);
}

void Lift::updateComplexLift(pros::Controller& controller) {
    // Stage controls (Press R2 to step up a stage, R1 to step down a stage)
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
        stepStageUp();
    } 
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) {
        stepStageDown();
    }

    // Average current position of both lift motors
    double leftPosition = L_liftMotor.get_position();
    double rightPosition = R_liftMotor.get_position();
    double currentPosition = (leftPosition + rightPosition) / 2.0;

    // Calculate PID output based on height error
    double error = liftTargetHeight - currentPosition;
    double motorPower = liftPID.update(error);

    // Auto-synchronize sides to keep the lift level
    double difference = leftPosition - rightPosition;
    double syncCorrection = difference * 0.05;

    double leftPower = motorPower - syncCorrection;
    double rightPower = motorPower + syncCorrection;

    // Apply motor power safely
    L_liftMotor.move(std::clamp(leftPower, -127.0, 127.0));
    R_liftMotor.move(std::clamp(rightPower, -127.0, 127.0));
}
*/