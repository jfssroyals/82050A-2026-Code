#include "control.hpp"
#include "claw_motor.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#include "lift.hpp"
#include "intake.hpp"

// For complex intake sequence declerations
extern pros::adi::DigitalOut intakePiston;
extern Intake intake;
// Intake constructor
Control::Control(Claw& claw, Bar& bar, Lift& lift)
    : claw(claw), bar(bar),lift(lift){
    state = IDLE;
    frontDrop = true;
    }


// Toggle where L2 drops
// false = back drop
// true = front drop
void Control::toggleDropSide() {

    frontDrop = !frontDrop;

}



// L1 action
// Move to front and grab
void Control::frontPickup() {

    bar.moveToFront();

    state = MOVING_FRONT_PICKUP;

}



// L2 action
// Either front drop or back drop
void Control::drop() {


    if (frontDrop) {

        // Move to front and release
        bar.moveToFront();

        state = MOVING_FRONT_DROP;

    }
    else {

        // Move to back and release
        bar.moveToBack();

        state = MOVING_BACK_DROP;

    }

}

// Intkae sequence function
// void Control::startIntakeSequence() {
//     state = INTAKE_SEQUENCE;
//     step = 0;
//     stepStartTime = pros::millis();
// }

// Runs continuously in opcontrol
// Checks when bar reaches position
// then controls claw
// Intkae sequence is also handled here
void Control::update() {

    switch(state) {

        case MOVING_FRONT_PICKUP:
            if(bar.isAtFront()) {
                claw.close();
                state = IDLE;
            }
            break;

        case MOVING_FRONT_DROP:
            if(bar.isAtFront()) {
                claw.open();

                if (lift.isUp) {
                    double adjust = bar.motor.get_position();
                    bar.motor.move_absolute(adjust, 10);
                    bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
                }

                state = IDLE;
            }
            break;

        case MOVING_BACK_DROP:
            if(bar.isAtBack()) {
                claw.open();
                state = IDLE;
            }
            break;

        // case INTAKE_SEQUENCE:

        //     switch(step) {

        //         case 0:
        //             lift.adjustHeight(700);
        //             intake.stop();
        //             intakePiston.set_value(true);
        //             step++;
        //             stepStartTime = pros::millis();
        //             break;

        //         case 1:
        //             if (lift.isUp) {
        //                 bar.motor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
        //                 bar.motor.move(0); // freefall
        //                 step++;
        //                 stepStartTime = pros::millis();
        //             }
        //             break;

        //         case 2:
        //             if (pros::millis() - stepStartTime > 500) {
        //                 lift.adjustHeight(-418);
        //                 step++;
        //                 stepStartTime = pros::millis();
        //             }
        //             break;

        //         case 3:
        //             if (!lift.isUp) {
        //                 claw.close();
        //                 step++;
        //                 stepStartTime = pros::millis();
        //             }
        //             break;

        //         case 4:
        //             lift.adjustHeight(200);
        //             step++;
        //             stepStartTime = pros::millis();
        //             break;

        //         case 5:
        //             bar.moveToAngle(120);
        //             step++;
        //             stepStartTime = pros::millis();
        //             break;

        //         case 6:
        //             lift.setTarget(0);
        //             step++;
        //             stepStartTime = pros::millis();
        //             break;

        //         case 7:
        //             state = IDLE;
        //             break;
        //     }

        //     break;

        case IDLE:
            break;
    }
}