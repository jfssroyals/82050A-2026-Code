#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "constants.hpp"
#include "lift.hpp"
#include "claw.hpp"
#include "claw_motor.hpp"
#include "intake.hpp"
#include "control.hpp"
#include "autons.hpp"

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
                              8 // horizontal drift is 2. If we had traction wheels, it would have been 8
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
    // bar.reset();
    lift.reset();
    claw.close();
    // print position to brain screen
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // delay to save resources
            pros::delay(20);
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
ASSET(path2_txt); // '.' replaced with "_" to make c++ happy


void autonomous() {
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // move 48" forwards
    // chassis.moveToPoint(5, 30, 10000);
    // chassis.moveToPose(5, 30, 12.77, 10000);
    // chassis.waitUntilDone();
    // pros::delay(1000000);

    // 2, 22
    //test_follow();
    // chassis.follow(path2_txt,15,4000);
    fourPinBlue();
    pros::delay(100000);
    // chassis.setPose(0, 0, 0);
    
    // chassis.arcade(127, 0);
    // pros::delay(300);
    // chassis.arcade(-127, 0);
    // pros::delay(150);
    // chassis.arcade(100, 0);
    // pros::delay(450);
    // chassis.arcade(0, 0);
    // chassis.moveToPose(22, -12.3, 270, 1000, {.forwards = false, .lead = 0.6});  
    // pros::delay(100);
    
    // chassis.setPose(0, 0, 0);
    // chassis.moveToPose(24, 24, 0, 500, {.lead = 0.5});

    // chassis.arcade(-127, 0);
    // chassis.moveToPoint(0, -3, 300, {.minSpeed = 127});
    // chassis.moveToPoint(0, 3, 500, {.minSpeed = 127});
   
    // // 1. Move forward 12 inches
    // chassis.moveToPoint(0, 12, 1000);
    // chassis.waitUntilDone(); // Wait for forward movement to complete

    // // 2. Turn 90 degrees
    // chassis.turnToHeading(90, 1000);
    // chassis.waitUntilDone(); // Wait for turn to complete!

    // 3. Smoothly drive back to (0,0) facing 0 degrees
    // chassis.turnToHeading(0, 1000, {.direction = AngularDirection::CCW_COUNTERCLOCKWISE});
    // chassis.waitUntilDone();

    // chassis.moveToPose(5, 24, 90, 2200, {.lead = 0.5});
    // chassis.waitUntilDone();
}


void opcontrol() {
    autonomous();
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
            if (bar.isAtBack()){
                bar.moveToFront();
            }
            else {
                bar.moveToBack();
            }
        }

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) { 
            claw.toggle();
            pros::delay(450);
            leftMotors.move(25);  // Power range: -127 to 127
            rightMotors.move(25);
            pros::delay(10);
            // pros::lcd::print(5,  "Boolean: %.2f", claw.isopen());
            if (claw.isopen() == true){
                bar.motor.move(-60);
                pros::delay(200);
                bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                bar.motor.brake();
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