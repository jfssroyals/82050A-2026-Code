#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "autons.hpp"
#include "constants.hpp"
#include "lift.hpp"
#include "claw.hpp"
#include "claw_motor.hpp"
#include "control.hpp"



// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// claw
Claw claw('A');

// bar
Bar bar(2); // change 6 to your motor port

// create lift
Lift lift(9, 3);

// control
Control control(claw, bar, lift);

// motor groups
pros::MotorGroup leftMotors({6, 5, 4}, pros::MotorGearset::blue); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({-10, -8, -7}, pros::MotorGearset::blue); // right motor group - ports 6, 7, 9 (reversed)

// Inertial Sensor on port 10
pros::Imu imu(15);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20
pros::Rotation horizontalEnc(20);
// vertical tracking wheel encoder. Rotation sensor
pros::Rotation verticalEnc(-12);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5.75);
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, -2.5);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

lemlib::ControllerSettings linearController(
    constants::linear_kP,
    constants::linear_kI,
    constants::linear_kD,
    constants::linear_antiWindup,
    constants::linear_smallError,
    constants::linear_smallTimeout,
    constants::linear_largeError,
    constants::linear_largeTimeout,
    constants::linear_slew
);

lemlib::ControllerSettings angularController(
    constants::angular_kP,
    constants::angular_kI,
    constants::angular_kD,
    constants::angular_antiWindup,
    constants::angular_smallError,
    constants::angular_smallTimeout,
    constants::angular_largeError,
    constants::angular_largeTimeout,
    constants::angular_slew
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);



void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    bar.reset();
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
// ASSET(example_txt); // '.' replaced with "_" to make c++ happy

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */

void autonomous() {
    //add autonomous selector
}

void opcontrol() {
    // controller
    // loop to continuously update motors
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
        
        lift.updateLiftController(controller);

        // L1 + L2 together = toggle claw mode
    if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        control.toggleClawMode();
    }

    // L1 = front action
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {

        control.frontSideAction();
    }

    // L2 = back action
    else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {

        control.backSideAction();
    }
        
        else {
            continue;
            pros::delay(10);
        }
        
        // delay to save resources
        pros::delay(10);
    }
}

// void opcontrol() {
//     // Start with the claw open and ready to grab
//     claw.open(); 
    
//     while (true) {
//         // ==========================================
//         // 1. DRIVETRAIN & LIFT CONTROL (KEPT EXACTLY THE SAME)
//         // ==========================================
//         int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
//         int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
//         chassis.arcade(leftY, rightX);
        
//         lift.updateLiftController(controller);

//         // ==========================================
//         // 2. NEW MODE SWITCHING (DOWN ARROW)
//         // ==========================================
//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
//             if (clawMode == PICKUP) {
//                 clawMode = DROP;
//                 claw.close(); // Keep cup secure when switching to drop mode
//             } else {
//                 clawMode = PICKUP;
//                 claw.open();  // Reset claw open to ready up for a pickup
//             }
//         }

//         // ==========================================
//         // 3. NEW ACTION LOGIC (L1 & L2 DEPENDING ON MODE)
//         // ==========================================
//         if (clawMode == PICKUP) {
//             // === PICKUP MODE ===
            
//             // L1: Pickup in FRONT (Starts open, then closes)
//             if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
//                 bar.moveToFront(); 
//                 claw.open();       
//                 pros::delay(200);  
//                 claw.close();      
//             }

//             // L2: Pickup from BEHIND (Rotates 180, then closes)
//             if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
//                 bar.moveToBack();  
//                 claw.open();       
//                 pros::delay(350);  // Wait for the heavy physical bar to flip
//                 claw.close();      
//             }

//         } else if (clawMode == DROP) {
//             // === DROP MODE ===
            
//             // L1: Drop in FRONT (Stays closed until it faces front, then opens)
//             if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {
//                 bar.moveToFront(); 
//                 pros::delay(200);
//                 claw.open();       
//             }

//             // L2: Drop BEHIND (Stays closed until it rotates 180, then opens)
//             if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
//                 bar.moveToBack();  
//                 pros::delay(350);  // Wait for rotation so it doesn't drop early
//                 claw.open();       
//             }
//         }
        
//         // Background task delay (KEPT EXACTLY THE SAME)
//         pros::delay(10);
//     }
// }