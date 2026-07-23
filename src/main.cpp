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

// motor groups
pros::MotorGroup leftMotors({-6, -5, -4}); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({10, 8, 7}); // right motor group - ports 6, 7, 9 (reversed)

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
    lift.reset();
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


void autonomous() {
    //add autonomous selector
}

void opcontrol() {
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
            claw.toggle();
        }

        lift.updateComplexLift();
        control.update();
        // intake.update();

        // // L1 + L2 together = toggle claw mode
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)) {
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

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R2)) {
            lift.stepStageUp();
            controller.rumble(".");
        } 
        
        else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_R1)) 
        {
            lift.stepStageDown();
            controller.rumble(".");
        }    

        // else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) 
        // {
        //     intake.toggle_state();
        // }

        // // else if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X))
        // // {
        // //     lift.goToHighestStage();
        // // }

        // else if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A))
        // {
        //     intake.toggle_direction();
        // }
        
        // delay to save resources
        pros::delay(10);
    }
}

