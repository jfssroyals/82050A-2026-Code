#pragma once

#include "lemlib/api.hpp"

// global objects defined in main.cpp
extern pros::Controller controller;
extern lemlib::Chassis chassis;

// auton routines
void right3pin();
void left3pin();
void auton1();
void auton2();
void autonskills();
void toggleDriveSequence();
void matchload();
// Blocking: open claw and intake until port 4 reads <70 mm for >1 second,
// then stop intake, lower briefly, grip, raise, and flip wrist up.
void intake_pick_up();
// Nonblocking driver-control interface to the same pickup sequence.
void start_intake_pick_up();
void update_intake_pick_up();
void cancel_intake_pick_up();
bool intake_pick_up_active();
void match_load_ready();
void autonMatchloadTest();
void align_pose(double straight);
