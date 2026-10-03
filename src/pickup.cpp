#include "pickup.hpp"


// =====================================================
// CONSTRUCTOR
// =====================================================

PickupSystem::PickupSystem(
    Lift& lift,
    Intake& intake,
    Claw& claw,
    Wrist& wrist,
    pros::Distance& distanceSensor
)
    : lift(lift),
      intake(intake),
      claw(claw),
      wrist(wrist),
      distanceSensor(distanceSensor) {
}


// =====================================================
// START AUTOMATIC PICKUP
// =====================================================

void PickupSystem::start() {

    intake.spinInward();

    wrist.moveDOWN();

    lift.moveAngle(LIFT_UP_ANGLE);

    state = State::MOVING_UP;
}


// =====================================================
// STOP AUTOMATIC PICKUP
// =====================================================

void PickupSystem::stop() {

    state = State::OFF;

    lift.cancelAuto();
}


// =====================================================
// CHECK IF RUNNING
// =====================================================

bool PickupSystem::isRunning() {

    return state != State::OFF;
}


// =====================================================
// UPDATE STATE MACHINE
// =====================================================

void PickupSystem::update() {

    // Always allow lift automatic controller to run
    lift.update();


    if (state == State::OFF) {

        return;
    }


    // Read distance sensor
    int distance = distanceSensor.get();


    bool objectDetected =
        distance > 0
        &&
        distance < DISTANCE_THRESHOLD;


    switch (state) {


        // =================================================
        // MOVING LIFT TO PICKUP HEIGHT
        // =================================================

        case State::MOVING_UP:

            if (lift.atTarget()) {

                state = State::WAITING_FOR_OBJECT;
            }

            break;


        // =================================================
        // WAIT FOR OBJECT
        // =================================================

        case State::WAITING_FOR_OBJECT:

            if (objectDetected) {

                lift.moveAngle(LIFT_DOWN_ANGLE);

                state = State::MOVING_DOWN;
            }

            break;


        // =================================================
        // LOWER LIFT
        // =================================================

        case State::MOVING_DOWN:

            if (lift.atTarget()) {

                clawCloseTimer = pros::millis();

                state = State::WAITING_TO_CLOSE;
            }

            break;


        // =================================================
        // WAIT BEFORE CLOSING CLAW
        // =================================================

        case State::WAITING_TO_CLOSE:

            if (
                pros::millis() - clawCloseTimer
                >= CLAW_CLOSE_DELAY
            ) {

                claw.close();

                wrist.moveUP();

                lift.moveAngle(LIFT_UP_ANGLE);

                state = State::MOVING_BACK_UP;
            }

            break;


        // =================================================
        // MOVE OBJECT BACK UP
        // =================================================

        case State::MOVING_BACK_UP:

            if (lift.atTarget()) {

                state = State::WAITING_FOR_CLEAR;
            }

            break;


        // =================================================
        // WAIT UNTIL OBJECT LEAVES SENSOR
        // =================================================

        case State::WAITING_FOR_CLEAR:

            if (!objectDetected) {

                state = State::WAITING_FOR_OBJECT;
            }

            break;


        case State::OFF:

            break;
    }
}