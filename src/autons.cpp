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

void matchload() {
    // Use the matchload as the new origin without changing the heading.
    chassis.arcade(0, 0);
    chassis.setPose(0, 0, chassis.getPose().theta);
    claw.open();
    intake.spinInward();
    pros::delay(2000);
    intake.stop();

    // Pull forward 15 inches out of the loader before moving the lift.
    const double heading = chassis.getPose(true).theta;
    chassis.moveToPoint(15 * sin(heading), 15 * cos(heading), 3000,
                        {.forwards = true, .maxSpeed = 50}, false);

    // Briefly lower before gripping the loaded stack.
    lift.manual(-20);
    pros::delay(150);
    lift.stop();
    claw.close();
    pros::delay(100);
    // Raise for clearance, then flip the wrist while still raising.
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    lift.manual(100);
    pros::delay(100);
    wrist.moveUP();
    pros::delay(200);
    lift.stop();
}

void autonMatchloadTest() {
    match_load_ready();
    pros::delay(5000); // Time to manually position the robot at the loader.
    matchload();
}

void match_load_ready() {
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    lift.manual(100);
    pros::delay(300);
    lift.stop();
    wrist.moveDOWN();
    claw.open();
    lift.moveToMatchloadHeight(11.0);
    while (lift.isAuto()) {
        lift.update();
        pros::delay(5);
    }
}
namespace {
void waitForAutoClaw() {
    bool autoClawTriggered = false;
    while (chassis.isInMotion()) {
        int distance = distanceSensor.get(); // PROS 3.8 distance API; millimeters.

        if (distance > 0 && distance <= 100 && !autoClawTriggered) {
            autoClawTriggered = true;
            claw.close();
            lift.manual(127);
            pros::delay(300);
            lift.stop();
            chassis.cancelMotion();
        }

        pros::delay(5);
    }
}
} // namespace

void toggleDriveSequence() {
    // Backward: 870 ms total, slowing down before reversing.
    chassis.arcade(-127, 0);
    pros::delay(620);
    chassis.arcade(-100, 0);
    pros::delay(50);
    chassis.arcade(-75, 0);
    pros::delay(50);
    chassis.arcade(-50, 0);
    pros::delay(50);
    chassis.arcade(-25, 0);
    pros::delay(50);
    chassis.arcade(0, 0);
    pros::delay(50);

    // Forward: 300 ms total.
    chassis.arcade(100, 0);
    pros::delay(100);
    chassis.arcade(75, 0);
    pros::delay(50);
    chassis.arcade(50, 0);
    pros::delay(50);
    chassis.arcade(25, 0);
    pros::delay(50);
    chassis.arcade(0, 0);
    pros::delay(50);

    // Backward: 500 ms total.
    chassis.arcade(-100, 0);
    pros::delay(300);
    chassis.arcade(-75, 0);
    pros::delay(50);
    chassis.arcade(-50, 0);
    pros::delay(50);
    chassis.arcade(-25, 0);
    pros::delay(50);
    chassis.arcade(0, 0);
    pros::delay(50);

    // Forward: 300 ms total, slowing to a final stop.
    chassis.arcade(100, 0);
    pros::delay(100);
    chassis.arcade(75, 0);
    pros::delay(50);
    chassis.arcade(50, 0);
    pros::delay(50);
    chassis.arcade(25, 0);
    pros::delay(50);
    chassis.arcade(0, 0);
    pros::delay(50);
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

    // Calculate offset around goal using x`degrees
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
    // // set chassis pose
    // pros::lcd::print(5, "Skills: running auton2");
    auton2();
    return;
    pros::lcd::print(5, "Skills: turn to 90");
    chassis.turnToHeading(90, 3000, {.maxSpeed = 50}, false);
    pros::lcd::print(5, "Skills: pickup at -1,69");
    chassis.moveToPoint(-1, 69, 10000, {.maxSpeed = 50});
    waitForAutoClaw();

    pros::lcd::print(5, "Skills: pickup at -22,69");
    chassis.moveToPoint(-22, 69, 10000, {.maxSpeed = 50});
    waitForAutoClaw();
    chassis.turnToHeading(245, 3000, {.maxSpeed = 50}, false);

    // Score using auton1's opening single-pin sequence.
    chassis.arcade(100, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    lift.unfold(wrist);
    pros::delay(200);
    lift.stop();
    chassis.arcade(40, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    lift.manual(-100);
    pros::delay(300);
    lift.stop();
    pros::delay(200);
    claw.open();
    pros::delay(200);
    chassis.arcade(0, 0);

    // ADD MATCHLOAD CODE HERE

    // Do toggle and mid goal 
    chassis.moveToPoint(-1.3, 19.3, 1000, {.forwards = false});
    chassis.waitUntilDone();
    chassis.turnToHeading(-53, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-27, 31, 1000);
    chassis.waitUntilDone();
    chassis.turnToHeading(-1, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-36, 70, 1500);
    chassis.waitUntilDone();
    chassis.turnToHeading(-90, 1000);
    chassis.waitUntilDone();
    chassis.arcade(127, 0);
    pros::delay(400);
    toggleDriveSequence();
}

// void right3pin() {
//     // Align robot to goal
//     align_pose(270.0);

//     // Move to goal
//     chassis.moveToPoint(0, 0, 1000);
//     chassis.waitUntilDone();

//     // Open claw
//     claw.open();
// }

void auton2() {
    chassis.setPose(0, 0, 0);

    // Toggle twice using the skills opening.
    chassis.arcade(0, 100);
    pros::delay(240);
    chassis.arcade(0, 0);
    intake.spinOutward();
    pros::delay(250);
    intake.stop();

    chassis.arcade(0, 100);
    pros::delay(180);
    chassis.arcade(0, 0);
    intake.spinOutward();
    pros::delay(200);
    intake.stop();
    pros::delay(200);

    // Turn and score the first pin.
    chassis.turnToHeading(-20, 1000, {}, false);
    chassis.arcade(100, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    lift.unfold(wrist);
    pros::delay(700);
    lift.stop();
    chassis.arcade(70, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    lift.manual(-40);
    pros::delay(300);
    lift.stop();
    claw.open();
    pros::delay(200);
    chassis.arcade(0, 0);
    pros::delay(300); // Shorter release pause than the skills opening's 1000 ms.
    return;
    
    // Back away while lowering the lift.
    chassis.moveToPose(-5, 8, 0, 1000, {.forwards = false});
    lift.manual(-40);
    pros::delay(100);
    lift.stop();
    chassis.waitUntilDone();

    
    // Pick up the first stack; monitor the sensor while driving.
    chassis.turnToPoint(-34.224, 28.844, 1500, {
        .maxSpeed = 75, .minSpeed = 20, .earlyExitRange = 3
    });
    chassis.waitUntilDone();
    chassis.moveToPoint(-34.224, 28.844, 10000, {.maxSpeed = 45});
    waitForAutoClaw();

    // Raise the stack while turning to the goal.
    chassis.turnToHeading(72.5, 3000, {.maxSpeed = 75});
    lift.manual(90);
    pros::delay(400);
    lift.stop();
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    chassis.waitUntilDone();
    // go into the goal
    chassis.arcade(100, 0);
    pros::delay(800);
    chassis.arcade(0, 0);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    lift.manual(-40);
    pros::delay(200);
    claw.open();
    lift.stop();

    // Back away and prepare to pick up the second stack.
    chassis.moveToPoint(-41, 29, 10000, {.forwards = false, .maxSpeed = 60}, false);
    chassis.turnToPoint(-21.494, 48.507, 1500, {
        .maxSpeed = 60, .minSpeed = 20, .earlyExitRange = 3
    });
    lift.manual(-40);
    pros::delay(800);
    lift.stop();
    chassis.waitUntilDone();
    chassis.moveToPoint(-21.494, 48.507, 10000, {.maxSpeed = 60, .minSpeed = 20});
    waitForAutoClaw();

    // Raise and score the second stack.
    chassis.turnToHeading(160, 3000, {.maxSpeed = 75});
    lift.manual(57);
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    chassis.waitUntilDone();
    lift.stop();
    chassis.arcade(100, 0);
    pros::delay(800);
    chassis.arcade(30, 0);
    lift.manual(-80);
    pros::delay(250);
    claw.open();
    lift.stop();
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    chassis.arcade(0, 0);

    // Approach the matchloader using the skills route.
    pros::lcd::print(5, "Auton2: last move -23,69");
    chassis.moveToPose(-23, 69, 180, 10000, {.forwards = false, .maxSpeed = 50});
    // match_load_ready();
    chassis.waitUntilDone();

    // chassis.turnToHeading(265, 3000, {.maxSpeed = 50}, false);
    // chassis.moveToPoint(-17.698, 68.710, 10000, {.forwards = false, .maxSpeed = 50}, false);
    // chassis.moveToPoint(-8.5, 69.5, 10000, {.forwards = false, .maxSpeed = 25}, false);
    // chassis.arcade(0, 0);

    // // Use the same height preset as the driver Up button.
    // lift.moveToMatchloadHeight();
    // while (lift.isAuto()) {
    //     lift.update();
    //     pros::delay(5);
    // }
    // if (!lift.atTarget()) return;

    // // Nudge farther backward into the matchloader, then stop driving.
    // chassis.arcade(-40, 0);
    // pros::delay(300);
    // chassis.arcade(0, 0);

    // // Load and secure the stack using the skills intake sequence.
    // claw.open();
    // intake.spinInward();
    // pros::delay(2500);
    // intake.stop();
    // pros::delay(500);
    // claw.close();
    // chassis.arcade(100, 0);
    // pros::delay(328);
    // chassis.arcade(0, 0);
    // lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);
    // lift.unfold(wrist, 200);
    // pros::delay(200);
    // lift.stop();
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
    
    // First-stack target shifted 0.125 inch left relative to the approach.
    chassis.turnToPoint(-2.261, 7.791, 1500, {.maxSpeed = 50});
    chassis.waitUntilDone();


    // Drive to the same target while checking the distance sensor.
    chassis.moveToPoint(-2.261, 7.791, 10000, {
        .maxSpeed = 75
    });
    waitForAutoClaw();
    
    
    // Close only once per move; continue to the next auton step when motion ends.
    
    pros::delay(200);
    
    // turn to the goal
    chassis.turnToHeading(85, 3000, {.maxSpeed = 75});
    lift.manual(90);
    pros::delay(600);
    lift.stop();
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
    chassis.moveToPoint(-1, 7, 10000, {.forwards = false, .maxSpeed = 75}, false);
    
    // Face the stack from the current pose before driving directly toward it.
    chassis.turnToPoint(13.6, 28.7, 3000, {.maxSpeed = 75}, false);
    // Approach the stack at 40 degrees; a smaller lead reduces the approach curve.
    chassis.moveToPose(13.6, 28.7, 34, 10000, {.lead = 0.25, .maxSpeed = 75});
    waitForAutoClaw();

    // Turn to face the goal
    chassis.turnToHeading(173.82, 3000, {.maxSpeed = 75});
    lift.manual(80);
    pros::delay(800);
    lift.stop();
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
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);

    // Small forward nudge before the chained toggle approach.
    chassis.arcade(-70, 0);
    pros::delay(100);

    // Lock the left side and hand off without waiting to settle.
    chassis.swingToHeading(290, lemlib::DriveSide::LEFT, 3000, {
        .direction = lemlib::AngularDirection::CW_CLOCKWISE,
        .maxSpeed = 127, .minSpeed = 70, .earlyExitRange = 8
    }, false);
    chassis.arcade(-127, 0);
    pros::delay(300);
    chassis.swingToHeading(320, lemlib::DriveSide::LEFT, 3000, {
        .direction = lemlib::AngularDirection::CW_CLOCKWISE,
        .maxSpeed = 127, .minSpeed = 127, .earlyExitRange = 8
    }, false);
    toggleDriveSequence();

    return;
    // Assume scoring is 5 inches forward from (13.6, 28.7) at 173.82 degrees.
    // Back up 10 inches from that estimated scoring position.
    // lift.manual(-100);
    // chassis.moveToPoint(13.062, 33.671, 10000, {
    //     .forwards = false, .maxSpeed = 127, .minSpeed = 60, .earlyExitRange = 2
    // });
    // pros::delay(250);
    // lift.stop();
    
    // // Chain the turn into the forward move without waiting to settle.
    // chassis.turnToHeading(110, 3000, {
    //     .maxSpeed = 127, .minSpeed = 60, .earlyExitRange = 8
    // });
    
    
    // // Finish and settle at the point before starting the final turn.
    // chassis.moveToPoint(41, 23, 10000, {.maxSpeed = 127}, false);
    // chassis.turnToHeading(0, 3000, {.maxSpeed = 127}, false);
    // // Drive 35 inches forward at 150 degrees.
    // chassis.moveToPoint(30.562, 3.360, 10000, {.maxSpeed = 127}, false);

    // return;
}

