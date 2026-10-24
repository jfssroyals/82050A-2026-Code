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

    // turn to first stack
    chassis.turnToPoint(-4.100, 16.064, 10000, {.maxSpeed = 50});
    chassis.waitUntilDone();

    // Move to pose and check the distance while LemLib drives asynchronously.
    // Keep this whole block commented until ready to test.
    chassis.moveToPoint(-4.100, 16.064, 10000, {
        // .lead = 0.15,
        .maxSpeed = 50
    });
    waitForAutoClaw();
    // Close only once per move; continue to the next auton step when motion ends.
    
    pros::delay(200);
    
    // turn to the goal
    chassis.turnToHeading(90, 10000, {.maxSpeed = 50});
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

    // move backwards
    chassis.moveToPoint(-0.5, 16, 10000, {.forwards = false, .maxSpeed = 50}, false);
    
    // Face the stack from the current pose before driving directly toward it.
    chassis.turnToPoint(16.468, 31.286, 10000, {.maxSpeed = 50}, false);
    // Approach the stack at 40 degrees; a smaller lead reduces the approach curve.
    chassis.moveToPose(16.468, 31.286, 40, 10000, {.lead = 0.25, .maxSpeed = 50});
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


    // return;
}
