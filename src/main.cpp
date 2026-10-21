#include "main.h"
#include "lemlib/api.hpp"

#include "autons.hpp"
#include "constants.hpp"

#include "lift.hpp"
#include "claw.hpp"
#include "pros/rtos.hpp"
#include "wrist.hpp"
#include "intake.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

Claw claw('B');
Wrist wrist('A');

Lift lift(-9, 2, 10);

Intake intake(1);

pros::Distance distanceSensor(7);


// drivetrain motors
pros::MotorGroup leftMotors({-13, -11, -14});
pros::MotorGroup rightMotors({16, 12, 15});


// sensors
pros::Imu imu(17);

pros::Rotation horizontalEnc(-18);
pros::Rotation verticalEnc(19);


lemlib::TrackingWheel horizontal(
    &horizontalEnc,
    lemlib::Omniwheel::NEW_2,
    -0.6
);

lemlib::TrackingWheel vertical(
    &verticalEnc,
    lemlib::Omniwheel::NEW_2,
    -1.75
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

    // calibrate the lift rotation sensor
    lift.rotationSensor.reset_position();
    //
    claw.open();
    pros::delay(3000);
    claw.close();

    pros::delay(200);
    lift.foldInSize(wrist);
    // lift.reset();
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

            pros::lcd::print(
                4,
                "Distance: %d mm",
                distanceSensor.get_distance()
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
//     chassis.setPose(0, 0, 0);
//     //chassis.moveToPoint(0, 10, 3000, {.maxSpeed = 50});
    // chassis.moveToPose(0, 24, 90, 3000);
    // chassis.waitUntilDone();
    // auton1();
    // lift.foldInSize(wrist);

    // delete this after testing
    pros::delay(3000);

    auton1();    
}

// void opcontrol() {s

//     while (true) {

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

//         // Button A: Move lift to 15 degrees
//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
//             lift.moveAngle(20);
//         }
        
//         // Button Y: Move lift to 10 degrees
//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
//             lift.moveAngle(10);
//         }


//         // Button B: Check if lift is at target
//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
//             if (lift.atTarget()) {
//                 controller.rumble(".");
//             }
//         }

//         // Constantly update lift movement
//         lift.update();

//         pros::delay(10);
//     }
// }


enum class PickupState {
    IDLE,
    PREPARING,
    READY,
    LOWERING,
    WAITING,
    RAISING
};

PickupState pickupState = PickupState::IDLE;
unsigned long int pickupWaitStart = 0;

void opcontrol() {
    autonomous();
    pros::delay(1000);

    // op control setup
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);

    bool outtakeWasHeld = false;
    bool autoGrabArmed = true;
    bool distanceClearTiming = false;
    std::uint32_t distanceClearStart = 0;

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

            // wrist.moveDOWN();

            // lift.moveAngle(16);

            // pickupState = PickupState::PREPARING;
        }


        // // =========================================
        // // B = PICK UP OBJECT
        // // =========================================

        // if (controller.get_digital_new_press(
        //         pros::E_CONTROLLER_DIGITAL_B)) {

        //     if (pickupState == PickupState::READY) {
        //         claw.close();   
        //         intake.stop();
        //         // change pickup state to lowering
        //         pickupState = PickupState::LOWERING;
        //     }
        // }


        // // =========================================
        // // PICKUP STATE MACHINE
        // // =========================================

        // switch (pickupState) {

        //     case PickupState::IDLE:

        //         break;


        //     // Lift goes up first
        //     case PickupState::PREPARING:

        //         if (lift.atTarget()) {

        //             claw.open();

        //             lift.moveAngle(10);

        //             // We are not waiting for lift to reach 10 deg
        //             // driver can close claw earlier if they want to
        //             pickupState = PickupState::READY;
        //         }
        //         // else{
        //         //     pros::lcd::print(
        //         //         4,
        //         //         "Prep: %.2f Err: %.2f",
        //         //         lift.getAngle(),
        //         //         15.0 - lift.getAngle()
        //         //     );
        //         // }

        //         break;


        //     // Robot stays here until driver presses B
        //     case PickupState::READY:

        //         break;


        //     // Lift goes down to grab position
        //     case PickupState::LOWERING:

        //         if (lift.atTarget()) {
        //             pickupWaitStart = pros::millis();
        //             pickupState = PickupState::WAITING;
        //         }

        //         break;


        //     // Allow the mechanism to settle while driver control keeps running.
        //     case PickupState::WAITING:

        //         if (pros::millis() - pickupWaitStart >= 200U) {

        //             wrist.moveUP();

        //             lift.moveAngle(20);

        //             pickupState = PickupState::RAISING;
        //         }

        //         break;


        //     // Lift returns back up
        //     case PickupState::RAISING:

        //         if (lift.atTarget()) {

        //             pickupState = PickupState::IDLE;
        //         }

        //         break;
        // }


        // =========================================
        // MANUAL WRIST
        // L1
        // =========================================

        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_L1)) {

            wrist.toggle();
        }

        // CLAW: L2 overrides auto-grab, including when opening to score.
        const bool manualClawPressed = controller.get_digital_new_press(
            pros::E_CONTROLLER_DIGITAL_L2);
        const int distance = distanceSensor.get();
        const bool validDistance = distance != PROS_ERR && distance > 0;

        if (manualClawPressed) {

            if (claw.isExtended) {
                claw.close();
            }
            else {
                claw.open();
            }

            autoGrabArmed = false;
            distanceClearTiming = false;
        } else if (!validDistance || !claw.isExtended) {

            distanceClearTiming = false;
        } else if (distance <= 23) {

            distanceClearTiming = false;
            if (autoGrabArmed) {
                claw.close();
                autoGrabArmed = false;
            }
        } else if (!autoGrabArmed) {

            // Re-arm only after the open claw has been clear for 200 ms.
            // Use elapsed time so driving and manual controls keep updating.
            const std::uint32_t now = pros::millis();
            if (!distanceClearTiming) {
                distanceClearStart = now;
                distanceClearTiming = true;
            } else if (now - distanceClearStart >= 200U) {
                autoGrabArmed = true;
                distanceClearTiming = false;
            }
        }

        // OUTTAKE: hold Y to run, release Y to stop.
        const bool outtakeHeld = controller.get_digital(
            pros::E_CONTROLLER_DIGITAL_Y);

        if (outtakeHeld) {

            intake.spinOutward();

        } else if (outtakeWasHeld) {

            intake.stop();
        }

        outtakeWasHeld = outtakeHeld;

        if (controller.get_digital(
                pros::E_CONTROLLER_DIGITAL_X)) {

            intake.stop();
        }

        // =========================================
        // MANUAL LIFT
        // =========================================

        if (controller.get_digital(
        pros::E_CONTROLLER_DIGITAL_R1)) {

            pickupState = PickupState::IDLE;

            lift.manual(127);
        }

        else if (controller.get_digital(
                    pros::E_CONTROLLER_DIGITAL_R2)) {

            pickupState = PickupState::IDLE;

            lift.manual(-127);
        }

        else {

            if (lift.isAuto()) {

                lift.update();
            }

            else {

                lift.stop();
            }
        }


        pros::delay(5);
    }
}

