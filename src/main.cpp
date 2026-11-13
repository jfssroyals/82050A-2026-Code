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
#include "leds.hpp"

pros::Controller controller(pros::E_CONTROLLER_MASTER);

Claw claw('B');
Wrist wrist('A');

AutonSelector selector;
bool autonexecuted = false;

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

void telemetryTask() {

    while (true) {

        // Telemetry only owns the screen after confirmation.
        if (selector.isAutonConfirmed()) {

            // Read robot data.
            const lemlib::Pose pose = chassis.getPose();

            const double liftAngle = lift.getAngle();
            const int distance = distanceSensor.get_distance();
            const int horizontalPosition = horizontalEnc.get_position();

            // Prepare display strings.
            char line1[40];
            char line2[32];
            char line3[32];
            char line4[32];
            char line5[40];

            std::snprintf(line1, sizeof(line1),
                          "X: %.1f Y: %.1f", pose.x, pose.y);

            std::snprintf(line2, sizeof(line2),
                          "HEADING: %.1f", pose.theta);

            std::snprintf(line3, sizeof(line3),
                          "LIFT: %.1f", liftAngle);

            std::snprintf(line4, sizeof(line4),
                          "DIST: %d MM", distance);

            std::snprintf(line5, sizeof(line5),
                          "H-ENC: %ld",
                          static_cast<long>(horizontalPosition));

            // Redraw dashboard.
            ui::clear();

            ui::drawText(10, 15, "ROBOT TELEMETRY",
                         pros::Color::green, 2);

            ui::drawText(10, 65, line1,
                         pros::Color::white, 2);

            ui::drawText(10, 110, line2,
                         pros::Color::white, 2);

            ui::drawText(10, 155, line3,
                         pros::Color::white, 2);

            ui::drawText(10, 200, line4,
                         pros::Color::white, 2);

            // Encoder line is omitted here to keep the
            // dashboard within the portrait display.
        }

        pros::delay(100);
    }
}

// initialize function. Runs on program startup
void initialize() {    
    controller.rumble(".."); // rumble to indicate that the robot is initializing
    pros::lcd::initialize(); // initialize brain screen
    startLeds(); // start the LED pattern
    chassis.calibrate(); // calibrate sensors
    // bar.reset();
    lift.rotationSensor.reset_position();

    // automatically sets the selector in portrait mode
    selector.setOrientation(AutonSelector::Orientation::Portrait);
    selector.initialize();    

    static pros::Task* telemetryTaskHandle = nullptr;

    if (telemetryTaskHandle == nullptr) {
        telemetryTaskHandle = new pros::Task(telemetryTask);
    }

    lift.reset();
    
    claw.open();
    pros::delay(3000);
    claw.close();

    pros::delay(200);
    lift.foldInSize(wrist);

}


/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

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

        default:
            pros::lcd::print(5, "Invalid auton ID!");
            return;
    }

    chassis.waitUntilDone();

    pros::lcd::print(5, "Auton completed!");
    controller.rumble("..");
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

    std::uint32_t outtakeRemainingMs = 0;
    std::uint32_t lastOuttakeUpdate = pros::millis();
    std::uint32_t lastOuttakeRumble = 0;

    AutoGrabState autoGrabState = AutoGrabState::ARMED;
    std::uint32_t distanceClearStart = 0;

    // =========================================
    // MAIN OPERATOR CONTROL LOOP
    // =========================================

    while (true) {

        // =========================================
        // OPTIONAL AUTONOMOUS TESTING
        // =========================================

        if (!autonexecuted) {

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

        // DRIVE
        int leftY = controller.get_analog(
            pros::E_CONTROLLER_ANALOG_LEFT_Y
        );

        int rightX = controller.get_analog(
            pros::E_CONTROLLER_ANALOG_RIGHT_X
        );

        chassis.arcade(leftY, rightX);


        // =========================================
        // A = INTAKE
        // =========================================

        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_A)) {

            intake.spinInward();
        }


        // =========================================
        // L1 = WRIST TOGGLE
        // =========================================

        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_L1)) {

            wrist.toggle();
        }


        // =========================================
        // L2 = MANUAL CLAW
        // =========================================

        const bool manualClawPressed =
            controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_L2
            );

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
            intake.stop();
        }


        // =========================================
        // LIFT CONTROL
        // =========================================

        // UP = preset matchload height.
        if (controller.get_digital_new_press(
                pros::E_CONTROLLER_DIGITAL_UP)) {

            lift.moveToMatchloadHeight();
        }

        // R1 = manual lift upward.
        if (controller.get_digital(
                pros::E_CONTROLLER_DIGITAL_R1)) {

            lift.manual(127);

        }

        // R2 = manual lift downward.
        else if (controller.get_digital(
                     pros::E_CONTROLLER_DIGITAL_R2)) {

            lift.manual(-70);

        }

        // Otherwise, update the automatic lift controller or stop.
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

