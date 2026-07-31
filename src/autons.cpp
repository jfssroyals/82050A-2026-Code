 #include "main.h"
 #include "autons.hpp"
 ASSET(path1);

//void fourPinBlue() {
// pros::delay(1000);
//     chassis.setPose(0, 0, 0);
    
//     chassis.arcade(127, 0);
//     pros::delay(300);
//     chassis.arcade(-127, 0);
//     pros::delay(150);
//     chassis.arcade(100, 0);
//     pros::delay(450);
//     chassis.arcade(0, 0);
//     chassis.moveToPose(20, -12.3, 270, 1000, {.forwards = false, .lead = 0.6});
//}
 void redRight() {

    chassis.setPose(-19.885, -172.457, 350);

    chassis.follow(path1, 2000, false);

}

