#include "main.h"
#include "autons.hpp"
#include "pros/colors.hpp"
#include "pros/screen.hpp"
#include "selector.hpp"

// draw boxes and colours on the screen
// map out where each auton is supposed to go 
//  make it responsive to touchscreen 
// 


// Add the other functions

void AutonSelector::initialize() {
    // Start from a known state
    screen_number = 0; // Main menu
    auton_number = 0;  // Skills selected

    render();
}

void AutonSelector::render() {
    // Clear the screen before drawing
    pros::screen::erase();

    // =========================
    // MAIN MENU
    // =========================
    if (screen_number == 0) {

        // Title
        pros::screen::set_pen(pros::Color::white);
        pros::screen::print(
            pros::E_TEXT_LARGE,
            180,
            20,
            "Select Auton"
        );

        // =========================
        // SKILLS BUTTON
        // =========================

        if (auton_number == 0) {
            // Selected
            pros::screen::set_pen(pros::Color::green);
        } else {
            // Not selected
            pros::screen::set_pen(pros::Color::blue);
        }

        pros::screen::fill_rect(
            50,   // x1
            70,   // y1
            250,  // x2
            140   // y2
        );

        pros::screen::set_pen(pros::Color::white);

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            115,
            95,
            "Skills"
        );

        // =========================
        // COMPETITION BUTTON
        // =========================

        if (auton_number == 1) {
            // Selected
            pros::screen::set_pen(pros::Color::green);
        } else {
            // Not selected
            pros::screen::set_pen(pros::Color::blue);
        }

        pros::screen::fill_rect(
            300,
            70,
            500,
            140
        );

        pros::screen::set_pen(pros::Color::white);

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            345,
            95,
            "Competition"
        );

        // Instructions
        pros::screen::print(
            pros::E_TEXT_SMALL,
            270,
            160,
            "Touch an option"
        );
    }

    // =========================
    // SKILLS SCREEN
    // =========================
    else if (screen_number == 1) {

        pros::screen::set_pen(pros::Color::white);

        pros::screen::print(
            pros::E_TEXT_LARGE,
            220,
            30,
            "Skills"
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            170,
            80,
            "Skills Autonomous"
        );

        // Back button
        pros::screen::set_pen(pros::Color::red);

        pros::screen::fill_rect(
            20,
            180,
            150,
            220
        );

        pros::screen::set_pen(pros::Color::white);

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            55,
            195,
            "Back"
        );
    }

    // =========================
    // COMPETITION SCREEN
    // =========================
    else if (screen_number == 2) {

        pros::screen::set_pen(pros::Color::white);

        pros::screen::print(
            pros::E_TEXT_LARGE,
            160,
            30,
            "Competition"
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            160,
            80,
            "Competition Autonomous"
        );

        // Back button
        pros::screen::set_pen(pros::Color::red);

        pros::screen::fill_rect(
            20,
            180,
            150,
            220
        );

        pros::screen::set_pen(pros::Color::white);

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            55,
            195,
            "Back"
        );
    }
}

void AutonSelector::update() {
    // Get touchscreen information
    pros::screen_touch_status_s_t touch = pros::screen::touch_status();

    // Nothing is being touched
    if (!touch.touch_status) {
        return;
    }

    int x = touch.x;
    int y = touch.y;

    // =========================
    // MAIN MENU
    // =========================
    if (screen_number == 0) {

        // Skills button
        if (x >= 50 && x <= 250 &&
            y >= 70 && y <= 140) {

            auton_number = 0;
            screen_number = 1;

            render();

            // Prevent multiple inputs
            pros::delay(200);
        }

        // Competition button
        else if (x >= 300 && x <= 500 &&
                 y >= 70 && y <= 140) {

            auton_number = 1;
            screen_number = 2;

            render();

            // Prevent multiple inputs
            pros::delay(200);
        }
    }

    // =========================
    // SKILLS SCREEN
    // =========================
    else if (screen_number == 1) {

        // Back button
        if (x >= 20 && x <= 150 &&
            y >= 180 && y <= 220) {

            screen_number = 0;

            render();

            pros::delay(200);
        }
    }

    // =========================
    // COMPETITION SCREEN
    // =========================
    else if (screen_number == 2) {

        // Back button
        if (x >= 20 && x <= 150 &&
            y >= 180 && y <= 220) {

            screen_number = 0;

            render();

            pros::delay(200);
        }
    }
}

int AutonSelector::getSelectedAuton() {
    return auton_number;
}

int AutonSelector::getScreen() {
    return screen_number;
}
