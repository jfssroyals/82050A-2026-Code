
#pragma once

#include "pros/colors.hpp"
#include "ui_transform.hpp"

namespace ui {

void clear();

void fillRect(
    int x1, int y1, int x2, int y2,
    pros::Color color
);

void drawRect(
    int x1, int y1, int x2, int y2,
    pros::Color color
);

void drawText(
    int x, int y,
    const char* text,
    pros::Color color = pros::Color::white,
    int scale = 1
);

} // namespace ui
