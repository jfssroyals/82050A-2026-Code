#include "control.hpp"
#include "claw_motor.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#include "lift.hpp"


Control::Control(Claw& claw, Bar& bar, Lift& lift)
    : 
    claw(claw),
    bar(bar),
    lift(lift),
    state(IDLE),
    frontDrop(false)
{};


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



// Runs continuously in opcontrol
// Checks when bar reaches position
// then controls claw
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

                if (lift.isUp)
                {
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



        case IDLE:

            break;

    }

}