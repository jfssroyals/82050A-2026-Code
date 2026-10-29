#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "autons.hpp"
#include "constants.hpp"
#include "lift.hpp"
#include "claw.hpp"
#include "intake.hpp"
#include "wrist.hpp"

extern Claw claw;
extern Lift lift;
extern Wrist wrist;
extern Intake intake;
extern pros::Distance distanceSensor;
// extern pros::Distance distanceSensor; // Uncomment with the auto-close block below.

namespace {
void waitForAutoClaw() {
    bool autoClawTriggered = false;
    while (chassis.isInMotion()) {
        int distance = distanceSensor.get(); // PROS 3.8 distance API; millimeters.

        if (distance > 0 && distance <= 100 && !autoClawTriggered) {
            autoClawTriggered = true;
            claw.close();
            lift.manual(127);
            pros::delay(200);
            lift.stop();
            chassis.cancelMotion();
        }

        pros::delay(5);
    }
}
} // namespace

void align_pose(double straight){
    double goalRadius = 6.5;

    // Current robot angle
    double currentTheta = chassis.getPose().theta;

    // // Straight alignment angle
    // straight = 270.0;

    // Calculate rotation offset from straight
    double offsetTheta = currentTheta - straight;

    // Normalize angle between -180 and 180
    while (offsetTheta > 180) offsetTheta -= 360;
    while (offsetTheta < -180) offsetTheta += 360;

    // Calculate offset around goal using degrees
    double offsetX = sin(offsetTheta * M_PI / 180.0) * goalRadius;
    double offsetY = (-cos(offsetTheta * M_PI / 180.0) * goalRadius) + goalRadius;

    // Set new coordinate system
    chassis.setPose(offsetX, offsetY, offsetTheta);
}

void test() {
    chassis.setPose(0, 0, 0);
    chassis.turnToHeading(90, 1000);
    chassis.waitUntilDone();
}

void autonskills() {
    // set chassis pose
    chassis.setPose(0, 0, 0);

    // toggle twice
    chassis.arcade(0, 100);
    pros::delay(240);
    chassis.arcade(0, 0);

    intake.spinOutward();
    pros::delay(250);
    intake.stop();

    chassis.arcade(0, 100);
    pros::delay(180);
    chassis.arcade(0, 0);
    
    intake.spinOutward();
    pros::delay(200);
    intake.stop();

    pros::delay(200);

    // turn and score first pin
    chassis.turnToHeading(-20, 1000, {}, false);

    chassis.arcade(100, 0);
    // Raise for clearance, then turn the wrist up while driving.
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    lift.unfold(wrist);
    pros::delay(700); // Drive toward the goal for an extra half second.
    // Stop lift and chassis
    lift.stop();
    chassis.arcade(40, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    // Move lift down once at goal
    lift.manual(-100);
    pros::delay(300);
    lift.manual(0);
    pros::delay(200);
    // Open claw to score
    claw.open();
    pros::delay(1000);

    // move forward and bring lift down
    chassis.moveToPose(-5, 8, 0, 1000, {.forwards = false});
    lift.manual(-60);
    pros::delay(100);
    lift.manual(0);
    chassis.waitUntilDone();
    // Back toward the target and finish before returning to driver control.
    // chassis.arcade(0, 0);
    // chassis.moveToPoint(-4, 13, 3000, {.forwards = false}, false);
    
    // picking first stack 
    chassis.turnToPoint(-35.055, 27.595, 10000, {.maxSpeed = 50});
    chassis.waitUntilDone();

    chassis.moveToPoint(-35.055, 27.595, 10000, {.maxSpeed = 50});
    waitForAutoClaw();
    
    
    // Turn to face the goal
    chassis.turnToHeading(72.5, 10000, {.maxSpeed = 50});
    lift.manual(80);
    pros::delay(500);
    lift.manual(0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    chassis.waitUntilDone();
    // Move to goal
    chassis.arcade(100, 0);
    pros::delay(800);
    chassis.arcade(30, 0);
    // drop the pin and cup
    lift.manual(-100);
    pros::delay(200);
    claw.open();
    lift.stop();
    chassis.arcade(0, 0);

    // after scoring go backwards turn and score
    chassis.moveToPoint(-41, 29, 10000, {.forwards = false, .maxSpeed = 50}, false);

    chassis.turnToPoint(-21.854, 48.854, 10000, {.maxSpeed = 50});
    lift.manual(-100);
    pros::delay(800);
    lift.stop();
    chassis.waitUntilDone();

    // Monitor the distance sensor while this move runs asynchronously.
    chassis.moveToPoint(-21.854, 48.854, 10000, {.maxSpeed = 75, .minSpeed = 20});
    waitForAutoClaw();
    
    // Turn to face the goal
    chassis.turnToHeading(160, 10000, {.maxSpeed = 50});
    lift.manual(57);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    chassis.waitUntilDone();
    lift.stop();
    // Move to goal
    chassis.arcade(100, 0);
    pros::delay(800);
    chassis.arcade(30, 0);
    // drop the pin and cup
    lift.manual(-200);
    pros::delay(250);
    claw.open();
    lift.stop();
    chassis.arcade(0, 0);
    // Back toward the next position, then face 270 degrees.
    chassis.moveToPoint(-23, 67, 10000, {.forwards = false, .maxSpeed = 50});
    lift.manual(100);
    pros::delay(100);
    wrist.moveDOWN();
    claw.open();
    lift.lowerUntilTorque(1.0, 2000);
    chassis.waitUntilDone();
    chassis.turnToHeading(270, 10000, {.maxSpeed = 50}, false);
    // Backward at 270 degrees increases X: 10 inches from (-23, 67).
    chassis.moveToPoint(-13, 67, 10000, {.forwards = false, .maxSpeed = 50}, false);
    return;
    
}

void right3pin() {
    // Align robot to goal
    align_pose(270.0);

    // Move to goal
    chassis.moveToPoint(0, 0, 1000);
    chassis.waitUntilDone();

    // Open claw
    claw.open();
}

// coding first auton, in progress
void auton1() {
    // set position to 0,0
    chassis.setPose(0, 0, 51);

    // Score single pin

    // Unfold while the chassis drives to the goal.

    // Move to goal
    chassis.arcade(100, 0);
    // Raise for clearance, then turn the wrist up while driving.
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    lift.unfold(wrist);
    pros::delay(200);
    // Stop lift and chassis
    lift.stop();
    chassis.arcade(40, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    // Move lift down once at goal
    lift.manual(-100);
    pros::delay(300);
    lift.manual(0);
    pros::delay(200);
    // Open claw to score
    claw.open();
    pros::delay(200);

    // Two extra inches behind the original (5.5, 3) target along the 51-degree heading.
    chassis.moveToPoint(3.946, 1.741, 10000, {.forwards = false, .maxSpeed = 50}, false);
    
    // Midpoint between the original first-stack target and the one-inch-right correction.
    chassis.turnToPoint(-2.167, 7.873, 10000, {.maxSpeed = 50});
    chassis.waitUntilDone();


    // Drive to the same target while checking the distance sensor.
    chassis.moveToPoint(-2.167, 7.873, 10000, {
        .maxSpeed = 50
    });
    waitForAutoClaw();
    
    // Close only once per move; continue to the next auton step when motion ends.
    
    pros::delay(200);
    
    // turn to the goal
    chassis.turnToHeading(85, 10000, {.maxSpeed = 50});
    lift.manual(70);
    pros::delay(800);
    lift.manual(0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    chassis.waitUntilDone();
    lift.stop();
    // go into the goal
    chassis.arcade(100, 0);
    pros::delay(800);
    chassis.arcade(0, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    lift.manual(-100);
    pros::delay(200);
    claw.open();
    lift.stop();
    // return;
    // move backwards
    chassis.moveToPoint(-1, 7, 10000, {.forwards = false, .maxSpeed = 50}, false);
    
    // Face the stack from the current pose before driving directly toward it.
    chassis.turnToPoint(13.6, 28.7, 10000, {.maxSpeed = 50}, false);
    // Approach the stack at 40 degrees; a smaller lead reduces the approach curve.
    chassis.moveToPose(13.6, 28.7, 34, 10000, {.lead = 0.25, .maxSpeed = 50});
    waitForAutoClaw();

    // Turn to face the goal
    chassis.turnToHeading(173.82, 10000, {.maxSpeed = 50});
    lift.manual(80);
    pros::delay(900);
    lift.manual(0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    chassis.waitUntilDone();
    // Move to goal
    chassis.arcade(100, 0);
    pros::delay(800);
    chassis.arcade(30, 0);
    // drop the pin and cup
    lift.manual(-100);
    pros::delay(200);
    claw.open();
    lift.stop();

    // Stop pushing the goal before the toggle approach.
    chassis.arcade(0, 0);

    // Assume scoring is 5 inches forward from (13.6, 28.7) at 173.82 degrees.
    // Back up 10 inches from that estimated scoring position.
    chassis.moveToPoint(13.062, 33.671, 10000, {.forwards = false, .maxSpeed = 50}, false);

    // Turn to an absolute heading of 150 degrees.
    chassis.turnToHeading(150, 10000, {.maxSpeed = 50}, false);

    // Drive 35 inches forward at 150 degrees.
    chassis.moveToPoint(30.562, 3.360, 10000, {.maxSpeed = 50}, false);

    // return;
}
