#include "lift.hpp"

// Defining Motor Groups
pros::MotorGroup liftMotors({1, -2}, pros::MotorGearset::red);

// Rotation sensor
pros::Rotation liftRotation(3);


// Defining Stage Heights with constants
// Starts at 3.25 inches and ends at 45.25 inches

const int Stage_Height_1  = 1250;   // 3.25 in
const int Stage_Height_2  = 2490;   // 6.48 in
const int Stage_Height_3  = 3730;   // 9.70 in
const int Stage_Height_4  = 4970;   // 12.93 in
const int Stage_Height_5  = 6210;   // 16.16 in
const int Stage_Height_6  = 7450;   // 19.38 in
const int Stage_Height_7  = 8690;   // 22.61 in
const int Stage_Height_8  = 9930;   // 25.84 in
const int Stage_Height_9  = 11170;  // 29.06 in
const int Stage_Height_10 = 12410;  // 32.29 in
const int Stage_Height_11 = 13650;  // 35.52 in
const int Stage_Height_12 = 14890;  // 38.74 in
const int Stage_Height_13 = 16130;  // 41.97 in
const int Stage_Height_14 = 17404;  // 45.25 in


// Minimum and maximum stage numbers
const int MIN_STAGE = 1;
const int MAX_STAGE = 14;


// Setting up PID for lemlib
// Variable 1: Proportional
// Variable 2: Integral
// Variable 3: Derivative
// Variable 4: Antiwindup
// Variable 5: Sign-flip not needed for lift
lemlib::PID liftPID(0.018, 0.0, 0.08, 0, false);


// Stores the target sensor value the lift is currently trying to reach
int LiftTargetHeight = Stage_Height_1;


// Tracks current stage
int current_stage_number = 1;


// Sets lift stage safely
void setLiftStage(int Stage) {

    if (Stage < MIN_STAGE) {
        Stage = MIN_STAGE;
    }

    if (Stage > MAX_STAGE) {
        Stage = MAX_STAGE;
    }

    current_stage_number = Stage;

    if (Stage == 1) {
        LiftTargetHeight = Stage_Height_1;
    } else if (Stage == 2) {
        LiftTargetHeight = Stage_Height_2;
    } else if (Stage == 3) {
        LiftTargetHeight = Stage_Height_3;
    } else if (Stage == 4) {
        LiftTargetHeight = Stage_Height_4;
    } else if (Stage == 5) {
        LiftTargetHeight = Stage_Height_5;
    } else if (Stage == 6) {
        LiftTargetHeight = Stage_Height_6;
    } else if (Stage == 7) {
        LiftTargetHeight = Stage_Height_7;
    } else if (Stage == 8) {
        LiftTargetHeight = Stage_Height_8;
    } else if (Stage == 9) {
        LiftTargetHeight = Stage_Height_9;
    } else if (Stage == 10) {
        LiftTargetHeight = Stage_Height_10;
    } else if (Stage == 11) {
        LiftTargetHeight = Stage_Height_11;
    } else if (Stage == 12) {
        LiftTargetHeight = Stage_Height_12;
    } else if (Stage == 13) {
        LiftTargetHeight = Stage_Height_13;
    } else if (Stage == 14) {
        LiftTargetHeight = Stage_Height_14;
    }
}


// Background lift control loop
void liftcontrolloop() {
    while (true) {

        double currentPosition = liftRotation.get_position();

        double error = LiftTargetHeight - currentPosition;

        double motorPower = liftPID.update(error);

        liftMotors.move(motorPower);

        pros::delay(20);
    }
}


// Controller control
void updateLiftController(pros::Controller& controller) {

    static int buttonTimer = 0;

    // R2 = move up one stage every 0.4 seconds
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {

        if (pros::millis() - buttonTimer > 400) {

            if (current_stage_number < MAX_STAGE) {
                setLiftStage(current_stage_number + 1);
                controller.rumble(".");
            }

            buttonTimer = pros::millis();
        }
    }

    // R1 = move down one stage every 0.4 seconds
    else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {

        if (pros::millis() - buttonTimer > 400) {

            if (current_stage_number > MIN_STAGE) {
                setLiftStage(current_stage_number - 1);
                controller.rumble(".");
            }

            buttonTimer = pros::millis();
        }
    }
}