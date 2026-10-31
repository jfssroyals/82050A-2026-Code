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
void match_load_ready();
void autonMatchloadTest();
void align_pose(double straight);
