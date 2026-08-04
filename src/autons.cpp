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
    
    // pros::Task liftTask([&] { // lift.updateComplexLift will run independently every 20 ms
    //     while (true) {
    //         lift.updateComplexLift();
    //         pros::delay(20);
    //     }
    // });

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

    // chassis.arcade(-100, 0);
    // pros::delay(1000);
    // bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    // pros::delay(600);
    // claw.open();
    // chassis.arcade(0, 0);
    // // finished first pin
    
    // align_pose(270);

    // pros::delay(200);
    
    // controller.rumble(".");

    //starting from aligner
    chassis.setPose(0, 0, 0);

    // next movement setting up to pickup first pin cup stack
    chassis.moveToPose(0, 9, 0, 2000); //-20, -10
    chassis.waitUntilDone();

    chassis.turnToHeading(119, 1000);
    pros::Task barTask([&]{
        bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        bar.motor.move(127);
        pros::delay(800);
        bar.motor.brake();
    });;
    chassis.waitUntilDone();
    chassis.moveToPose(17.5, -4.25, 119, 4000);
    claw.open();
    chassis.waitUntilDone();

    claw.close();
    pros::delay(100);
    bar.motor.move(-60);
    pros::delay(200);
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    bar.motor.brake();

    chassis.moveToPoint(5, 2.5, 4000, {.forwards = false});
    chassis.waitUntilDone();
    // backwards, swing, score
    
    chassis.turnToHeading(185, 2000);
    chassis.waitUntilDone();  
    
    chassis.moveToPose(0, -4, 185, 3000);
    chassis.waitUntilDone();
    claw.open();

    // pick next

    // chassis.moveToPose(2, 1, 191, 2000, {.forwards = false});
    

    //2,1,191
    //133
    //15, -6, 133, 
    


    

    // chassis.waitUntilDone();

    // // // chassis.arcade(0, 0);
    // chassis.moveToPose(14, -5.25, 125, 4000, {.lead = 0.3, .maxSpeed = 30}); //-20, -10
    // // claw.open();
    // chassis.waitUntilDone();
    // claw.open();

    // chassis.arcade(55, 0);
    // pros::delay(400);
    // chassis.arcade(0, 0);
    
    // claw.close();
    // pros::delay(100);

    

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

    skillsScore();

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

void skillsScore() {

//  / ------------------ Scoring Function ------------------- /
  
// Back up while lowering the bar and opening the claw
    chassis.arcade(-60, 0);
    pros::delay(3000);
    chassis.arcade(0, 0);
    // Stop drivetrain
    bar.motor.move(-115);
    pros::delay(600);
    // Let the bar coast
    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    bar.motor.brake();

    pros::delay(200);

    claw.open();
    pros::delay(150);

    align_pose(0);

    chassis.arcade(60, 0);
    pros::delay(3000);
    chassis.arcade(0, 0);

}





void test() {
    
}

