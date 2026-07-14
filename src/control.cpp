#include "control.hpp"

Control::Control(Claw& claw, Bar& bar, Lift& lift)
    : claw(claw), 
      bar(bar), 
      lift(lift),
      clawMode(PICKUP),
      state(IDLE)
{}


// Change between pickup and drop
void Control::toggleClawMode() {
    if (clawMode == PICKUP) {
        clawMode = DROP;
    } else {
        clawMode = PICKUP;
    }
}


void Control::frontSideAction() {

    if (clawMode == PICKUP) {
        bar.moveToFront();
        lastPickup = FRONT;
        state = MOVING_FRONT_PICKUP;
    }
    else if (lastPickup == FRONT) {
        bar.moveToFront();
        state = MOVING_FRONT_DROP;
    }
}


void Control::backSideAction() {

    if (clawMode == PICKUP) {
        bar.moveToBack();
        lastPickup = BACK;
        state = MOVING_BACK_PICKUP;
    }
    else {
        // Always allow dropping at the back
        bar.moveToBack();
        state = MOVING_BACK_DROP;
    }
}

void Control::update() {

    switch(state) {

        case MOVING_FRONT_PICKUP:

            if(bar.isAtFront()) {
                claw.close();
                state = IDLE;
            }

            break;

        case MOVING_BACK_PICKUP:

            if(bar.isAtBack()) {
                claw.close();
                state = IDLE;
            }

            break;

        case MOVING_FRONT_DROP:

            if(bar.isAtFront()) {
                claw.open();
                state = IDLE;
            }

            break;

        case MOVING_BACK_DROP:

            if(bar.isAtBack()) {
                claw.open();
                state = IDLE;
            }

            break;

        case IDLE:
            break;
    }
}

// // Pickup functions
// void Control::frontSidePickup() {
//     bar.moveToFront();
//     while (!bar.isAtFront()) {
//         pros::delay(10);
//     }
//     claw.close();
// }

// void Control::backSidePickup() {
//     bar.moveToBack();

//     while (!bar.isAtBack()) {
//         pros::delay(10);
//     }

//     claw.close();
// }


// // Drop functions
// void Control::frontSideDrop() {
//      bar.moveToFront();
//      // pros::delay(1000);
//      while (!bar.isAtFront()){
//         pros::delay(10);
//     }
//     claw.open();
// }

// void Control::backSideDrop() {
//     bar.moveToBack();
//     while (!bar.isAtBack()) {
//         pros::delay(10);
//     }

//     claw.open();
// }
