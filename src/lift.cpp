#include "lift.hpp"

#include <algorithm>
#include <cmath>


// constructor

Lift::Lift(signed char leftPort, signed char rightPort, signed char rotationPort)
    : L_liftMotor(leftPort, pros::MotorGearset::green),
      R_liftMotor(rightPort, pros::MotorGearset::green),
      rotationSensor(rotationPort) {

    L_liftMotor.set_brake_mode(pros::MotorBrake::hold);
    R_liftMotor.set_brake_mode(pros::MotorBrake::hold);
}


// lift brake function

void Lift::stop() {
    L_liftMotor.brake();
    R_liftMotor.brake();
}


// manual motor control

void Lift::manual(double speed) {

    autoMode = false;

    L_liftMotor.move(speed);
    R_liftMotor.move(speed);
}


// get current angle of lift according to rotation sensor

double Lift::getAngle() {

    return rotationSensor.get_position() / 100.0;
}


// set target height

void Lift::moveAngle(double angle) {

    targetAngle = angle;
    autoMode = true;
}


void Lift::update() {

    if (!autoMode) {
        stop();
        return;
    }

    double currentAngle = getAngle();

    double error = targetAngle - currentAngle;

    if (std::abs(error) < margin) {
        stop();
        autoMode = false;
        return;
    }

    double speed = kp * error;

    speed = std::clamp(speed, -127.0, 127.0);

    L_liftMotor.move(speed);
    R_liftMotor.move(speed);
}


void Lift::reset() {

    L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);

    L_liftMotor.move(-80);
    R_liftMotor.move(-80);

    int stableTime = 0;
    int totalTime = 0;

    double lastPosition =
        (L_liftMotor.get_position() + R_liftMotor.get_position()) / 2.0;

    while (stableTime < 500 && totalTime < 3000) {

        pros::delay(20);

        totalTime += 20;

        double currentPosition =
            (L_liftMotor.get_position() + R_liftMotor.get_position()) / 2.0;

        if (std::fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        }
        else {
            stableTime = 0;
        }

        lastPosition = currentPosition;
    }

    L_liftMotor.move(0);
    R_liftMotor.move(0);

    rotationSensor.reset_position();

    targetAngle = 0.0;
    autoMode = false;

    L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

    stop();
}