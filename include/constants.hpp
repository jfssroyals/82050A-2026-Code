#pragma once

namespace constants {

    // linear controller
    constexpr float linear_kP = 2;
    constexpr float linear_kI = 0;
    constexpr float linear_kD = 10;
    constexpr float linear_antiWindup = 0;
    constexpr float linear_smallError = 0;
    constexpr int linear_smallTimeout = 0;
    constexpr float linear_largeError = 0;
    constexpr int linear_largeTimeout = 00;
    constexpr float linear_slew = 0;

    // angular controller
    constexpr float angular_kP = 2;
    constexpr float angular_kI = 0;
    constexpr float angular_kD = 10; // 40
    constexpr float angular_antiWindup = 3;
    constexpr float angular_smallError = 1;
    constexpr int angular_smallTimeout = 100;
    constexpr float angular_largeError = 3;
    constexpr int angular_largeTimeout = 500;
    constexpr float angular_slew = 0;

    // // profile wrappers
    // constexpr int profileDriveMaxSpeed = 90;
    // constexpr int profileTurnMaxSpeed = 80;
}