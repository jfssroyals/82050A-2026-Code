#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "autons.hpp"
#include "constants.hpp"
#include "lift.hpp"
#include "claw.hpp"
#include "claw_motor.hpp"
#include "intake.hpp"
#include "control.hpp"
#include "pros/misc.h"


// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// subsystems ports
// claw
Claw claw('A');

//Bellcrank Piston
pros::adi::DigitalOut intakePiston('H');

// create lift
Lift lift(-9, 2);


// Intake 
Intake intake(1);

// ------------------------------ //

// motor groups
pros::MotorGroup leftMotors({-10, -8, -7}); // left motor group - ports 3 (reversed), 4, 5 (reversed)
pros::MotorGroup rightMotors({20, 5, 4}); // right motor group - ports 6, 7, 9 (reversed)


// Inertial Sensor on port 6
pros::Imu imu(6);

// horizontal tracking wheel encoder Rotation sensor, port 20
pros::Rotation horizontalEnc(-12);
// vertical tracking wheel 
pros::Rotation verticalEnc(-19);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, -4.917);

lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, -1);
// ------------------------------ //

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10.06, // UPDATED
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
                            &vertical, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     5, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  4, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);
// void screenTask(void*) {
//     while (true) {
//         pros::lcd::print(0, "X: %.2f", chassis.getPose().x);
//         pros::lcd::print(1, "Y: %.2f", chassis.getPose().y);
//         pros::lcd::print(2, "Theta: %.2f", chassis.getPose().theta);
//         lift.LiftVoltage();
//         // printf("X: %.2f Y: %.2f\n", chassis.getPose().x, chassis.getPose().y);
//         // printf("Theta: %.2f\n", chassis.getPose().theta);
//         pros::delay(50);
//     }
// }

void initialize() {
    controller.rumble(".."); // rumble to indicate that the robot is initializing
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    // bar.reset();
    lift.reset();
    claw.open();

    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // delay to save resources
            pros::delay(100);
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


void opcontrol() {
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
    
        // -------------------------------------------------------------
        //LIFT CODE
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
            lift.New_LiftControl(80);
        }
        if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
            lift.New_LiftControl(-80);
        }
        // lift.updateComplexLift();
        // --------------------------------------------------------------

        // --------------------------------------------------------
        // Intake Code
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)){
            intake.spinInward();
        }
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)){
            intake.spinOutward();
        }
  

        // if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)){
        //         //bell crank piston up
        //         intakePiston.set_value(false);
        // }
        // if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        //         //bell crank piston down
        //         intakePiston.set_value(true);
        // }
        
        pros::delay(5);
    
    }
}
