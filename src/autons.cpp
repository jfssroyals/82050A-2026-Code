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

void align_pose(double straight){
    double goalRadius = 6.5;

    // Current robot angle
    double currentTheta = chassis.getPose().theta;

    // Straight alignment angle
    straight = 270.0;

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

void fivePin_red1() {
    
    pros::Task liftTask([&] { // lift.updateComplexLift will run independently every 20 ms
        while (true) {
            lift.updateComplexLift();
            pros::delay(20);
        }
    });

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
    pros::delay(300);

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
    pros::delay(500);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    pros::delay(600);
    claw.open();
    chassis.arcade(0, 0);
    // finished first pin
    
    // align on the goal and reset pose
    align_pose(270);

    pros::delay(20);

    // next movement setting up to pickup first pin cup stack, dont go too fast
    chassis.moveToPose(0, 9, 0, 900); //-20, -10
    chassis.waitUntilDone();

    // create a task to parrelly move the bar to the front 
    pros::Task barTask([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(800);
        bar.motor.brake();
    });;
    
    // turn to pick up the first cup to throw away
    chassis.turnToHeading(118, 800);
    chassis.waitUntilDone();

    // move to the cup
    chassis.moveToPose(18, -4.5, 118, 1250);
    claw.open();
    chassis.waitUntilDone();

    // pick the cup
    claw.close();
    pros::delay(100);

    // while going backwards throw the cup away 
    chassis.moveToPoint(11, 0, 4000, {.forwards = false, .minSpeed = 100});
    bar.motor.move(-127);
    pros::delay(600);
    claw.open();
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    bar.motor.brake();
    chassis.waitUntilDone();

    // turn to face the stack (CHANGE LATER)
    chassis.turnToHeading(129, 1000);
    pros::Task barTask1([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(800);
        bar.motor.brake();
    });;
    chassis.waitUntilDone();

    chassis.moveToPose(18, -6.5, 129, 1500);
    chassis.waitUntilDone();
    claw.close();
    pros::delay(100);
    bar.motor.move(-60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    // score it
    chassis.moveToPose(8, 1, 129, 2000, {.forwards = false});
    // chassis.waitUntilDone();
    lift.setTargetHeight(700);
    chassis.turnToHeading(202, 1000);
    chassis.waitUntilDone();
    chassis.moveToPose(2.5, -5, 202, 3000);
    chassis.waitUntilDone();
    lift.setTargetHeight(400);

    // // go backwards, turn to face the standalone pin, cup
    // chassis.moveToPose(5, 3, 199, 800, {.forwards = false});
    // lift.setTargetHeight(175);
    // chassis.waitUntilDone();
    // chassis.turnToHeading(238, 600);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // chassis.waitUntilDone();

    // // go forward and pick
    // chassis.moveToPose(-19, -8, 236, 1250);
    // chassis.waitUntilDone();
    // claw.close();
    // pros::delay(100);
    // bar.motor.move(-60);
    // pros::delay(200);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // bar.motor.brake();

    // //lift the lift, go backwards, turn to face the goal, go forward and score
    // lift.setTargetHeight(1200);
    // chassis.moveToPose(-13, -4, 236, 800); //min = 100
    // // chassis.waitUntilDone();
    // chassis.turnToHeading(129, 600); //min speed = 100
    // chassis.waitUntilDone();
    // chassis.moveToPose(-0.5, -6, 129, 800);
    // chassis.waitUntilDone();
    // lift.setTargetHeight(800);
    // claw.open();

}


void fivePin_red2() {


}

void fivePin_blue1() {

}

void fivePin_blue2() {

}

//ASSET(askillpath1_txt);
ASSET(mtest_txt);

void skillsAuton() {
    
    pros::Task liftTask([&] { // lift.updateComplexLift will run independently every 20 ms
        while (true) {
            lift.updateComplexLift();
            pros::delay(20);
        }
    });

    // pros::delay(2000);

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

    // chassis.arcade(-60, 0);
    // pros::delay(1000);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // pros::delay(600);
    // claw.open();
    // chassis.arcade(0, 0);
    // // finished first pin
    
    // //reset pose when on aligner right before leaving

    // double goalRadius = 6.25;

    // // Current robot angle
    // double currentTheta = chassis.getPose().theta;

    // // Straight alignment angle
    // double straightTheta = 270.0;

    // // Calculate rotation offset from straight
    // double offsetTheta = currentTheta - straightTheta;

    // // Normalize angle between -180 and 180
    // while (offsetTheta > 180) offsetTheta -= 360;
    // while (offsetTheta < -180) offsetTheta += 360;

    // // Calculate offset around goal using degrees
    // double offsetX = sin(offsetTheta * M_PI / 180.0) * goalRadius;
    // double offsetY = (-cos(offsetTheta * M_PI / 180.0) * goalRadius) + goalRadius;

    // // Set new coordinate system
    // chassis.setPose(offsetX, offsetY, offsetTheta);

    // pros::delay(200);
    
    chassis.setPose(0, 0, 0);

    // move back a little
    chassis.moveToPose(0, 9, 0, 1000);
    // Subsystem motion while moving
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();
    chassis.waitUntilDone();

    chassis.turnToHeading(308, 1000);
    chassis.waitUntilDone();

    chassis.moveToPoint(-13,12,10000, {.maxSpeed = 60});
    chassis.waitUntilDone();

    chassis.swingToHeading(0, DriveSide::RIGHT, 3000, {.maxSpeed = 50});
    chassis.waitUntilDone();

    chassis.moveToPose(-13, 75, 0, 5000, {.maxSpeed = 100});
    claw.open();
    chassis.waitUntilDone();

    chassis.turnToHeading(133, 1000, {.maxSpeed = 70});
    pros::Task barTask([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(800);
        bar.motor.brake();
    });
    chassis.waitUntilDone();
    
    chassis.moveToPose(-5, 63, 129, 5000, {.maxSpeed = 40});
    chassis.waitUntilDone();

    claw.close();
    pros::delay(200);
    bar.motor.move(-60);
    pros::delay(300);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.moveToPose(5, 53, 129, 5000);
    chassis.waitUntilDone();

    chassis.arcade(-80, 0);
    pros::delay(400);
    chassis.arcade(0, 0);

    chassis.turnToHeading(350, 1500);
    chassis.waitUntilDone();

    skillsScore_L();

    lift.setTargetHeight(0);

    



    // chassis.moveToPose(-2.5, 75, 0, 3000);
    // chassis.waitUntilDone();
    // chassis.turnToHeading(90, 800);
    // chassis.waitUntilDone();
    // chassis.moveToPose(15.5, 70, 90, 3000);
    // chassis.waitUntilDone();

    // //repeatable code
    // //close claw
    // claw.close();
    // //lift bar up upwards
    // bar.motor.move(-60);
    // pros::delay(300);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // bar.motor.brake();

    // chassis.moveToPoint(-30,74,10000);
    // chassis.waitUntilDone();

    // chassis.moveToPoint(1, 71, 10000, {.maxSpeed = 90});
    // claw.open();
    // bar.motor.move(100);
    // pros::delay(700);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // bar.motor.brake();
    // chassis.waitUntilDone();

    // chassis.swingToHeading(86, DriveSide::RIGHT, 2000);
    // chassis.waitUntilDone();

    // chassis.moveToPose(15, 71, 82, 3000); 
    // chassis.waitUntilDone();

    

    // chassis.arcade(-100, 0);
    // pros::delay(250);
    // chassis.arcade(0, 0);

    // //
    // chassis.swingToHeading(0, DriveSide::RIGHT, 2000);
    // chassis.waitUntilDone();

    // chassis.arcade(-80, 0);
    // pros::delay(3000);
    // chassis.arcade(0, 0);

    // bar.motor.move(-100);
    // pros::delay(800);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // bar.motor.brake();
    // claw.open();
    // //backup and score
    // chassis.moveToPoint(0, 70, 10000);
    // chassis.waitUntilDone();

    // chassis.turnToHeading(0, 2000);
    // chassis.waitUntilDone();

    // chassis.moveToPoint(0, 45, 10000, {.forwards = false});
    // chassis.waitUntilDone();

    
    // Back up while lowering the bar and opening the claw
    // chassis.arcade(-80, 0);
    // bar.motor.move(-80);
    // pros::delay(300);

    // claw.open();

    // pros::delay(300);

    // // Stop drivetrain
    // chassis.arcade(0, 0);

    // // Let the bar coast
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // bar.motor.move(0);

    // align_pose();
    // // repeat this code
    // chassis.moveToPose(0, 20, 0, 2000);
    // bar.motor.move(100);
    // pros::delay(700);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // bar.motor.brake();
    // chassis.waitUntilDone();

    // chassis.swingToHeading(90, DriveSide::RIGHT, 2000);
    // chassis.waitUntilDone();

    
    
    // // // Turn
    // chassis.turnToHeading(126, 1000);//122

    // // Start bar movement in the background 
    // pros::Task barTask([&]{
    //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    //     bar.motor.move(127);
    //     pros::delay(8bar.motor.move(60);
    // pros::delay(200);
    // bar.motor.set_00);
    //     bar.motor.brake();
    // });

    // chassis.waitUntilDone();

}

void skillsScore_L() {

//  / ------------------ Scoring Function ------------------- /
  
// Back up while lowering the bar and opening the claw
    // chassis.arcade(-60, 0);
    // pros::delay(3000);
    // chassis.arcade(0, 0);
    // // Stop drivetrain
    // bar.motor.move(-115);
    // pros::delay(600);
    // // Let the bar coast
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // bar.motor.brake();

    // pros::delay(200);

    // claw.open();
    // pros::delay(150);

    // align_pose(0);

    // chassis.arcade(60, 0);
    // pros::delay(3000);
    // chassis.arcade(0, 0);



}





void test() {
    //starting from aligner
    chassis.setPose(0, 0, 0);
    chassis.moveToPose(0, 29, 0, 2000);
    chassis.waitUntilDone();
    chassis.turnToHeading(90, 1500,{
        .maxSpeed = 50
        });
    
    
    chassis.waitUntilDone();
    chassis.moveToPoint(15, 29, 1000,{.maxSpeed=20} );
    claw.open();
    chassis.waitUntilDone();
    // claw.close();
    // bar.motor.move(-60);
    // pros::delay(100);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    // bar.motor.brake();
    // chassis.moveToPoint(5,25,1000);
    // chassis.waitUntilDone();
    // chassis.turnToHeading(0, 1000);
    // chassis.waitUntilDone();
    //chassis.moveToPoint(0, 0, 1000);
    //chassis.waitUntilDone();
    
    
}

