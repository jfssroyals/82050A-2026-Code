#pragma once

#include "lemlib/api.hpp"

// global objects defined in main.cpp
extern pros::Controller controller;
extern lemlib::Chassis chassis;

// auton routines
void fivePin_red1();
void fivePin_red2();
void fivePin_blue1();
void fivePin_blue2();
void test();
void skillsAuton();