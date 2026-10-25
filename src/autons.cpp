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
    chassis.setPose(0, 0, 0);
    chassis.arcade(0, 100);
    pros::delay(240);
    chassis.arcade(0, 0);

    // Match two Y presses: 235 ms outward per press.
    intake.spinOutward();
    pros::delay(275);
    intake.stop();

    chassis.arcade(0, 100);
    pros::delay(240);
    chassis.arcade(0, 0);
    
    intake.spinOutward();
    pros::delay(250);
    intake.stop();

    pros::delay(200);
    chassis.turnToHeading(-20, 1000, {}, false);

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
