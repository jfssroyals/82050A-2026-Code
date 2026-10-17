#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "autons.hpp"
#include "constants.hpp"
#include "lift.hpp"
#include "claw.hpp"
#include "intake.hpp"

extern Claw claw;
extern Lift lift;

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
     chassis.setPose(0, 0, 0);
    // Open claw
    claw.open();
    pros::delay(2000);
    // Move to goal
    chassis.moveToPoint(0, 8, 300);
    chassis.waitUntilDone();
    // Open claw
    claw.open();
}