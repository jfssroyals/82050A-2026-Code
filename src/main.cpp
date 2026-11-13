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
pros::Distance objectSensor(4);

// Only detect objects strictly closer than 7 cm (70 mm).
constexpr int objectDetectionMaxMm = 70;
constexpr std::uint32_t objectDetectionTimeMs = 1000;


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
    -0.6//-0.194
);

lemlib::TrackingWheel vertical(
    &verticalEnc,
    lemlib::Omniwheel::NEW_2,
    1.25
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

    // calibrate the lift rotation sensor
    // lift.rotationSensor.reset_position();
    // claw.open();
    // pros::delay(3000);
    // claw.close();

    // pros::delay(200);
    // lift.foldInSize(wrist);
    chassis.calibrate();

    // Use one task for all LCD writes to avoid competing display updates.
    static pros::Task object_screen_task([]() {
        bool trackingObject = false;
        std::uint32_t objectDetectedSince = 0;

        while (true) {
            const int objectDistance = objectSensor.get();
            if (objectDistance == PROS_ERR) {
                pros::lcd::set_text(4, "Port 4 distance: error");
            } else {
                pros::lcd::print(4, "Port 4 distance: %d mm", objectDistance);
            }
            const bool objectPresent = objectDistance != PROS_ERR
                && objectDistance > 0 && objectDistance < objectDetectionMaxMm;
            const std::uint32_t now = pros::millis();

            if (!objectPresent) {
                trackingObject = false;
                pros::lcd::set_text(5, "No object detected");
            } else {
                if (!trackingObject) {
                    trackingObject = true;
                    objectDetectedSince = now;
                }
                if (now - objectDetectedSince > objectDetectionTimeMs) {
                    pros::lcd::set_text(5, "Object detected");
                } else {
                    pros::lcd::set_text(5, "No object detected");
                }
            }

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
                6,
                "Port 7 distance: %d mm",
                distanceSensor.get_distance()
            );

            pros::delay(50);
        }
    });
}


void disabled() {
    cancel_intake_pick_up();
}


void competition_initialize() {
}


void autonomous() {
    // chassis.setPose(0, 0, 0);
    // delete this after testing
    //autonMatchloadTest(); // Standalone matchloader test.
    //autonskills();
    // auton1();
    //auton2();
    match_load_ready();
}

enum class AutoGrabState {
    ARMED,
    DISARMED,
    WAITING_FOR_CLEAR
};


void opcontrol() {
    //autonomous();
    // pros::delay(1000);

    // op control setup
    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);

    std::uint32_t outtakeRemainingMs = 0;
    std::uint32_t lastOuttakeUpdate = pros::millis();
    std::uint32_t lastOuttakeRumble = 0;
    AutoGrabState autoGrabState = AutoGrabState::ARMED;
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
        // A = sensor-triggered intake pickup
        // =========================================

        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_A)) {

            outtakeRemainingMs = 0;
            start_intake_pick_up();

        }


        // =========================================
        // MANUAL WRIST
        // L1
        // =========================================

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_L1)) {

            cancel_intake_pick_up();
            wrist.toggle();
        }

        // CLAW: L2 overrides auto-grab, including when opening to score.
        const bool manualClawPressed = controller.get_digital_new_press(
            pros::E_CONTROLLER_DIGITAL_L2);
        if (manualClawPressed) cancel_intake_pick_up();
        // Let the pickup sequence control the claw while it is running.
        if (intake_pick_up_active()) {
            autoGrabState = AutoGrabState::DISARMED;
        }
        const int distance = distanceSensor.get();
        const bool validDistance = distance != PROS_ERR && distance > 0;
        const std::uint32_t autoGrabNow = pros::millis();

        if (manualClawPressed) {
            if (claw.isExtended) {
                claw.close();
            }
            else {
                claw.open();
            }

            autoGrabState = AutoGrabState::DISARMED;
        } else {
            switch (autoGrabState) {
                case AutoGrabState::ARMED:
                    if (wrist.isUp && validDistance && claw.isExtended
                        && distance <= 60) {
                        claw.close();
                        autoGrabState = AutoGrabState::DISARMED;
                    }
                    break;

                case AutoGrabState::DISARMED:
                    if (wrist.isUp && validDistance && claw.isExtended
                        && distance > 60) {
                        distanceClearStart = autoGrabNow;
                        autoGrabState = AutoGrabState::WAITING_FOR_CLEAR;
                    }
                    break;

                case AutoGrabState::WAITING_FOR_CLEAR:
                    if (!wrist.isUp || !validDistance || !claw.isExtended
                        || distance <= 60) {
                        autoGrabState = AutoGrabState::DISARMED;
                    } else if (autoGrabNow - distanceClearStart >= 200U) {
                        autoGrabState = AutoGrabState::ARMED;
                    }
                    break;
            }
        }

        // OUTTAKE: each Y press adds 500 ms of run time.
        const std::uint32_t outtakeNow = pros::millis();
        const std::uint32_t outtakeElapsed = outtakeNow - lastOuttakeUpdate;
        lastOuttakeUpdate = outtakeNow;
        const bool outtakeWasRunning = outtakeRemainingMs > 0;
        outtakeRemainingMs = outtakeElapsed >= outtakeRemainingMs
            ? 0 : outtakeRemainingMs - outtakeElapsed;

        const bool outtakePressed = controller.get_digital_new_press(
            pros::E_CONTROLLER_DIGITAL_Y);

        if (outtakePressed) {
            cancel_intake_pick_up();
            if (outtakeRemainingMs == 0) {
                controller.rumble(".");
                lastOuttakeRumble = outtakeNow;
            }
            outtakeRemainingMs += 235U;
        }

        if (outtakeRemainingMs > 0) {

            intake.spinOutward();

            if (outtakeNow - lastOuttakeRumble >= 300U) {
                controller.rumble(".");
                lastOuttakeRumble = outtakeNow;
            }

        } else if (outtakeWasRunning) {

            intake.stop();
        }

        if (controller.get_digital(
                pros::E_CONTROLLER_DIGITAL_X)) {

            outtakeRemainingMs = 0;
            cancel_intake_pick_up();
            intake.stop();
        }

        // =========================================
        // MANUAL LIFT
        // =========================================

        // Up starts the preset; the existing update loop keeps driving responsive.
        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_UP)) {
            cancel_intake_pick_up();
            lift.moveToMatchloadHeight();
        }

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {

            cancel_intake_pick_up();
            lift.manual(127);
        }

        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {

            cancel_intake_pick_up();
            lift.manual(-70);
        }

        else if (!intake_pick_up_active()) {

            if (lift.isAuto()) {

                lift.update();
            }

            else {

                lift.stop();
            }
        }


        update_intake_pick_up();
        pros::delay(5);
    }
}
