#include "main.h"
#include "lemlib/api.hpp"

#include "autons.hpp"
#include "constants.hpp"

#include "lift.hpp"
#include "claw.hpp"
#include "pros/rtos.hpp"
#include "wrist.hpp"
#include "intake.hpp"
#include "selector.hpp"
#include "pros/misc.h"
#include "ui_renderer.hpp"
#include <cstdio>
#include <cerrno>
#include "leds.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

Claw claw('B');
Wrist wrist('A');

AutonSelector selector;
bool autonexecuted = false;

Lift lift(-9, 2, 10);

Intake intake(1);

pros::Distance distanceSensor(7);
pros::Distance objectSensor(4);
pros::Distance sideDistanceSensor(20);

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
    1.03
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


void telemetryTask() {

    // Object detection tracking variables.
    bool trackingObject = false;
    std::uint32_t objectDetectedSince = 0;

    while (true) {

        if (selector.isTelemetryVisible()) {

            // ==========================================
            // 1. READ ROBOT DATA
            // ==========================================

            const lemlib::Pose pose = chassis.getPose();

            const double liftAngle = lift.getAngle();

            const int distance = distanceSensor.get_distance();

            const int horizontalPosition =
                horizontalEnc.get_position();

            const int objectDistance = objectSensor.get();
            const int sideDistance = sideDistanceSensor.get();
            const int sideDistanceError = sideDistance == PROS_ERR ? errno : 0;

            // ==========================================
            // 2. OBJECT DETECTION LOGIC
            // ==========================================

            const bool objectPresent =
                objectDistance != PROS_ERR &&
                objectDistance > 0 &&
                objectDistance < constants::objectDetectionMaxMm;

            const std::uint32_t now = pros::millis();

            const char* objectStatus;

            if (!objectPresent) {

                trackingObject = false;
                objectStatus = "No object detected";

            } else {

                if (!trackingObject) {

                    trackingObject = true;
                    objectDetectedSince = now;

                }

                if (now - objectDetectedSince >
                    objectDetectionTimeMs) {

                    objectStatus = "Object detected";

                } else {

                    objectStatus = "Detecting...";

                }
            }

            // ==========================================
            // 3. PREPARE DISPLAY STRINGS
            // ==========================================

            char line1[40];
            char line2[32];
            char line3[32];
            char line4[32];
            char line5[40];
            char line6[40];
            char line7[40];
            char sideDistanceLine[32];

            std::snprintf(
                line1, sizeof(line1),
                "X: %.1f Y: %.1f",
                pose.x, pose.y
            );

            std::snprintf(
                line2, sizeof(line2),
                "HEADING: %.1f",
                pose.theta
            );

            std::snprintf(
                line3, sizeof(line3),
                "LIFT: %.1f",
                liftAngle
            );

            std::snprintf(
                line4, sizeof(line4),
                "DIST: %d MM",
                distance
            );

            std::snprintf(
                line5, sizeof(line5),
                "H-ENC: %ld",
                static_cast<long>(horizontalPosition)
            );

            if (objectDistance == PROS_ERR) {

                std::snprintf(
                    line6, sizeof(line6),
                    "OBJECT SENSOR: ERROR"
                );

            } else {

                std::snprintf(
                    line6, sizeof(line6),
                    "OBJECT DIST: %d MM",
                    objectDistance
                );
            }

            std::snprintf(
                line7, sizeof(line7),
                "STATUS: %s",
                objectStatus
            );

            if (sideDistance == PROS_ERR) {
                const char* reason = sideDistanceError == ENODEV ? "NO SENSOR" : "ERROR";
                std::snprintf(sideDistanceLine, sizeof(sideDistanceLine),
                              "SIDE P20: %s (%d)", reason, sideDistanceError);
            } else if (sideDistance == 9999) {
                std::snprintf(sideDistanceLine, sizeof(sideDistanceLine), "SIDE DIST: NO TARGET");
            } else {
                std::snprintf(sideDistanceLine, sizeof(sideDistanceLine), "SIDE DIST: %d MM", sideDistance);
            }

            // ==========================================
            // 4. DRAW TELEMETRY DASHBOARD
            // ==========================================

            ui::clear();

            ui::drawText(
                10, 15,
                "ROBOT TELEMETRY",
                pros::Color::green, 2
            );

            const char* const rows[] = {
                line1, line2, sideDistanceLine, line3, line4, line5, line6, line7
            };
            const int rowHeight = (ui::height() - 65) / 8;
            int rowY = 65;
            for (const char* row : rows) {
                ui::drawText(10, rowY, row);
                rowY += rowHeight;
            }
        }

        pros::delay(100);
    }
}


// initialize function. Runs on program startup

void initialize() {

    // ==========================================
    // 1. INITIALIZATION
    // ==========================================

    selector.clearConfirmation();

    controller.rumble("..");
-
    pros::lcd::initialize();

    startLeds();

    // ==========================================
    // 2. CALIBRATION
    // ==========================================

    chassis.calibrate();

    lift.rotationSensor.reset_position();

    // ==========================================
    // 3. AUTON SELECTOR
    // ==========================================

    selector.setOrientation(AutonSelector::Orientation::Portrait);

    selector.initialize();

    // ==========================================
    // 4. MECHANISM INITIALIZATION
    // ==========================================

    // lift.reset();

    // claw.open();

    // pros::delay(3000);

    // claw.close();

    pros::delay(200);

    // lift.foldInSize(wrist);

    // ==========================================
    // 5. START TELEMETRY TASK
    // ==========================================

    static pros::Task* telemetryTaskHandle = nullptr;

    if (telemetryTaskHandle == nullptr) {

        telemetryTaskHandle = new pros::Task(telemetryTask);

    }
}



/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

void disabled() {
    cancel_intake_pick_up();
}


ASSET(test_path_txt);

void autonomous() {

    // The competition system has entered autonomous.
    autonexecuted = true;

    const int selectedId = selector.getConfirmedAuton();

    if (selectedId == -1) {
        pros::lcd::print(5, "ERROR: No auton confirmed!");
        controller.rumble("---");
        return;
    }

    pros::lcd::print(5, "Starting auton ID: %d", selectedId);

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
        test_5();
        return;
    }


    switch (selectedId) {

        case 0:
            auton1();
            break;

        case 1:
            auton2();
            break;

        case 2:
            pros::lcd::print(5, "RED 1 not implemented");
            return;

        case 3:
            pros::lcd::print(5, "RED 2 not implemented");
            return;

        case 4:
            autonskills();
            break;
        case 5:
            test_5();
            break;
        default:
            pros::lcd::print(5, "Invalid auton ID!");
            return;
    }

    chassis.waitUntilDone();

    pros::lcd::print(5, "Auton completed!");
    controller.rumble("..");

    match_load_ready();

}





enum class AutoGrabState {
    ARMED,
    DISARMED,
    WAITING_FOR_CLEAR
};



void opcontrol() {

    // =========================================
    // DRIVER CONTROL INITIALIZATION
    // =========================================

    lift.getMotorGroup().set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD);

    bool drivetrainLocked = false;
    leftMotors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);
    rightMotors.set_brake_mode_all(pros::E_MOTOR_BRAKE_COAST);

    std::uint32_t outtakeRemainingMs = 0;
    std::uint32_t lastOuttakeUpdate = pros::millis();
    std::uint32_t lastOuttakeRumble = 0;

    AutoGrabState autoGrabState = AutoGrabState::ARMED;
    std::uint32_t distanceClearStart = 0;

    
    while (true) {

        if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
            drivetrainLocked = !drivetrainLocked;
            const auto brakeMode = drivetrainLocked ? pros::E_MOTOR_BRAKE_HOLD
                                                    : pros::E_MOTOR_BRAKE_COAST;
            leftMotors.set_brake_mode_all(brakeMode);
            rightMotors.set_brake_mode_all(brakeMode);
            if (drivetrainLocked) {
                // Command each drive motor to stay at its current encoder position.
                leftMotors.move_relative(0, 100);
                rightMotors.move_relative(0, 100);
            } else {
                chassis.arcade(0, 0);
            }
            controller.rumble(drivetrainLocked ? "-" : ".");
        }

        // =========================================
        // OPTIONAL AUTONOMOUS TESTING
        // =========================================

        if (!drivetrainLocked && !autonexecuted) {

            if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_DOWN)){
                selector.showPracticeTelemetry();
                chassis.arcade(0, 0);
                test_5();
            }

            // Read the confirmed autonomous ID.
            const int selectedId = selector.getConfirmedAuton();

            // Require BOTH a confirmed selection and a new B press.
            if (selectedId != -1 &&
                controller.get_digital_new_press(
                    pros::E_CONTROLLER_DIGITAL_DOWN)) {

                // Lock autonomous execution.
                autonexecuted = true;

                // Stop manual drivetrain commands before auton.
                chassis.arcade(0, 0);

                pros::lcd::print(5, "Starting auton ID: %d", selectedId);

                switch (selectedId) {

                    case 0:
                        auton1();
                        break;

                    case 1:
                        auton2();
                        break;

                    case 2:
                        pros::lcd::print(5, "RED 1 not implemented");
                        break;

                    case 3:
                        pros::lcd::print(5, "RED 2 not implemented");
                        break;

                    case 4:
                        autonskills();
                        break;
                    
                    case 5:
                        test_5();
                        break;
                    

                    default:
                        pros::lcd::print(5, "Invalid auton ID!");
                        break;
                }

                // Wait for outstanding LemLib movements.
                chassis.waitUntilDone();

                pros::lcd::print(5, "Auton completed!");
                controller.rumble("..");
            }
        }

        // =========================================
        // DRIVER CONTROL
        // This runs even when no auton is selected.
        // =========================================

        if (!drivetrainLocked) {
            const int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            const int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
            chassis.arcade(leftY, rightX);
        }


        // =========================================
        // A = sensor-triggered intake pickup
        // =========================================

        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_B)) {
            claw.open();
            wrist.moveDOWN();
            outtakeRemainingMs = 0;
            start_intake_pick_up();

        }


        // =========================================
        // L1 = WRIST TOGGLE
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

        const bool validDistance =
            distance != PROS_ERR && distance > 0;

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

                    if (wrist.isUp &&
                        validDistance &&
                        claw.isExtended &&
                        distance <= 60) {

                        claw.close();
                        autoGrabState = AutoGrabState::DISARMED;
                    }

                    break;


                case AutoGrabState::DISARMED:

                    if (wrist.isUp &&
                        validDistance &&
                        claw.isExtended &&
                        distance > 60) {

                        distanceClearStart = autoGrabNow;
                        autoGrabState = AutoGrabState::WAITING_FOR_CLEAR;
                    }

                    break;


                case AutoGrabState::WAITING_FOR_CLEAR:

                    if (!wrist.isUp ||
                        !validDistance ||
                        !claw.isExtended ||
                        distance <= 60) {

                        autoGrabState = AutoGrabState::DISARMED;

                    } else if (autoGrabNow - distanceClearStart >= 200U) {

                        autoGrabState = AutoGrabState::ARMED;
                    }

                    break;
            }
        }


        // =========================================
        // Y = OUTTAKE
        // Each press adds 235 ms.
        // =========================================

        const std::uint32_t outtakeNow = pros::millis();

        const std::uint32_t outtakeElapsed =
            outtakeNow - lastOuttakeUpdate;

        lastOuttakeUpdate = outtakeNow;

        const bool outtakeWasRunning = outtakeRemainingMs > 0;

        outtakeRemainingMs =
            outtakeElapsed >= outtakeRemainingMs
                ? 0
                : outtakeRemainingMs - outtakeElapsed;

        const bool outtakePressed =
            controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_Y
            );

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

        // X = cancel outtake.
        if (controller.get_digital(
                pros::E_CONTROLLER_DIGITAL_X)) {

            outtakeRemainingMs = 0;
            cancel_intake_pick_up();
            intake.stop();
        }


        // =========================================
        // LIFT CONTROL
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
