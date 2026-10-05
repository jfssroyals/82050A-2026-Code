#include "main.h"
#include "lemlib/api.hpp"

#include "autons.hpp"
#include "constants.hpp"

#include "lift.hpp"
#include "claw.hpp"
#include "wrist.hpp"
#include "intake.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

Claw claw('B');
Wrist wrist('A');

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


// initialize function. Runs on program startup
void initialize() {

    pros::lcd::initialize();

    chassis.calibrate();

    lift.reset();

    pros::Task screen_task([&]() {

        while (true) {

            // robot position
            pros::lcd::print(
                0,
                "X: %.2f",
                chassis.getPose().x
            );

            pros::lcd::print(
                1,
                "Y: %.2f",
                chassis.getPose().y
            );

            pros::lcd::print(
                2,
                "Theta: %.2f",
                chassis.getPose().theta
            );

            // lift rotation sensor angle
            pros::lcd::print(
                3,
                "Lift Angle: %.2f",
                lift.getAngle()
            );

            pros::delay(100);
        }
    });
}


void disabled() {
}


void competition_initialize() {
}


void autonomous() {
}


// void opcontrol() {

//     while (true) {

//         // DRIVE
//         int leftY = controller.get_analog(
//             pros::E_CONTROLLER_ANALOG_LEFT_Y
//         );

//         int rightX = controller.get_analog(
//             pros::E_CONTROLLER_ANALOG_RIGHT_X
//         );

//         chassis.arcade(leftY, rightX);


//         // INTAKE
//         // A = inward
//         if (controller.get_digital_new_press(
//                 pros::E_CONTROLLER_DIGITAL_A)) {

//             intake.spinInward();
//         }


//         // Y = outward
//         if (controller.get_digital_new_press(
//                 pros::E_CONTROLLER_DIGITAL_Y)) {

//             intake.stop();
//         }



//         // WRIST
//         // L1 = toggle up/down
//         if (controller.get_digital_new_press(
//                 pros::E_CONTROLLER_DIGITAL_L1)) {

//             if (wrist.isUp) {
//                 wrist.moveDOWN();
//             }
//             else {
//                 wrist.moveUP();
//             }
//         }


//         // CLAW
//         // L2 = toggle open/close
//         if (controller.get_digital_new_press(
//                 pros::E_CONTROLLER_DIGITAL_L2)) {

//             if (claw.isExtended) {
//                 claw.close();
//             }
//             else {
//                 claw.open();
//             }
//         }


//         // LIFT
//         // R1 = up
//         if (controller.get_digital(
//                 pros::E_CONTROLLER_DIGITAL_R1)) {

//             lift.manual(120);
//         }

//         // R2 = down
//         else if (controller.get_digital(
//                      pros::E_CONTROLLER_DIGITAL_R2)) {

//             lift.manual(-40);
//         }

//         // neither pressed = stop
//         else {

//             lift.stop();
//         }


//         pros::delay(10);
//     }
// }

enum class PickupState {
    IDLE,
    PREPARING,
    READY,
    LOWERING,
    RAISING
};

PickupState pickupState = PickupState::IDLE;

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


        // =========================================
        // A = PREPARE FOR INTAKE
        // =========================================

        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_A)) {

            intake.spinInward();
 
            lift.moveAngle(15);

            pickupState = PickupState::PREPARING;
        }


        // =========================================
        // B = PICK UP OBJECT
        // =========================================

        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_B)) {

            if (pickupState == PickupState::READY) {

                lift.moveAngle(11);

                pickupState = PickupState::LOWERING;
            }
        }


        // =========================================
        // PICKUP STATE MACHINE
        // =========================================

        switch (pickupState) {

            case PickupState::IDLE:

                break;


            // Lift goes up first
            case PickupState::PREPARING:

                if (lift.atTarget()) {

                    claw.open();

                    wrist.moveDOWN();

                    pickupState = PickupState::READY;
                }
                // else{
                //     pros::lcd::print(
                //         4,
                //         "Prep: %.2f Err: %.2f",
                //         lift.getAngle(),
                //         15.0 - lift.getAngle()
                //     );
                // }

                break;


            // Robot stays here until driver presses B
            case PickupState::READY:

                break;


            // Lift goes down to grab position
            case PickupState::LOWERING:

                if (lift.atTarget()) {

                    intake.stop();

                    claw.close();

                    wrist.moveUP();

                    lift.moveAngle(15);

                    pickupState = PickupState::RAISING;
                }

                break;


            // Lift returns back up
            case PickupState::RAISING:

                if (lift.atTarget()) {

                    pickupState = PickupState::IDLE;
                }

                break;
        }


        // =========================================
        // MANUAL CLAW
        // L2
        // =========================================

        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_L2)) {

            if (claw.isExtended) {
                claw.close();
            }
            else {
                claw.open();
            }
        }


        // =========================================
        // MANUAL LIFT
        // =========================================

        if (controller.get_digital(
        pros::E_CONTROLLER_DIGITAL_R1)) {

            pickupState = PickupState::IDLE;

            lift.manual(120);
        }

        else if (controller.get_digital(
                    pros::E_CONTROLLER_DIGITAL_R2)) {

            pickupState = PickupState::IDLE;

            lift.manual(-60);
        }

        else {

            if (lift.isAuto()) {

                lift.update();
            }

            else {

                lift.stop();
            }
        }


        pros::delay(10);
    }
}

// void opcontrol() {

//     while (true) {

//         // DRIVE
//         int leftY = controller.get_analog(
//             pros::E_CONTROLLER_ANALOG_LEFT_Y
//         );

//         int rightX = controller.get_analog(
//             pros::E_CONTROLLER_ANALOG_RIGHT_X
//         );

//         chassis.arcade(leftY, rightX);

//         // A = move lift to 15 degrees
//         if (controller.get_digital_new_press(
//                 pros::E_CONTROLLER_DIGITAL_A)) {

//             lift.moveAngle(15);
//         }

//         lift.update();

//         // // keep automatic lift movement running
//         // if (lift.isAuto()) {
//         //     lift.update();
//         // }
//         // else {
//         //     lift.stop();
//         // }


//         pros::delay(10);
//     }
// }
