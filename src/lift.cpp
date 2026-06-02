#include "lift.hpp"
#include <cmath>

// =======================
// DR4B LIFT MOTORS
// Change ports to your robot
// =======================
pros::MotorGroup liftMotors({7, -8}, pros::MotorGearset::red);

// Rotation sensor for lift
pros::Rotation liftRotation(9);

// =======================
// LIFT STAGE SENSOR VALUES
// These are examples. Tune them on your robot.
// =======================
const double STAGE_1_SENSOR = 2500;   // 6.5"
const double STAGE_2_SENSOR = 5200;   // 13"
const double STAGE_3_SENSOR = 10500;  // 26"
const double STAGE_4_SENSOR = 16000;  // 39"

// Current PID target
double liftTarget = STAGE_1_SENSOR;

// =======================
// PID CONSTANTS
// Tune these
// =======================
double kP = 0.018;
double kI = 0.0;
double kD = 0.08;

// Helps lift hold against gravity
double holdPower = 8;

// Maximum motor power
double maxLiftPower = 127;

double clampValue(double value, double minValue, double maxValue) {
    if (value > maxValue) return maxValue;
    if (value < minValue) return minValue;
    return value;
}

double getLiftPosition() {
    return liftRotation.get_position();
}

void setLiftStage(int stage) {
    if (stage == 1) {
        liftTarget = STAGE_1_SENSOR;
    } else if (stage == 2) {
        liftTarget = STAGE_2_SENSOR;
    } else if (stage == 3) {
        liftTarget = STAGE_3_SENSOR;
    } else if (stage == 4) {
        liftTarget = STAGE_4_SENSOR;
    }
}

void liftPIDTask() {
    double lastError = 0;
    double integral = 0;

    while (true) {
        double currentPosition = liftRotation.get_position();

        double error = liftTarget - currentPosition;
        double derivative = error - lastError;

        integral += error;

        // Stop integral from becoming too large
        if (std::fabs(error) > 1000) {
            integral = 0;
        }

        double power = (kP * error) + (kI * integral) + (kD * derivative);

        // Add hold power when lift is raised
        if (liftTarget > 500) {
            power += holdPower;
        }

        power = clampValue(power, -maxLiftPower, maxLiftPower);

        liftMotors.move(power);

        lastError = error;

        pros::delay(20);
    }
}

void liftInit() {
    liftRotation.reset_position();

    liftTarget = STAGE_1_SENSOR;

    static pros::Task liftTask(liftPIDTask);
}