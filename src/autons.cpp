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

    chassis.setPose(0, 0, 350);

    // Open-loop movements (continuous motion)
    chassis.arcade(127, 127);
    pros::delay(700);
    chassis.arcade(-80, -127);
    pros::delay(200);
    chassis.arcade(127, 127);
    pros::delay(500);
    chassis.arcade(-100, 0);
    pros::delay(300);

    chassis.swingToHeading(270, DriveSide::RIGHT, 2000,{
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion while moving
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    chassis.arcade(-60, 0);
    pros::delay(1000);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(600);
    claw.open();
    chassis.arcade(0, 0);
    // finished first pin
    
    //reset pose when on aligner right before leaving
    chassis.setPose(cosf(chassis.getPose().theta) * 7, sinf(chassis.getPose().theta) * 7, chassis.getPose().theta);
    pros::delay(100);
    // radius: 6.5, angle: theta, 

    // chassis.setPose(0, 0, 0);
    // // next movement setting up to pickup first pin cup stack
    chassis.moveToPoint(0, 10, 2000); //-20, -10
    chassis.waitUntilDone();

    // // // Turn
    // chassis.turnToHeading(135, 800);

    // // Start bar movement in the background 
    // pros::Task barTask([&]{
    //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //     bar.motor.move(127);
    //     pros::delay(800);
    //     bar.motor.brake();
    // });

    // chassis.waitUntilDone();

    // chassis.moveToPoint(12, 6, 10000, {.maxSpeed = 20}); //-20, -10
    // chassis.waitUntilDone();
    // // chassis.moveToPose(15.77, -0.6, 34, 1500);
    // // chassis.waitUntilDone();
    // claw.close();
}

// move to pose 5, -6, 400

// -10, 7

// 6, -12, 270