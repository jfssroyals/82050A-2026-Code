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

    // Unfold and home while the chassis drives to the goal.

    // Move to goal
    chassis.arcade(100, 0);
    // Move wrist up and lift up while driving
    wrist.moveUP();
    lift.getMotorGroup().move_relative(300, 100);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    pros::delay(350);
    // Stop lift and chassis
    lift.stop();
    chassis.arcade(0, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    // Move lift down once at goal
    lift.manual(-100);
    pros::delay(200);
    lift.manual(0);
    // Open claw to score
    claw.open();


    // Move to point (3, 2, 48 degrees)
    chassis.moveToPoint(3, 2, 2000);
    chassis.waitUntilDone();

    // turn to -43.2 degrees
    chassis.turnToHeading(-43.2, 2000);
    chassis.waitUntilDone();

    // // move to pose (-3, 11, -43.58 degrees)
    // chassis.moveToPose(-3, 11, -43.58, 2000);
    // chassis.waitUntilDone();
    
    // pros::delay(200);
    // Open claw
    // chassis.waitUntilDone();
    // kill task

    // Pickup First Stack

    // Score First Stack

    // Pickup Second Stack

    // Score Second Stack
}
