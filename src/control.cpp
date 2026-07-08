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


// Same side button
void Control::sameSideAction() {
    if (clawMode == PICKUP) {
        sameSidePickup();
    } else {
        sameSideDrop();
    }
}


// Opposite side button
void Control::oppositeSideAction() {
    if (clawMode == PICKUP) {
        oppositeSidePickup();
    } else {
        oppositeSideDrop();
    }
}


// Pickup functions
void Control::sameSidePickup() {
    bar.moveToFront();
    claw.close();
}

void Control::oppositeSidePickup() {
    bar.moveToBack();
    claw.close();
}


// Drop functions
void Control::sameSideDrop() {
    bar.moveToFront();
    claw.open();
}

void Control::oppositeSideDrop() {
    bar.moveToBack();
    claw.open();
}