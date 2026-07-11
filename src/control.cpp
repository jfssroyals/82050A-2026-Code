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


// back side button
void Control::backSideAction() {
    if (clawMode == PICKUP) {
        backSidePickup();
    } else {
        backSideDrop();
    }
}

// Pickup functions
void Control::frontSidePickup() {
    bar.moveToFront();
    while (!bar.isAtFront()) {
        pros::delay(10);
    }
    claw.close();
}

void Control::backSidePickup() {
    bar.moveToBack();

    while (!bar.isAtBack()) {
        pros::delay(10);
    }

    claw.close();
}


// Drop functions
void Control::frontSideDrop() {
     bar.moveToFront();
     // pros::delay(1000);
     while (!bar.isAtFront()){
        pros::delay(10);
    }
    claw.open();
}

void Control::backSideDrop() {
    bar.moveToBack();
    while (!bar.isAtBack()) {
        pros::delay(10);
    }

    claw.open();
}
