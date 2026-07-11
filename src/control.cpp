#include "control.hpp"

Control::Control(Claw& claw, Bar& bar, Lift& lift)
    : claw(claw), 
      bar(bar), 
      lift(lift),
      clawMode(PICKUP)
{}


// Change between pickup and drop
void Control::toggleClawMode() {
    if (clawMode == PICKUP) {
        clawMode = DROP;
    } else {
        clawMode = PICKUP;
    }
}


// front side button
void Control::frontSideAction() {
    if (clawMode == PICKUP) {
        frontSidePickup();
    } else {
        frontSideDrop();
    }
}


// // back side button
// void Control::backSideAction() {
//     if (clawMode == PICKUP) {
//         backSidePickup();
//     } else {
//         backSideDrop();
//     }
// }

void Control::backSideDrop() {
    bar.moveToBack();

    while (!bar.isAtBack()) {
        pros::delay(10);
    }

    claw.open();
}


// Pickup functions
void Control::frontSidePickup() {
    bar.moveToFront();
    claw.close();
}

// void Control::backSidePickup() {
//     bar.moveToBack();
//     pros::delay(1000);
//     claw.close();
// }
void Control::backSidePickup() {
    bar.moveToBack();

    while (!bar.isAtBack()) {
        pros::delay(10);
    }

    claw.open();
}


// Drop functions
void Control::frontSideDrop() {
    bar.moveToFront();
    // pros::delay(1000);
    claw.open();
}

void Control::backSideDrop() {
    bar.moveToBack();
    pros::delay(1000);
    claw.open();
}