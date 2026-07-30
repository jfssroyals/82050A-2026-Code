#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "autons.hpp"
#include "constants.hpp"
#include "lift.hpp"
#include "claw.hpp"
#include "claw_motor.hpp"
#include "intake.hpp"
#include "control.hpp"

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// subsystems ports
// claw
Claw claw('A');

// bar
Bar bar(-3); 

// create lift
Lift lift(-9, 2);

// control
Control control(claw, bar, lift);

// Intake 
// Change the port later 
Intake intake(1);

// ------------------------------ //

// motor groups
pros::MotorGroup leftMotors({-10, -8, -7}); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({20, 5, 4}); // right motor group - ports 6, 7, 9 (reversed)
// Tell PROS that motor index 1 (port 5) has a green cartridge
// leftMotors.set_gearing(pros::MotorGears::green, 1);

// 8,7,5 not working on the drivetrain

// tracking

// Inertial Sensor on port 6
pros::Imu imu(6);

// horizontal tracking wheel encoder Rotation sensor, port 20
pros::Rotation horizontalEnc(-12);

// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, -4.917);

// ------------------------------ //

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              12.1, // 12.1 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 2.75" omnis
                              450, // drivetrain rpm is 450
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
lemlib::OdomSensors sensors(nullptr, // we do not have vertical tracking wheel
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
    lift.reset();
    // pros::Task cd screenTask([&]() {
    //     while (true) {
    //         // print robot location to the brain screen
    //         // pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
    //         // pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
    //         // pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
    //         // pros::lcd::print(3, "Rotation Sensor: %i", horizontalEnc.get_position());

    //         // log position telemetry
    //         lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
    //         // delay to save resources
    //         pros::delay(50);
    //     }
    // });
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

void autonomous() {
    // // set position to x:0, y:0, heading:0
    // chassis.setPose(0, 0, 0);
    // // turn to face heading 90 with a very long timeout
    // chassis.turnToHeading(90, 100000);
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(0, 12.3, 5000);
    chassis.waitUntilDone();
    chassis.turnToHeading(270, 2000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-17, 1552, 5000);  
    chassis.waitUntilDone();
}

void opcontrol() {
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);

        // if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
        //     claw.toggle();
        // }

        lift.updateComplexLift();
          

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            // lift.test_lift();
            // controller.rumble(".");
            lift.stepStageUp();
            controller.rumble(".");
        } 
        
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) 
        {
            controller.rumble(".");
            lift.stepStageDown();
            controller.rumble(".");
        }    

        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) 
        {
            controller.rumble(".");
            lift.stepStageDown();
            controller.rumble(".");
        }  
        
        // for testing
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L2)) {
             
            claw.toggle();
            pros::delay(450);

            if (claw.isOpen == false) {
                leftMotors.move(-25);  // Power range: -127 to 127
                rightMotors.move(-25);
                pros::delay(10);

            }
    
            leftMotors.move(-25);  // Power range: -127 to 127
            rightMotors.move(-25);
            pros::delay(10);
            // pros::lcd::print(5,  "Boolean: %.2f", claw.isopen());

            if (claw.isopen() == true){
                if (lift.isUp == true) {
                bar.motor.move(-60);
                pros::delay(200);
                bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                bar.motor.brake();
                }
            }

        }

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) { 
         
            if (bar.isAtBack()){
                bar.moveToFront();
            }
            else {
                bar.moveToBack();
            }
        }
        // }
        // if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
        //     bar.motor.move(-108);
        //     pros::delay(500);
        //     bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        //     bar.motor.brake();
        // }
        // delay to save resources
        pros::delay(10);
    
    }
}