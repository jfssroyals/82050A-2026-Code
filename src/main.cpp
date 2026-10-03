#include "main.h"
#include "lemlib/api.hpp"

#include "autons.hpp"
#include "constants.hpp"

#include "lift.hpp"
#include "claw.hpp"
#include "wrist.hpp"
#include "intake.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

Claw claw('A');
Wrist wrist('B');

Lift lift(-9, 2, 10);

Intake intake(1);

pros::Distance distanceSensor(3);


// drivetrain motors
pros::MotorGroup leftMotors({-13, -11, -14});
pros::MotorGroup rightMotors({16, 12, 15});


// sensors
pros::Imu imu(6);

pros::Rotation horizontalEnc(-12);
pros::Rotation verticalEnc(-19);


lemlib::TrackingWheel horizontal(
    &horizontalEnc,
    lemlib::Omniwheel::NEW_2,
    -4.917
);

lemlib::TrackingWheel vertical(
    &verticalEnc,
    lemlib::Omniwheel::NEW_2,
    -1
);


// drivetrain
lemlib::Drivetrain drivetrain(
    &leftMotors,
    &rightMotors,
    10.06,
    lemlib::Omniwheel::NEW_275,
    450,
    8
);


// PID settings
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


// odometry sensors
lemlib::OdomSensors sensors(
    nullptr,
    &vertical,
    &horizontal,
    nullptr,
    &imu
);


// drive curves
lemlib::ExpoDriveCurve throttleCurve(
    3,
    5,
    1.019
);

lemlib::ExpoDriveCurve steerCurve(
    3,
    4,
    1.019
);


// chassis
lemlib::Chassis chassis(
    drivetrain,
    linearController,
    angularController,
    sensors,
    &throttleCurve,
    &steerCurve
);


void initialize() {

    pros::lcd::initialize();

    chassis.calibrate();

    lift.reset();
}


void disabled() {
}


void competition_initialize() {
}


void autonomous() {
}


void opcontrol() {

    while (true) {

        // DRIVE
        int leftY = controller.get_analog(
            pros::E_CONTROLLER_ANALOG_LEFT_Y
        );

        int rightX = controller.get_analog(
            pros::E_CONTROLLER_ANALOG_RIGHT_X
        );

        chassis.arcade(leftY, rightX);


        // INTAKE
        // A = inward
        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_A)) {

            intake.spinInward();
        }


        // Y = outward
        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_Y)) {

            intake.spinOutward();
        }


        // WRIST
        // L1 = toggle up/down
        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_L1)) {

            if (wrist.isUp) {
                wrist.moveDOWN();
            }
            else {
                wrist.moveUP();
            }
        }


        // CLAW
        // L2 = toggle open/close
        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_L2)) {

            if (claw.isExtended) {
                claw.close();
            }
            else {
                claw.open();
            }
        }


        // LIFT
        // R1 = up
        if (controller.get_digital(
                pros::E_CONTROLLER_DIGITAL_R1)) {

            lift.manual(120);
        }

        // R2 = down
        else if (controller.get_digital(
                     pros::E_CONTROLLER_DIGITAL_R2)) {

            lift.manual(-40);
        }

        // neither pressed = stop
        else {

            lift.stop();
        }


        pros::delay(10);
    }
}