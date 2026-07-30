#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "autons.hpp"
#include "constants.hpp"
#include "lift.hpp"
#include "claw.hpp"
#include "claw_motor.hpp"
#include "intake.hpp"
#include "control.hpp"

extern Bar bar;
extern Claw claw;

void fourPinBlue() {
    pros::delay(2000);
    
    // Seamless transition into odometry tracking
    chassis.setPose(0, 0, 0);
    // chassis.setPose(0, 0, 350);

    // // Open-loop movements (continuous motion)
    // chassis.arcade(127, 127);
    // pros::delay(700);
    // chassis.arcade(-80, -127);
    // pros::delay(200);
    // chassis.arcade(127, 127);
    // pros::delay(500);
    // chassis.arcade(-100, 0);
    // pros::delay(300);

    // chassis.swingToHeading(270, DriveSide::RIGHT, 2000,{
    //     .minSpeed = 127,
    //     .earlyExitRange = 20
    // });

    // // Subsystem motion while moving
    // bar.motor.move(60);
    // pros::delay(200);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // bar.motor.brake();

    // chassis.waitUntilDone();

    // chassis.arcade(-127, 0);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // pros::delay(500);
    // chassis.arcade(0, 0);
    // claw.open();
    // pros::delay(100);

    // // next movement to pickup first pin cup stack
    // chassis.moveToPose(3, -5, 270, 800);
    // chassis.waitUntilDone();
    // // Turn
    // chassis.turnToHeading(30, 800);

    // // Start bar movement in the background 
    // pros::Task barTask([]{
    //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //     bar.motor.move(127);
    //     pros::delay(800);
    //     bar.motor.brake();
    // });

    //
    chassis.moveToPoint(0, 30, 2000);
    chassis.waitUntilDone();
}

// move to pose 5, -6, 400

// 14, 0.5