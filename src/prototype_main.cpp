// #include "../VEXtensions/main.h"
// #include "lemlib/api.hpp" // IWYU pragma: keep
// #include "../VEXtensions/autons.hpp"
// #include "../VEXtensions/constants.hpp"
// #include <cmath>

// // controller
// pros::Controller controller(pros::E_CONTROLLER_MASTER);

// // motor groups
// pros::MotorGroup leftMotors({-5, 4, -3}, pros::MotorGearset::blue);
// pros::MotorGroup rightMotors({6, -9, 7}, pros::MotorGearset::blue);

// // Inertial Sensor on port 10
// pros::Imu imu(10);

// // tracking wheels
// pros::Rotation horizontalEnc(20);
// pros::Rotation verticalEnc(-11);

// lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5.75);
// lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, -2.5);

// // drivetrain settings
// lemlib::Drivetrain drivetrain(
//     &leftMotors,
//     &rightMotors,
//     10,
//     lemlib::Omniwheel::NEW_4,
//     360,
//     2
// );

// lemlib::ControllerSettings linearController(
//     constants::linear_kP,
//     constants::linear_kI,
//     constants::linear_kD,
//     constants::linear_antiWindup,
//     constants::linear_smallError,
//     constants::linear_smallTimeout,
//     constants::linear_largeError,
//     constants::linear_largeTimeout,
//     constants::linear_slew
// );

// lemlib::ControllerSettings angularController(
//     constants::angular_kP,
//     constants::angular_kI,
//     constants::angular_kD,
//     constants::angular_antiWindup,
//     constants::angular_smallError,
//     constants::angular_smallTimeout,
//     constants::angular_largeError,
//     constants::angular_largeTimeout,
//     constants::angular_slew
// );

// lemlib::OdomSensors sensors(
//     &vertical,
//     nullptr,
//     &horizontal,
//     nullptr,
//     &imu
// );

// lemlib::ExpoDriveCurve throttleCurve(3, 10, 1.019);
// lemlib::ExpoDriveCurve steerCurve(3, 10, 1.019);

// lemlib::Chassis chassis(
//     drivetrain,
//     linearController,
//     angularController,
//     sensors,
//     &throttleCurve,
//     &steerCurve
// );

// // =======================
// // DR4B LIFT SETUP - ADDED
// // Change ports to match your robot
// // =======================
// pros::MotorGroup liftMotors({1, -2}, pros::MotorGearset::red);
// pros::Rotation liftRotation(12);

// // =======================
// // LIFT STAGES - ADDED
// // These sensor values are examples only.
// // Tune them on the real robot.
// // =======================
// const double LIFT_STAGE_1_SENSOR = 2500;   // 6.5"
// const double LIFT_STAGE_2_SENSOR = 5200;   // 13"
// const double LIFT_STAGE_3_SENSOR = 10500;  // 26"
// const double LIFT_STAGE_4_SENSOR = 16000;  // 39"

// double liftTarget = LIFT_STAGE_1_SENSOR;

// // =======================
// // LIFT PID CONSTANTS - ADDED
// // =======================
// double lift_kP = 0.018;
// double lift_kI = 0.0;
// double lift_kD = 0.08;

// double liftHoldPower = 8;
// double liftMaxPower = 127;

// double clampLiftPower(double value, double minValue, double maxValue) {
//     if (value > maxValue) return maxValue;
//     if (value < minValue) return minValue;
//     return value;
// }

// void setLiftStage(int stage) {
//     if (stage == 1) {
//         liftTarget = LIFT_STAGE_1_SENSOR;
//     } else if (stage == 2) {
//         liftTarget = LIFT_STAGE_2_SENSOR;
//     } else if (stage == 3) {
//         liftTarget = LIFT_STAGE_3_SENSOR;
//     } else if (stage == 4) {
//         liftTarget = LIFT_STAGE_4_SENSOR;
//     }
// }

// void liftPIDTask() {
//     double lastError = 0;
//     double integral = 0;

//     while (true) {
//         double currentPosition = liftRotation.get_position();

//         double error = liftTarget - currentPosition;
//         double derivative = error - lastError;

//         integral += error;

//         if (std::fabs(error) > 1000) {
//             integral = 0;
//         }

//         double power =
//             (lift_kP * error) +
//             (lift_kI * integral) +
//             (lift_kD * derivative);

//         if (liftTarget > 500) {
//             power += liftHoldPower;
//         }

//         power = clampLiftPower(power, -liftMaxPower, liftMaxPower);

//         liftMotors.move(power);

//         lastError = error;

//         pros::delay(20);
//     }
// }

// void liftInit() {
//     liftRotation.reset_position();
//     liftTarget = LIFT_STAGE_1_SENSOR;

//     static pros::Task liftTask(liftPIDTask);
// }

// void initialize() {
//     pros::lcd::initialize();
//     chassis.calibrate();

//     liftInit();

//     pros::Task screenTask([&]() {
//         while (true) {
//             pros::lcd::print(0, "X: %f", chassis.getPose().x);
//             pros::lcd::print(1, "Y: %f", chassis.getPose().y);
//             pros::lcd::print(2, "Theta: %f", chassis.getPose().theta);

//             // Added lift display
//             pros::lcd::print(3, "Lift: %.0f", liftRotation.get_position());
//             pros::lcd::print(4, "Lift target: %.0f", liftTarget);

//             lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());

//             pros::delay(50);
//         }
//     });
// }

// void disabled() {}

// void competition_initialize() {}

// ASSET(example_txt);

// void autonomous() {
//     // add autonomous selector
// }

// void opcontrol() {
//     while (true) {
//         int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
//         int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

//         chassis.arcade(leftY, rightX);

//         // =======================
//         // DR4B LIFT BUTTONS - ADDED
//         // A = Stage 1: 6.5"
//         // B = Stage 2: 13"
//         // X = Stage 3: 26"
//         // Y = Stage 4: 39"
//         // =======================
//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
//             setLiftStage(1);
//             controller.print(0, 0, "Lift Stage 1   ");
//         }

//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_B)) {
//             setLiftStage(2);
//             controller.print(0, 0, "Lift Stage 2   ");
//         }

//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_X)) {
//             setLiftStage(3);
//             controller.print(0, 0, "Lift Stage 3   ");
//         }

//         if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_Y)) {
//             setLiftStage(4);
//             controller.print(0, 0, "Lift Stage 4   ");
//         }

//         // Manual tuning:
//         // R1 raises lift
//         // R2 lowers lift//         if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
//             liftMotors.move(80);
//             liftTarget = liftRotation.get_position();
//         } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
//             liftMotors.move(-80);
//             liftTarget = liftRotation.get_position();
//         }

//         pros::delay(10);
//     }
// }