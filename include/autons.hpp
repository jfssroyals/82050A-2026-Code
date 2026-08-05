#pragma once

#include "lemlib/api.hpp"

// global objects defined in main.cpp
extern pros::Controller controller;
extern lemlib::Chassis chassis;

// auton routines
void score_back();
void grab_up();
void fourPin_red1();
void fourPin_red2();
void fourPin_blue1();
void fourPin_blue2();
void test();
void skillsAuton();
void skillsScore_L();
void skillsScore_R();
void PostFourPin();
void align_pose(double straight);