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
extern Lift lift;

void fivePin_red1() {
    
    pros::Task liftTask([&] { // lift.updateComplexLift will run independently every 20 ms
        while (true) {
            lift.updateComplexLift();
            pros::delay(20);
        }
    });

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

    double goalRadius = 6.25;

    // Current robot angle
    double currentTheta = chassis.getPose().theta;

    // Straight alignment angle
    double straightTheta = 270.0;

    // Calculate rotation offset from straight
    double offsetTheta = currentTheta - straightTheta;

    // Normalize angle between -180 and 180
    while (offsetTheta > 180) offsetTheta -= 360;
    while (offsetTheta < -180) offsetTheta += 360;

    // Calculate offset around goal using degrees
    double offsetX = sin(offsetTheta * M_PI / 180.0) * goalRadius;
    double offsetY = (-cos(offsetTheta * M_PI / 180.0) * goalRadius) + goalRadius;

    // Set new coordinate system
    chassis.setPose(offsetX, offsetY, offsetTheta);

    pros::delay(200);
    
    controller.rumble(".");

    // next movement setting up to pickup first pin cup stack
    chassis.moveToPose(0, 9, 0, 1000); //-20, -10
    chassis.waitUntilDone();

    // // Turn
    chassis.turnToHeading(126, 1000);//122

    // Start bar movement in the background 
    pros::Task barTask([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(800);
        bar.motor.brake();
    });

    chassis.waitUntilDone();

    // chassis.moveToPose(15.5, -5.75, 125, 5000, {
    //     .lead = 0.3,
    //     .maxSpeed = 60,
    //     .minSpeed = 30,
    //     .earlyExitRange = 4
    // });
    // chassis.waitUntilDone();
    // chassis.arcade(55, 0);
    // pros::delay(1600);
    
    // chassis.arcade(0, 0);
    chassis.moveToPose(15.5, -5.75, 125, 4000, {.lead = 0.3, .maxSpeed = 30}); //-20, -10
    chassis.waitUntilDone();
    claw.close();
    pros::delay(100);

    

    // turn while lifting lift
    // chassis.turnToHeading(227, 1000, {.maxSpeed = 60});
    // lift.setTargetHeight(400);
    // // Subsystem motion while moving
    // bar.motor.move(-60);
    // pros::delay(100);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // bar.motor.brake();
    // chassis.waitUntilDone();
    // //
    // claw.open();
    // pros::delay(200);
    // // pick second pin cup
    // chassis.turnToHeading(290, 800);
    // chassis.waitUntilDone();

    // chassis.moveToPoint(-4, 3, 800);
    // chassis.waitUntilDone();
    // chassis.swingToHeading(233, DriveSide::LEFT, 2000);
    // chassis.waitUntilDone();
    // chassis.moveToPoint(-18, -9, 4000, {.maxSpeed = 60});
    // chassis.waitUntilDone(); // grabs the pin and cup and lift it up after this
    // chassis.turnToHeading(378, 800);
    // chassis.waitUntilDone();
    // chassis.moveToPoint(-5, 31, 7000);
    // chassis.waitUntilDone();

    // 378, x -5, y 31
    // -3.7, 3.24, 286
    // -8.2, -2.3
    // -15.2, -7.3, 233
    // chassis.moveToPose(-18, -10, 240, 4000, {.lead = 0.8});
    // chassis.waitUntilDone();

    // claw.close();
    
    // 
    // // Start bar movement in the background 
    // bar.motor.move(-60);
    // pros::delay(200);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // bar.motor.brake();
}

void fivePin_red2() {


}

void fivePin_blue1() {

}

void fivePin_blue2() {

}

void skills_auton() {

    pros::Task liftTask([&] { // lift.updateComplexLift will run independently every 20 ms
        while (true) {
            lift.updateComplexLift();
            pros::delay(20);
        }
    });

    pros::delay(2000);

    // 350 -> 10
    chassis.setPose(0, 0, 10);

    // Open-loop movements (continuous motion)
    chassis.arcade(127, -127);
    pros::delay(700);

    chassis.arcade(-80, 127);
    pros::delay(200);

    chassis.arcade(127, -127);
    pros::delay(500);

    chassis.arcade(-100, 0);
    pros::delay(300);

    // RIGHT swing -> LEFT swing
    // 270 -> 90
    chassis.swingToHeading(90, DriveSide::LEFT, 2000, {
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion while moving
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    // Left motor toggle -> Right motor toggle
    chassis.arcade(-60, 0);
    pros::delay(1000);

    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(600);

    claw.open();

    chassis.arcade(0, 0);

    // finished first pin

    // reset pose when on aligner right before leaving

    double goalRadius = 6.25;

    // Current robot angle
    double currentTheta = chassis.getPose().theta;

    // Straight alignment angle
    double straightTheta = 90.0;

    // Calculate rotation offset from straight
    double offsetTheta = currentTheta - straightTheta;

    // Normalize angle between -180 and 180
    while (offsetTheta > 180) offsetTheta -= 360;
    while (offsetTheta < -180) offsetTheta += 360;

    // Calculate offset around goal
    double offsetX = sin(offsetTheta * M_PI / 180.0) * goalRadius;
    double offsetY = (-cos(offsetTheta * M_PI / 180.0) * goalRadius) + goalRadius;

    // Set new coordinate system
    chassis.setPose(offsetX, offsetY, offsetTheta);
    

    // go to the loader for the first time
    chassis.moveToPose(0, 8, 0, 1000);
    chassis.waitUntilDone();

    // // Turn
    chassis.turnToHeading(126, 1000);//122

    // Start bar movement in the background 
    pros::Task barTask([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(800);
        bar.motor.brake();
    });

    chassis.waitUntilDone();

}

void test() {

}

