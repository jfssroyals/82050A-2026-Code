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

void score_back (){
    bar.motor.move(-90);
    pros::delay(600);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    bar.motor.brake();
    pros::delay(350);
    claw.open();
    pros::delay(100);
}

void grab_up() {
    // Give claw more time to secure grip
    claw.close();
    pros::delay(400);

    bar.motor.move(-60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();
}

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

void fourPin_red1() {
    // set first chassis pose with a slight offset to the right
    chassis.setPose(0, 0, 350);

    // Toggle Action: Speed up if needed
    chassis.arcade(127, 127);
    pros::delay(700);
    chassis.arcade(-80, -127);
    pros::delay(200);
    chassis.arcade(127, 127);
    pros::delay(500);

    //go back for little bit of time and chain to the next swing to use the aligner to align to the goal
    chassis.arcade(-100, 0);
    pros::delay(400);

    chassis.swingToHeading(270, DriveSide::RIGHT, 2000,{
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion during the chained movement
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    chassis.arcade(-100, 0);
    pros::delay(400);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(400);
    claw.open();
    pros::delay(300);
    chassis.arcade(0, 0);
    // finished first pin
    
    // // // align on the goal and reset pose
    align_pose(270); 
    ////

    // create a task to parrelly move the bar to the front 
    pros::Task barTask([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(900);
        bar.motor.brake();
    });;
    
    pros::delay(20);
    
    // go forward off aligner
    chassis.moveToPoint(0, 10, 700, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();

    // turn to face the stack
    chassis.turnToHeading(-126, 1000, {
        // .earlyExitRange = 10
    });
    chassis.waitUntilDone();


    chassis.moveToPoint(-22, -10, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();
    //-22.6, -11.9, -129

    grab_up();

    // score this one
 
    //lift the lift at the same time, turn, forward and score
    lift.setTargetHeight(700);
    chassis.turnToHeading(-252, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-4, -10.75, 4000);//-5.5, -13.5
    chassis.waitUntilDone();
    lift.setTargetHeight(150);
    pros::delay(50);
    claw.open();
    pros::delay(100);


    // Now pick second stack
    chassis.moveToPoint(-10.5, -11.5, 1200, {
        .maxSpeed = 100
    });
    chassis.waitUntilDone();

    chassis.swingToHeading(-217, DriveSide::LEFT, 1000);
    chassis.waitUntilDone();


    chassis.moveToPoint(-2, -33, 800);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    chassis.waitUntilDone();

    grab_up(); 
    
    //theta -205
    // x = -2
    // y =-34
    //score this

    //-216
    //-2
    //-33
    lift.setTargetHeight(1000);
    chassis.turnToHeading(-341, 1000);
    chassis.waitUntilDone();

    chassis.moveToPoint(1.5, -15.5, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();
    lift.setTargetHeight(700);
    claw.open();
}


void fourPin_red2() {
    // set first chassis pose with a slight offset to the right
    chassis.setPose(0, 0, 10);

    // Toggle Action: Speed up if needed
    chassis.arcade(127, -127);
    pros::delay(700);
    chassis.arcade(-80, 127);
    pros::delay(200);
    chassis.arcade(127, -127);
    pros::delay(500);

    //go back for little bit of time and chain to the next swing to use the aligner to align to the goal
    chassis.arcade(-100, 0);
    pros::delay(400);

    chassis.swingToHeading(90, DriveSide::LEFT, 2000,{
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion during the chained movement
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    chassis.arcade(-100, 0);
    pros::delay(400);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(400);
    claw.open();
    pros::delay(300);
    chassis.arcade(0, 0);
    // finished first pin
    
    // // // align on the goal and reset pose
    align_pose(90);

// / -------- the following is the new auton code ------------ /

    // Create a task to parallelly move the bar to the front
    pros::Task barTask([&] {
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(900);
        bar.motor.brake();
    });

    pros::delay(20);

    // Go forward off aligner
    // x = 0, so no change
    chassis.moveToPoint(0, 10, 700, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();


    // Turn to face the stack
    // MIRRORED: -126 -> 126
    chassis.turnToHeading(126, 1200, {
        // .earlyExitRange = 10
    });
    chassis.waitUntilDone();

    // MIRRORED:
    // x: -23 -> +23
    // y: -10 unchanged
    chassis.moveToPoint(23, -10, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();


    // Original:
    // -22.6, -11.9, -129
    //
    // Mirrored:
    // +22.6, -11.9, +129

    grab_up();


    // Score this one

    // Lift the lift at the same time, turn, forward and score
    lift.setTargetHeight(700);

    // MIRRORED: -252 -> 252
    chassis.turnToHeading(252, 1000);
    chassis.waitUntilDone();

    return;



    // MIRRORED:
    // x: -4 -> +4
    // y unchanged
    chassis.moveToPoint(4, -10.75, 4000);
    // Original tuning point: -5.5, -13.5
    // Mirrored equivalent: +5.5, -13.5

    chassis.waitUntilDone();

    lift.setTargetHeight(150);
    pros::delay(50);

    claw.open();
    pros::delay(100);


    // Now pick second stack

    // MIRRORED:
    // x: -10.5 -> +10.5
    chassis.moveToPoint(10.5, -11.5, 1200, {
        .maxSpeed = 100
    });
    chassis.waitUntilDone();


    // MIRRORED:
    // heading: -217 -> 217
    // LEFT -> RIGHT
    chassis.swingToHeading(217, DriveSide::RIGHT, 1000);
    chassis.waitUntilDone();


    // MIRRORED:
    // x: -2 -> +2
    chassis.moveToPoint(2, -33, 800);

    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    chassis.waitUntilDone();

    grab_up();


    // Original:
    // theta = -205
    // x = -2
    // y = -34
    //
    // Mirrored:
    // theta = 205
    // x = +2
    // y = -34


    // Score this

    // Original:
    // heading = -216
    // x = -2
    // y = -33
    //
    // Mirrored:
    // heading = 216
    // x = +2
    // y = -33

    lift.setTargetHeight(1000);

    // MIRRORED: -341 -> 341
    chassis.turnToHeading(341, 1000);
    chassis.waitUntilDone();


    // MIRRORED:
    // x: +1.5 -> -1.5
    // y unchanged
    chassis.moveToPoint(-1.5, -15.5, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();

    lift.setTargetHeight(700);

    claw.open();

// / ---------- below is the old auton code ---------- /

    // // move bar forward
    // pros::Task barTask([&]{
    //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //     bar.motor.move(127);
    //     pros::delay(800);
    //     bar.motor.brake();
    // });

    // pros::delay(20);


    // // go forward off aligner
    // chassis.moveToPoint(0, 9, 700);
    // chassis.waitUntilDone();


    // // flipped heading
    // chassis.turnToHeading(124, 1000);
    // chassis.waitUntilDone();


    // // flipped X: -22.5 -> 22.5
    // chassis.moveToPoint(22.5, -11.5, 800);
    // chassis.waitUntilDone();

    // grab_up();


    // // score first stack
    // lift.setTargetHeight(700);


    // // flipped heading
    // chassis.turnToHeading(252, 1000);
    // chassis.waitUntilDone();


    // // flipped X: -3.5 -> 3.5
    // chassis.moveToPoint(3.5, -12.5, 4000);
    // chassis.waitUntilDone();

    // lift.setTargetHeight(150);
    // pros::delay(50);
    // claw.open();
    // pros::delay(100);


    // // Pick second stack
    // // flipped X: -9.34 -> 9.34
    // chassis.moveToPoint(9.34, -11.5, 1200, {
    //     .maxSpeed = 100
    // });
    // chassis.waitUntilDone();


    // // flipped heading + swing side
    // chassis.swingToHeading(205, DriveSide::RIGHT, 800);
    // chassis.waitUntilDone();


    // // flipped X: -1 -> 1
    // chassis.moveToPoint(1, -34.5, 800);

    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // chassis.waitUntilDone();

    // grab_up();


    // // Score second stack
    // lift.setTargetHeight(1000);


    // // flipped heading
    // chassis.turnToHeading(341, 1000);
    // chassis.waitUntilDone();


    // // flipped X: 1.5 -> -1.5
    // chassis.moveToPoint(-1.5, -15.5, 1000);

    // chassis.waitUntilDone();

    // lift.setTargetHeight(700);
    // claw.open();


    // // above is the fourpin_BlueAuton2 code
}

void fourPin_blue1() {
// / -------- the following is the new auton code ------------ /
    // set first chassis pose with a slight offset to the right
    chassis.setPose(0, 0, 350);

    // Toggle Action: Speed up if needed
    chassis.arcade(127, 127);
    pros::delay(700);

    //go back for little bit of time and chain to the next swing to use the aligner to align to the goal
    chassis.arcade(-100, 0);
    pros::delay(400);

    chassis.swingToHeading(270, DriveSide::RIGHT, 2000,{
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion during the chained movement
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    chassis.arcade(-100, 0);
    pros::delay(400);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(400);
    claw.open();
    pros::delay(300);
    chassis.arcade(0, 0);
    // finished first pin
    
    // // // align on the goal and reset pose
    align_pose(270); 
    ////

    // create a task to parrelly move the bar to the front 
    pros::Task barTask([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(900);
        bar.motor.brake();
    });;
    
    pros::delay(20);
    
    // go forward off aligner
    chassis.moveToPoint(0, 10, 700, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();

    // turn to face the stack
    chassis.turnToHeading(-126, 1000, {
        // .earlyExitRange = 10
    });
    chassis.waitUntilDone();


    chassis.moveToPoint(-22, -10, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();
    //-22.6, -11.9, -129

    grab_up();

    // score this one
 
    //lift the lift at the same time, turn, forward and score
    lift.setTargetHeight(700);
    chassis.turnToHeading(-252, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-4, -10.75, 4000);//-5.5, -13.5
    chassis.waitUntilDone();
    lift.setTargetHeight(150);
    pros::delay(50);
    claw.open();
    pros::delay(100);


    // Now pick second stack
    chassis.moveToPoint(-10.5, -11.5, 1200, {
        .maxSpeed = 100
    });
    chassis.waitUntilDone();

    chassis.swingToHeading(-217, DriveSide::LEFT, 1000);
    chassis.waitUntilDone();


    chassis.moveToPoint(-2, -33, 800);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    chassis.waitUntilDone();

    grab_up(); 
    
    //theta -205
    // x = -2
    // y =-34
    //score this

    //-216
    //-2
    //-33
    lift.setTargetHeight(1000);
    chassis.turnToHeading(-341, 1000);
    chassis.waitUntilDone();

    chassis.moveToPoint(1.5, -15.5, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();
    lift.setTargetHeight(700);
    claw.open();
}

void fourPin_blue2() {
    // set first chassis pose with a slight offset to the right
    chassis.setPose(0, 0, 10);

    // Toggle Action: Speed up if needed
    chassis.arcade(127, -127);
    pros::delay(700);

    //go back for little bit of time and chain to the next swing to use the aligner to align to the goal
    chassis.arcade(-100, 0);
    pros::delay(400);

    chassis.swingToHeading(270, DriveSide::LEFT, 2000,{
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion during the chained movement
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    chassis.arcade(-100, 0);
    pros::delay(400);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(400);
    claw.open();
    pros::delay(300);
    chassis.arcade(0, 0);
    // finished first pin
    
    // // // align on the goal and reset pose
    align_pose(90);

 // / -------- the following is the new auton code ------------ /

    // Create a task to parallelly move the bar to the front
    pros::Task barTask([&] {
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(900);
        bar.motor.brake();
    });

    pros::delay(20);

    // Go forward off aligner
    // x = 0, so no change
    chassis.moveToPoint(0, 10, 700, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();


    // Turn to face the stack
    // MIRRORED: -126 -> 126
    chassis.turnToHeading(126, 1000, {
        // .earlyExitRange = 10
    });
    chassis.waitUntilDone();


    // MIRRORED:
    // x: -23 -> +23
    // y: -10 unchanged
    chassis.moveToPoint(23, -10, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();

    // Original:
    // -22.6, -11.9, -129
    //
    // Mirrored:
    // +22.6, -11.9, +129

    grab_up();


    // Score this one

    // Lift the lift at the same time, turn, forward and score
    lift.setTargetHeight(700);

    // MIRRORED: -252 -> 252
    chassis.turnToHeading(252, 1000);
    chassis.waitUntilDone();

    // MIRRORED:
    // x: -4 -> +4
    // y unchanged
    chassis.moveToPoint(4, -10.75, 4000);
    // Original tuning point: -5.5, -13.5
    // Mirrored equivalent: +5.5, -13.5

    chassis.waitUntilDone();

    lift.setTargetHeight(150);
    pros::delay(50);

    claw.open();
    pros::delay(100);


    // Now pick second stack

    // MIRRORED:
    // x: -10.5 -> +10.5
    chassis.moveToPoint(10.5, -11.5, 1200, {
        .maxSpeed = 100
    });
    chassis.waitUntilDone();


    // MIRRORED:
    // heading: -217 -> 217
    // LEFT -> RIGHT
    chassis.swingToHeading(217, DriveSide::RIGHT, 1000);
    chassis.waitUntilDone();


    // MIRRORED:
    // x: -2 -> +2
    chassis.moveToPoint(2, -33, 800);

    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    chassis.waitUntilDone();

    grab_up();


    // Original:
    // theta = -205
    // x = -2
    // y = -34
    //
    // Mirrored:
    // theta = 205
    // x = +2
    // y = -34


    // Score this

    // Original:
    // heading = -216
    // x = -2
    // y = -33
    //
    // Mirrored:
    // heading = 216
    // x = +2
    // y = -33

    lift.setTargetHeight(1000);

    // MIRRORED: -341 -> 341
    chassis.turnToHeading(341, 1000);
    chassis.waitUntilDone();


    // MIRRORED:
    // x: +1.5 -> -1.5
    // y unchanged
    chassis.moveToPoint(-1.5, -15.5, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();

    lift.setTargetHeight(700);

    claw.open();

// / ---------- below is the old auton code ---------- /

    // // move bar forward
    // pros::Task barTask([&]{
    //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //     bar.motor.move(127);
    //     pros::delay(800);
    //     bar.motor.brake();
    // });

    // pros::delay(20);


    // // go forward off aligner
    // chassis.moveToPoint(0, 9, 700);
    // chassis.waitUntilDone();


    // // flipped heading
    // chassis.turnToHeading(124, 1000);
    // chassis.waitUntilDone();


    // // flipped X: -22.5 -> 22.5
    // chassis.moveToPoint(22.5, -11.5, 800);
    // chassis.waitUntilDone();

    // grab_up();


    // // score first stack
    // lift.setTargetHeight(700);


    // // flipped heading
    // chassis.turnToHeading(252, 1000);
    // chassis.waitUntilDone();


    // // flipped X: -3.5 -> 3.5
    // chassis.moveToPoint(3.5, -12.5, 4000);
    // chassis.waitUntilDone();

    // lift.setTargetHeight(150);
    // pros::delay(50);
    // claw.open();
    // pros::delay(100);


    // // Pick second stack
    // // flipped X: -9.34 -> 9.34
    // chassis.moveToPoint(9.34, -11.5, 1200, {
    //     .maxSpeed = 100
    // });
    // chassis.waitUntilDone();


    // // flipped heading + swing side
    // chassis.swingToHeading(205, DriveSide::RIGHT, 800);
    // chassis.waitUntilDone();


    // // flipped X: -1 -> 1
    // chassis.moveToPoint(1, -34.5, 800);

    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // chassis.waitUntilDone();

    // grab_up();


    // // Score second stack
    // lift.setTargetHeight(1000);


    // // flipped heading
    // chassis.turnToHeading(341, 1000);
    // chassis.waitUntilDone();


    // // flipped X: 1.5 -> -1.5
    // chassis.moveToPoint(-1.5, -15.5, 1000);

    // chassis.waitUntilDone();

    // lift.setTargetHeight(700);
    // claw.open();
}

void Post() {
    //
    chassis.arcade(-110, -40);
    // claw.open();
    pros::delay(650);
    lift.setLiftStage(200);
    chassis.arcade(-80,0);
    pros::delay(2000);
    chassis.arcade(0,0);
    
    //reset pose
    chassis.setPose(0, 0, 0);
    chassis.swingToHeading(-106, DriveSide::LEFT, 1000, {}, false);
    chassis.arcade(70, 0);
    pros::delay(1000);
    chassis.arcade(0,0);
    grab_up();

    //-21,4,-106

    //score
    chassis.arcade(-100, 0);
    pros::delay(1250);
    chassis.arcade(-40, 0);
    score_back();
    chassis.tank(0, 0);
    align_pose(270);


    //repeat

    //second one
    // pick from loader
    chassis.arcade(100, 0);
    bar.motor.move(110);
    pros::delay(300);
    chassis.arcade(0, 0);
    chassis.swingToHeading(-17, DriveSide::LEFT, 1000);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    bar.motor.brake();
    chassis.waitUntilDone();

    // move to point to pick
    chassis.moveToPoint(-10, 26, 1000);
    chassis.waitUntilDone();
    chassis.arcade(80, 0);
    pros::delay(200);
    chassis.arcade(0, 0);
    grab_up();
    pros::delay(200);

    // score
    lift.setTargetHeight(600);
    chassis.arcade(-100, 0);
    pros::delay(1250);
    chassis.arcade(-40, 0);
    score_back();
    chassis.tank(0, 0);
    align_pose(0);


    //go forward wall square
    chassis.arcade(70, 0);
    pros::delay(300);
    lift.setTargetHeight(200);
    pros::delay(400);
    chassis.arcade(0, 0);
    chassis.turnToHeading(90, 800);
    chassis.waitUntilDone();
    bar.motor.move(100);
    chassis.arcade(-80, 0);
    pros::delay(700);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    bar.motor.brake();
    pros::delay(1300);
    chassis.setPose(0, 0, 0);


    //third one
    // pick from loader
    chassis.swingToHeading(-106, DriveSide::LEFT, 1000, {}, false);
    chassis.arcade(70, 0);
    pros::delay(1000);
    chassis.arcade(0,0);
    grab_up();
    // score
    chassis.arcade(-100, 0);
    pros::delay(500);
    lift.setTargetHeight(900);
    pros::delay(750);
    chassis.arcade(-40, 0);
    score_back();
    chassis.tank(0, 0);
    align_pose(270);

    //fourth one
    // pick from loader
    chassis.arcade(100, 0);
    bar.motor.move(110);
    pros::delay(300);
    chassis.arcade(0, 0);
    lift.setTargetHeight(200);
    chassis.swingToHeading(-18, DriveSide::LEFT, 1000);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    bar.motor.brake();
    chassis.waitUntilDone();

    // move to point to pick
    chassis.moveToPoint(-10, 26, 1000);
    chassis.waitUntilDone();
    chassis.arcade(80, 0);
    pros::delay(200);
    chassis.arcade(0, 0);
    grab_up();
    pros::delay(200);

    // score
    chassis.arcade(-100, 0);
    lift.setTargetHeight(1200);
    pros::delay(1250);
    chassis.arcade(-40, 0);
    score_back();
    chassis.tank(0, 0);
    align_pose(0);
    pros::delay(500);


    //go forward wall square
    chassis.arcade(70, 0);
    pros::delay(300);
    lift.setTargetHeight(200);
    pros::delay(400);
    chassis.arcade(0, 0);
    chassis.turnToHeading(90, 800);
    chassis.waitUntilDone();
    bar.motor.move(100);
    chassis.arcade(-80, 0);
    pros::delay(700);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    bar.motor.brake();
    pros::delay(1300);
    chassis.arcade(0, 0);
    chassis.setPose(0, 0, 0);
    
    //go do toggle and mid
    chassis.moveToPoint(0, 18, 3000, {.minSpeed = 100});
    chassis.swingToHeading(90, DriveSide::RIGHT, 1000, {.minSpeed = 100});
    chassis.moveToPoint(44, 22.5, 1000, {.minSpeed = 100});
    bar.motor.move(-100);
    chassis.swingToHeading(180, DriveSide::RIGHT, 1000);
    pros::delay(50);
    bar.motor.move(0);
    bar.motor.get_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    bar.motor.brake();
    chassis.waitUntilDone();

    chassis.arcade(100, 0);
    pros::delay(1250);
    chassis.arcade(-127, 0);
    pros::delay(500);
    chassis.arcade(0, 0);
}

void threePin_red1() {
    // set first chassis pose with a slight offset to the right
    chassis.setPose(0, 0, 350);

    // Toggle Action: Speed up if needed
    chassis.arcade(127, 127);
    pros::delay(700);
    chassis.arcade(-80, -127);
    pros::delay(200);
    chassis.arcade(127, 127);
    pros::delay(500);

    //go back for little bit of time and chain to the next swing to use the aligner to align to the goal
    chassis.arcade(-100, 0);
    pros::delay(400);

    chassis.swingToHeading(270, DriveSide::RIGHT, 2000,{
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion during the chained movement
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    chassis.arcade(-100, 0);
    pros::delay(400);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(400);
    claw.open();
    pros::delay(300);
    chassis.arcade(0, 0);
    // finished first pin
    
    // // // align on the goal and reset pose
    align_pose(270); 
    ////

    // create a task to parrelly move the bar to the front 
    pros::Task barTask([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(900);
        bar.motor.brake();
    });;
    
    pros::delay(20);
    
    // go forward off aligner
    chassis.moveToPoint(0, 10, 700, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();

    // turn to face the stack
    chassis.turnToHeading(-126, 1000, {
        // .earlyExitRange = 10
    });
    chassis.waitUntilDone();


    chassis.moveToPoint(-23, -10, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();
    //-22.6, -11.9, -129

    grab_up();

    // score this one
 
    //lift the lift at the same time, turn, forward and score
    lift.setTargetHeight(700);
    chassis.turnToHeading(-252, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-4, -10.75, 4000);//-5.5, -13.5
    chassis.waitUntilDone();
    lift.setTargetHeight(150);
    pros::delay(50);
    claw.open();
    pros::delay(100);


    // Now pick second stack
    chassis.moveToPoint(-10.5, -11.5, 1200, {
        .maxSpeed = 100
    });
    chassis.waitUntilDone();

    chassis.swingToHeading(-217, DriveSide::LEFT, 1000);
    chassis.waitUntilDone();

    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}


void skillsAuton() {
    fourPin_red2();
    return;
    Post();
}

void twoPin_red(){
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

    chassis.arcade(-100, 0);
    pros::delay(1000);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(600);
    claw.open();
    chassis.arcade(0, 0);
    // finished first pin
    
    align_pose(270);

    pros::delay(200);
    

    chassis.arcade(60, 0);
    pros::delay(500);
    chassis.arcade(0, 0);
    // controller.rumble(".");

    // //starting from aligner
    // chassis.setPose(0, 0, 0);


    // next movement setting up to pickup first pin cup stack
    // chassis.moveToPose(0, 9, 0, 2000); //-20, -10
    // chassis.waitUntilDone();

    // chassis.turnToHeading(118, 1000);
    // pros::Task barTask([&]{
    //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //     bar.motor.move(127);
    //     pros::delay(800);
    //     bar.motor.brake();
    // });;
    // chassis.waitUntilDone();
    // chassis.moveToPose(18, -4.5, 118, 4000);
    // claw.open();
    // chassis.waitUntilDone();

    // claw.close();
    // pros::delay(100);
    // bar.motor.move(-60);
    // pros::delay(200);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // bar.motor.brake();

    // chassis.moveToPoint(11, 0, 4000, {.forwards = false});
    // chassis.waitUntilDone();

    // bar.motor.move(-100);
    // pros::delay(600);
    // claw.open();
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // bar.motor.brake();

    // // pick next
    // chassis.turnToHeading(129, 1000);
    // pros::Task barTask1([&]{
    //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //     bar.motor.move(127);
    //     pros::delay(800);
    //     bar.motor.brake();
    // });;
    // chassis.waitUntilDone();

    // chassis.moveToPose(18, -6.5, 129, 4000);
    // chassis.waitUntilDone();
    // claw.close();
    // pros::delay(100);
    // bar.motor.move(-60);
    // pros::delay(200);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // bar.motor.brake();

    // // score it
    // chassis.moveToPose(8, 1, 129, 2000, {.forwards = false});
    // lift.setTargetHeight(700);
    // chassis.turnToHeading(202, 1000);
    // chassis.waitUntilDone();
    // chassis.moveToPose(2.5, -5, 202, 3000);
    // chassis.waitUntilDone();
    // lift.setTargetHeight(400);
    // claw.open();
}

void twoPin_blue1(){
// / -------- the following is the new auton code ------------ /
    // set first chassis pose with a slight offset to the right
    chassis.setPose(0, 0, 350);

    // Toggle Action: Speed up if needed
    chassis.arcade(127, 127);
    pros::delay(700);
    chassis.arcade(-80, -127);
    pros::delay(200);
    chassis.arcade(127, 127);
    pros::delay(500);

    //go back for little bit of time and chain to the next swing to use the aligner to align to the goal
    chassis.arcade(-100, 0);
    pros::delay(350);

    chassis.swingToHeading(270, DriveSide::RIGHT, 2000,{
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion during the chained movement
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    chassis.arcade(-100, 0);
    pros::delay(400);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(400);
    claw.open();
    pros::delay(300);
    chassis.arcade(0, 0);
    // finished first pin
    
    // // // align on the goal and reset pose
    align_pose(270); 
    //// 
}

void threePin_red2() {
    // set first chassis pose with a slight offset to the right
    chassis.setPose(0, 0, 10);

    // Toggle Action: Speed up if needed
    chassis.arcade(127, -127);
    pros::delay(700);
    chassis.arcade(-80, 127);
    pros::delay(200);
    chassis.arcade(127, -127);
    pros::delay(500);

    //go back for little bit of time and chain to the next swing to use the aligner to align to the goal
    chassis.arcade(-100, 0);
    pros::delay(350);

    chassis.swingToHeading(90, DriveSide::LEFT, 2000,{
        .minSpeed = 127,
        .earlyExitRange = 20
    });

    // Subsystem motion during the chained movement
    bar.motor.move(60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.waitUntilDone();

    chassis.arcade(-100, 0);
    pros::delay(400);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(400);
    claw.open();
    pros::delay(300);
    chassis.arcade(0, 0);
    // finished first pin
    
    // // // align on the goal and reset pose
    align_pose(90);

// / -------- the following is the new auton code ------------ /

    // Create a task to parallelly move the bar to the front
    pros::Task barTask([&] {
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(900);
        bar.motor.brake();
    });

    pros::delay(20);

    // Go forward off aligner
    // x = 0, so no change
    chassis.moveToPoint(0, 10, 700, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();


    // Turn to face the stack
    // MIRRORED: -126 -> 126
    chassis.turnToHeading(126, 1200, {
        // .earlyExitRange = 10
    });
    chassis.waitUntilDone();

    // MIRRORED:
    // x: -23 -> +23
    // y: -10 unchanged
    chassis.moveToPoint(23, -10, 1000, {
        // .minSpeed = 100
    });
    chassis.waitUntilDone();


    // Original:
    // -22.6, -11.9, -129
    //
    // Mirrored:
    // +22.6, -11.9, +129

    grab_up();


    // Score this one

    // Lift the lift at the same time, turn, forward and score
    lift.setTargetHeight(700);

    // MIRRORED: -252 -> 252
    chassis.turnToHeading(252, 1000);
    chassis.waitUntilDone();




    // MIRRORED:
    // x: -4 -> +4
    // y unchanged
    chassis.moveToPoint(4, -10.75, 4000);
    // Original tuning point: -5.5, -13.5
    // Mirrored equivalent: +5.5, -13.5

    chassis.waitUntilDone();

    lift.setTargetHeight(150);
    pros::delay(50);

    claw.open();
    pros::delay(100);


    // Now pick second stack

    // MIRRORED:
    // x: -10.5 -> +10.5
    chassis.moveToPoint(10.5, -11.5, 1200, {
        .maxSpeed = 100
    });
    chassis.waitUntilDone();


    // MIRRORED:
    // heading: -217 -> 217
    // LEFT -> RIGHT
    chassis.swingToHeading(217, DriveSide::RIGHT, 1000);
    chassis.waitUntilDone();


    // // MIRRORED:
    // // x: -2 -> +2
    // chassis.moveToPoint(2, -33, 800);

    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    // chassis.waitUntilDone();

    // grab_up();


    // Original:
    // theta = -205
    // x = -2
    // y = -34
    //
    // Mirrored:
    // theta = 205
    // x = +2
    // y = -34


    // Score this

    // Original:
    // heading = -216
    // x = -2
    // y = -33
    //
    // Mirrored:
    // heading = 216
    // x = +2
    // y = -33

    // lift.setTargetHeight(1000);

    // // MIRRORED: -341 -> 341
    // chassis.turnToHeading(341, 1000);
    // chassis.waitUntilDone();


    // // MIRRORED:
    // // x: +1.5 -> -1.5
    // // y unchanged
    // chassis.moveToPoint(-1.5, -15.5, 1000, {
    //     // .minSpeed = 100
    // });
    // chassis.waitUntilDone();

    // lift.setTargetHeight(700);

    // claw.open();

// / ---------- below is the old auton code ---------- /

    // // move bar forward
    // pros::Task barTask([&]{
    //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //     bar.motor.move(127);
    //     pros::delay(800);
    //     bar.motor.brake();
    // });

    // pros::delay(20);


    // // go forward off aligner
    // chassis.moveToPoint(0, 9, 700);
    // chassis.waitUntilDone();


    // // flipped heading
    // chassis.turnToHeading(124, 1000);
    // chassis.waitUntilDone();


    // // flipped X: -22.5 -> 22.5
    // chassis.moveToPoint(22.5, -11.5, 800);
    // chassis.waitUntilDone();

    // grab_up();


    // // score first stack
    // lift.setTargetHeight(700);


    // // flipped heading
    // chassis.turnToHeading(252, 1000);
    // chassis.waitUntilDone();


    // // flipped X: -3.5 -> 3.5
    // chassis.moveToPoint(3.5, -12.5, 4000);
    // chassis.waitUntilDone();

    // lift.setTargetHeight(150);
    // pros::delay(50);
    // claw.open();
    // pros::delay(100);


    // // Pick second stack
    // // flipped X: -9.34 -> 9.34
    // chassis.moveToPoint(9.34, -11.5, 1200, {
    //     .maxSpeed = 100
    // });
    // chassis.waitUntilDone();


    // // flipped heading + swing side
    // chassis.swingToHeading(205, DriveSide::RIGHT, 800);
    // chassis.waitUntilDone();


    // // flipped X: -1 -> 1
    // chassis.moveToPoint(1, -34.5, 800);

    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // chassis.waitUntilDone();

    // grab_up();


    // // Score second stack
    // lift.setTargetHeight(1000);


    // // flipped heading
    // chassis.turnToHeading(341, 1000);
    // chassis.waitUntilDone();


    // // flipped X: 1.5 -> -1.5
    // chassis.moveToPoint(-1.5, -15.5, 1000);

    // chassis.waitUntilDone();

    // lift.setTargetHeight(700);
    // claw.open();


    // // above is the fourpin_BlueAuton2 code
}