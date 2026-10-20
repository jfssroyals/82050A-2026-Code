#include "lift.hpp"
#include "wrist.hpp"

#include <algorithm>
#include <cmath>

void Lift::foldInSize(Wrist& wrist) {
    // Starting values: tune on the robot for wrist clearance and starting size.
    constexpr int raiseMs = 400;
    constexpr int wristMs = 300;
    constexpr int lowerMs = 300;

    manual(80);
    pros::delay(raiseMs);
    stop();

    wrist.moveDOWN();
    pros::delay(wristMs);

    manual(-60);
    pros::delay(lowerMs);
    stop();
}

bool Lift::unfold(Wrist& wrist) {
    // Raise for clearance without relying on an uncalibrated rotation sensor.
    constexpr int raiseMs = 200;

    wrist.moveUP();
    manual(100);
    pros::delay(raiseMs);
    stop();

    // // Like reset(), seek the bottom using motor movement, with a timeout.
    // // Keep reset() unchanged; this sequence also tares both motor encoders
    // // and does not zero anything if homing times out.
    // double lastLeft = liftMotors.get_position(0);
    // double lastRight = liftMotors.get_position(1);
    // int stableMs = 0;
    // manual(-100);
    // for (int elapsed = 0; elapsed < 3000 && stableMs < 500; elapsed += 20) {
    //     pros::delay(20);
    //     const double left = liftMotors.get_position(0);
    //     const double right = liftMotors.get_position(1);
    //     // Allow initial motor response, and require both motors to stop.
    //     if (elapsed >= 300 && std::isfinite(left) && std::isfinite(right) &&
    //         std::fabs(left - lastLeft) < 0.5 &&
    //         std::fabs(right - lastRight) < 0.5) {
    //         stableMs += 20;
    //     } else {
    //         stableMs = 0;
    //     }
    //     lastLeft = left;
    //     lastRight = right;
    // }
    // stop();

    // if (stableMs < 500) return false;

    // liftMotors.tare_position_all();
    // rotationSensor.reset_position();
    // targetAngle = 0.0;
    // return true;
}


// =====================================================
// CONSTRUCTOR
// =====================================================

Lift::Lift(
    signed char leftPort,
    signed char rightPort,
    signed char rotationPort
)
    : liftMotors({leftPort, rightPort}, pros::MotorGearset::red),
      rotationSensor(rotationPort) {

    liftMotors.set_brake_mode_all(pros::MotorBrake::coast);
}


// =====================================================
// STOP
// =====================================================

const pros::MotorGroup& Lift::getMotorGroup() const {
    return liftMotors;
}

void Lift::stop() {

    liftMotors.brake();
}


// =====================================================
// MANUAL CONTROL
// =====================================================

void Lift::manual(double speed) {

    // Manual control cancels automatic movement
    autoMode = false;

    liftMotors.move(speed);
}


// =====================================================
// GET ANGLE
// =====================================================

double Lift::getAngle() {

    return rotationSensor.get_position() / 100.0;
}


// =====================================================
// MOVE TO ANGLE
// =====================================================

void Lift::moveAngle(double angle) {

    targetAngle = angle;

    autoMode = true;
}


// =====================================================
// CHECK IF AUTO MODE IS ACTIVE
// =====================================================

bool Lift::isAuto() {

    return autoMode;
}


// =====================================================
// CHECK TARGET
// =====================================================

bool Lift::atTarget() {
   
    return std::abs(
        targetAngle - getAngle()
    ) <= margin;
}


// =====================================================
// AUTOMATIC UPDATE
// =====================================================

void Lift::update() {

    // Do nothing if automatic movement is not active
    if (!autoMode) {
        return;
    }


    double currentAngle = getAngle();

    double error =
        targetAngle - currentAngle;


    // Target reached
    if (std::abs(error) <= margin) {

        stop();

        autoMode = false;

        return;
    }


    // Proportional control with a minimum output to overcome static friction/load.
    double speed = (error > 0 ? kpUp : kpDown) * error; //double speed = kp * error;


    // This point is reached only when outside the target margin, so preserve
    // the direction while ensuring small errors still produce useful movement.
    if (speed > 0) {
        speed = std::max(speed, minUpOutput);
    } else {
        speed = std::min(speed, -minDownOutput);
    }

    // Limit motor power
    speed = std::clamp(
        speed,
        -90.0,
        127.0
    );


    liftMotors.move(speed);
}


// =====================================================
// CANCEL AUTOMATIC MOVEMENT
// =====================================================

void Lift::cancelAuto() {

    autoMode = false;

    stop();
}


// =====================================================
// RESET LIFT
// =====================================================

void Lift::reset() {

    // Temporarily coast while moving into hard stop
    liftMotors.set_brake_mode_all(
        pros::E_MOTOR_BRAKE_COAST
    );


    // Move downward into physical hard stop
    liftMotors.move(-80);


    int stableTime = 0;
    int totalTime = 0;


    double lastPosition =
        (
            liftMotors.get_position(0)
            +
            liftMotors.get_position(1)
        ) / 2.0;


    // Wait until lift stops physically moving
    while (
        stableTime < 500
        &&
        totalTime < 3000
    ) {

        pros::delay(20);

        totalTime += 20;


        double currentPosition =
            (
                liftMotors.get_position(0)
                +
                liftMotors.get_position(1)
            ) / 2.0;


        if (
            std::fabs(
                currentPosition - lastPosition
            ) < 0.5
        ) {

            stableTime += 20;
        }
        else {

            stableTime = 0;
        }


        lastPosition = currentPosition;
    }


    // Stop motors
    liftMotors.move(0);


    // Physical bottom becomes 0 degrees
    rotationSensor.reset_position();


    targetAngle = 0.0;

    autoMode = false;


    // Restore hold brake mode
    liftMotors.set_brake_mode_all(
        pros::E_MOTOR_BRAKE_HOLD
    );


    stop();
}
