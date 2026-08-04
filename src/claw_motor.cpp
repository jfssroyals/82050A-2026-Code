#include "../include/claw_motor.hpp"
#include <cmath>
#include "lift.hpp"

extern Lift lift;
// Creates the bar motor using the green gear cartridge.
Bar::Bar(int motorPort) : motor(motorPort, pros::MotorGearset::green) {}

// Converts a real bar angle into motor degrees using the mechanism's gear ratio.
double Bar::degreesToMotor(double realDegrees) {
    double degreesBelowZero = 45.0; 
    
    return (realDegrees + degreesBelowZero) * MOTOR_DEGREES_PER_BAR_DEGREE;
}

// Moves the bar to a desired angle using the motor's built-in position controller.
void Bar::moveToAngle(double realDegrees) {
    motor.move_absolute(degreesToMotor(realDegrees), 60);
}

// Moves the bar to the front hard stop.
// Uses a manual slew so the bar slows down before contacting the stop.
void Bar::moveToFront() {
    if (lift.isUp) {
        
        motor.move_absolute(-260, 110);
        
        
        while (std::fabs(motor.get_position() - (-260)) > 10) {
            printf("Motor Position: %.2f\n", motor.get_position());
            pros::delay(10);
        }
        
        //move to angle only if the lift is raised, otherwise will just go till hardstop on intake
        motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
        motor.brake();
    }
    else {
        //move for time
        
        motor.move(127);
        pros::delay(750);
        motor.move(60);
        pros::delay(100);

        motor.tare_position();
        motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        motor.brake();
        pros::delay(50);
    }

    isBack = false;
}

// Moves the bar back to the rear hard stop.
// The encoder is re-zeroed every time for accuracy.
void Bar::moveToBack() {
    motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    motor.move(-127);
    pros::delay(600);
    motor.move(-90);
    pros::delay(500);
    motor.move(-20);

    int stableTime = 0;
    double lastPosition = motor.get_position();

    while (stableTime < 100) {
        pros::delay(20);
        double currentPosition = motor.get_position();
        if (std::fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        } else {
            stableTime = 0;
        }
        lastPosition = currentPosition;
    }

    motor.brake();
    // motor.tare_position();
    isBack = true;
}

// Used during initialize() to home the bar.
// Slowly moves into the back hard stop and sets that position as encoder zero.
void Bar::calibrateBack() {
    motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    motor.move(-70);
    int stableTime = 0;
    double lastPosition = motor.get_position();

    while (stableTime < 100) {
        pros::delay(20);
        double currentPosition = motor.get_position();
        if (std::fabs(currentPosition - lastPosition) < 0.5) {
            stableTime += 20;
        } else {
            stableTime = 0;
        }
        lastPosition = currentPosition;
    }

    motor.move(0);
    // motor.tare_position();
    isBack = true;

    //pros::lcd::print(7, "Pos: %.2f", motor.get_position());
    //pros::lcd::print(6, "Bar Calibrated");
}

void Bar::reset() {
    calibrateBack();
}

// Returns true when the bar is at the back and the encoder is close to zero.
bool Bar::isAtBack() {
    return isBack;
}

// Returns true when the bar is at the front and the encoder is close to 180 degrees.
// UNCHANGED: Left exactly as you provided.
bool Bar::isAtFront() {
    return !isBack && std::fabs(
        motor.get_position() - degreesToMotor(180)
    ) < 20;
}

void Bar::comeToIntake() {
    //motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
    motor.move(100);
    pros::delay(800);
    motor.move(0);
}