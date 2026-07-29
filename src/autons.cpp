 #include "main.h"
 #include "autons.hpp"

void fourPinBlue() {
pros::delay(1000);
    chassis.setPose(0, 0, 0);
    
    chassis.arcade(127, 0);
    pros::delay(300);
    chassis.arcade(-127, 0);
    pros::delay(150);
    chassis.arcade(100, 0);
    pros::delay(450);
    chassis.arcade(0, 0);
    chassis.moveToPose(20, -12.3, 270, 1000, {.forwards = false, .lead = 0.6});
}