#include "lift.hpp"

#include <algorithm>
#include <cmath>


// constructor

Lift::Lift(signed char leftPort, signed char rightPort, signed char rotationPort)
    : L_liftMotor(leftPort, pros::MotorGearset::green), R_liftMotor(rightPort, pros::MotorGearset::green), rotationSensor(rotationPort) {

    // When stopped, actively hold the lift position.
    L_liftMotor.set_brake_mode(pros::MotorBrake::hold);
    R_liftMotor.set_brake_mode(pros::MotorBrake::hold);
}

// manual motor control

void Lift::manual(double speed) {

    L_liftMotor.move(speed);
    R_liftMotor.move(speed);

    // Update target angle to current angle to prevent sudden jumps when switching back to automatic control
    targetAngle = getAngle();
}


// get current angle of lift according to rotation sensor

double Lift::getAngle() {

    // Rotation sensor reports centidegrees, divide by 100 to get degrees
   
    return rotationSensor.get_position() / 100.0;
}


// set target height

void Lift::moveAngle(double angle) {

    // only changes where we WANT the lift to go.

    targetAngle = angle;
}

void Lift::update() {

    // Get current angle of lift
    double currentAngle = getAngle();

    // Calculate error between target and current angle
    double error = targetAngle - currentAngle;

    // If the error is within the margin, stop the motors
    if (std::abs(error) < margin) {
        stop(); // brake both motors
        return;
    }

    // Calculate motor speed using proportional control
    double speed = kp * error;

    // Limit speed to [-127, 127]
    speed = std::clamp(speed, -127.0, 127.0);

    // Move lift motors
    manual(speed);
}

void Lift::reset() {  
  
    // Let lift fall toward the hard stop  
    L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);  
    R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);  
  
    L_liftMotor.move(-80);  
    R_liftMotor.move(-80);  
  
    int stableTime = 0;  
  
    double lastPosition =   
        (L_liftMotor.get_position() + R_liftMotor.get_position()) / 2.0;  
  
    while (stableTime < 500) {  
  
        pros::delay(20);  
  
        double currentPosition =  
            (L_liftMotor.get_position() + R_liftMotor.get_position()) / 2.0;  
  
        // Lift is no longer moving
        if (std::fabs(currentPosition - lastPosition) < 0.5) {  
            stableTime += 20;  
        } 
        else {  
            stableTime = 0;  
        }  
  
        lastPosition = currentPosition;  
    }  
  
    // Stop motors  
    L_liftMotor.move(0);  
    R_liftMotor.move(0);  

    // Make position 0 degrees.
    rotationSensor.reset_position();

    // Controller's desired position should also be 0.
    targetAngle = 0.0;
  
  
    // Return to hold mode after calibration  
    L_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);  
    R_liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);  
}