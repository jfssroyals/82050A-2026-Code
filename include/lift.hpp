#pragma once
#include "main.h"
#include "lemlib/api.hpp"

extern pros::MotorGroup liftmotors;
extern pros::Rotation liftRotation;

void setLiftStage(int Stage);
void liftControlLoop();
