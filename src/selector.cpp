#include "selector.hpp"

#include "pros/colors.hpp"
#include "pros/screen.hpp"

// ============================================================
// GEOMETRY
// ============================================================

bool AutonSelector::isInside(
    int x,
    int y,
    const Button& button
) {

    /*
        Mathematical button boundary:

            left <= x < right
            top  <= y < bottom

        This gives every button a clear area
        and prevents overlapping boundaries.
    */

    return (
        x >= button.left &&
        x < button.right &&
        y >= button.top &&
        y < button.bottom
    );
}


// ============================================================
// CENTER X
// ============================================================

int AutonSelector::getCenterX(
    const Button& button
) {

    return (
        button.left +
        (button.right - button.left) / 2
    );
}


// ============================================================
// CENTER Y
// ============================================================

int AutonSelector::getCenterY(
    const Button& button
) {

    return (
        button.top +
        (button.bottom - button.top) / 2
    );
}


// ============================================================
// INITIALIZE
// ============================================================

void AutonSelector::initialize() {

    screen_number = SCREEN_MODE;

    auton_number = AUTON_BLUE_1;

    touch_was_pressed = false;

    render();
}


// ============================================================
// RENDER
// ============================================================

void AutonSelector::render() {

    pros::screen::erase();


    // ========================================================
    // MODE SCREEN
    // ========================================================

    if (screen_number == SCREEN_MODE) {

        // ----------------------------------------------------
        // TITLE
        // ----------------------------------------------------

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_LARGE,
            175,
            10,
            "SELECT MODE"
        );

        pros::screen::print(
            pros::E_TEXT_SMALL,
            125,
            38,
            "Choose your autonomous type"
        );


        // ----------------------------------------------------
        // COMPETITION AUTON
        // ----------------------------------------------------

        pros::screen::set_pen(
            pros::Color::blue
        );

        pros::screen::fill_rect(
            MODE_AUTON.left,
            MODE_AUTON.top,
            MODE_AUTON.right - 1,
            MODE_AUTON.bottom - 1
        );


        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_LARGE,
            getCenterX(MODE_AUTON) - 35,
            100,
            "AUTON"
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            getCenterX(MODE_AUTON) - 65,
            140,
            "COMPETITION"
        );

        pros::screen::print(
            pros::E_TEXT_SMALL,
            getCenterX(MODE_AUTON) - 45,
            175,
            "Tap to select"
        );


        // ----------------------------------------------------
        // DIVIDER
        // ----------------------------------------------------

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::fill_rect(
            238,
            65,
            241,
            239
        );


        // ----------------------------------------------------
        // SKILLS
        // ----------------------------------------------------

        pros::screen::set_pen(
            pros::Color::green
        );

        pros::screen::fill_rect(
            MODE_SKILLS.left,
            MODE_SKILLS.top,
            MODE_SKILLS.right - 1,
            MODE_SKILLS.bottom - 1
        );


        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_LARGE,
            getCenterX(MODE_SKILLS) - 65,
            100,
            "AUTON SKILLS"
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            getCenterX(MODE_SKILLS) - 65,
            140,
            "SKILLS CHALLENGE"
        );

        pros::screen::print(
            pros::E_TEXT_SMALL,
            getCenterX(MODE_SKILLS) - 45,
            175,
            "Tap to select"
        );

        return;
    }


    // ========================================================
    // COMPETITION AUTON SCREEN
    // ========================================================

    if (screen_number == SCREEN_AUTON) {

        // ----------------------------------------------------
        // TITLE
        // ----------------------------------------------------

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_LARGE,
            145,
            8,
            "COMPETITION AUTON"
        );

        pros::screen::print(
            pros::E_TEXT_SMALL,
            185,
            35,
            "Select a routine"
        );


        // ====================================================
        // BLUE 1
        // ====================================================

        if (auton_number == AUTON_BLUE_1) {

            pros::screen::set_pen(
                pros::Color::green
            );

        } else {

            pros::screen::set_pen(
                pros::Color::blue
            );
        }

        pros::screen::fill_rect(
            BLUE_1.left,
            BLUE_1.top,
            BLUE_1.right - 1,
            BLUE_1.bottom - 1
        );

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            getCenterX(BLUE_1) - 30,
            75,
            "BLUE 1"
        );


        // ====================================================
        // BLUE 2
        // ====================================================

        if (auton_number == AUTON_BLUE_2) {

            pros::screen::set_pen(
                pros::Color::green
            );

        } else {

            pros::screen::set_pen(
                pros::Color::blue
            );
        }

        pros::screen::fill_rect(
            BLUE_2.left,
            BLUE_2.top,
            BLUE_2.right - 1,
            BLUE_2.bottom - 1
        );

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            getCenterX(BLUE_2) - 30,
            75,
            "BLUE 2"
        );


        // ====================================================
        // RED 1
        // ====================================================

        if (auton_number == AUTON_RED_1) {

            pros::screen::set_pen(
                pros::Color::green
            );

        } else {

            pros::screen::set_pen(
                pros::Color::red
            );
        }

        pros::screen::fill_rect(
            RED_1.left,
            RED_1.top,
            RED_1.right - 1,
            RED_1.bottom - 1
        );

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            getCenterX(RED_1) - 30,
            145,
            "RED 1"
        );


        // ====================================================
        // RED 2
        // ====================================================

        if (auton_number == AUTON_RED_2) {

            pros::screen::set_pen(
                pros::Color::green
            );

        } else {

            pros::screen::set_pen(
                pros::Color::red
            );
        }

        pros::screen::fill_rect(
            RED_2.left,
            RED_2.top,
            RED_2.right - 1,
            RED_2.bottom - 1
        );

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            getCenterX(RED_2) - 30,
            145,
            "RED 2"
        );


        // ====================================================
        // BACK
        // ====================================================

        pros::screen::set_pen(
            pros::Color::grey
        );

        pros::screen::fill_rect(
            BACK.left,
            BACK.top,
            BACK.right - 1,
            BACK.bottom - 1
        );

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_SMALL,
            getCenterX(BACK) - 18,
            202,
            "BACK"
        );

        return;
    }


    // ========================================================
    // SKILLS SCREEN
    // ========================================================

    if (screen_number == SCREEN_SKILLS) {

        // ----------------------------------------------------
        // TITLE
        // ----------------------------------------------------

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_LARGE,
            160,
            8,
            "AUTON SKILLS"
        );

        pros::screen::print(
            pros::E_TEXT_SMALL,
            150,
            35,
            "Skills Challenge"
        );


        // ====================================================
        // SKILLS BUTTON
        // ====================================================

        if (auton_number == AUTON_SKILLS) {

            pros::screen::set_pen(
                pros::Color::green
            );

        } else {

            pros::screen::set_pen(
                pros::Color::blue
            );
        }

        pros::screen::fill_rect(
            SKILLS.left,
            SKILLS.top,
            SKILLS.right - 1,
            SKILLS.bottom - 1
        );

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_LARGE,
            getCenterX(SKILLS) - 35,
            85,
            "SKILLS"
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            getCenterX(SKILLS) - 80,
            125,
            "SKILLS AUTONOMOUS"
        );

        pros::screen::print(
            pros::E_TEXT_SMALL,
            getCenterX(SKILLS) - 30,
            155,
            "SELECTED"
        );


        // ====================================================
        // BACK
        // ====================================================

        pros::screen::set_pen(
            pros::Color::grey
        );

        pros::screen::fill_rect(
            BACK.left,
            BACK.top,
            BACK.right - 1,
            BACK.bottom - 1
        );

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_SMALL,
            getCenterX(BACK) - 18,
            202,
            "BACK"
        );

        return;
    }
}


// ============================================================
// UPDATE
// ============================================================

void AutonSelector::update() {

    // ========================================================
    // ROBOT MUST BE DISABLED
    // ========================================================

    if (!pros::competition::is_disabled()) {

        touch_was_pressed = false;

        return;
    }


    // ========================================================
    // GET TOUCH
    // ========================================================

    auto touch = pros::screen::touch_status();


    // ========================================================
    // NO TOUCH
    // ========================================================

    if (!touch.touch_status) {

        touch_was_pressed = false;

        return;
    }


    // ========================================================
    // IGNORE HELD TOUCH
    // ========================================================

    /*
        This makes one physical press count as ONE press.

        Without this:

            finger down
                  ↓
            update()
                  ↓
            update()
                  ↓
            update()

        could potentially trigger the button multiple times.
    */

    if (touch_was_pressed) {
        return;
    }


    // ========================================================
    // NEW PRESS
    // ========================================================

    touch_was_pressed = true;


    const int x = touch.x;
    const int y = touch.y;


    // ========================================================
    // MODE SCREEN
    // ========================================================

    if (screen_number == SCREEN_MODE) {

        // ----------------------------------------------------
        // COMPETITION
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                MODE_AUTON
            )
        ) {

            screen_number = SCREEN_AUTON;

            auton_number = AUTON_BLUE_1;

            render();

            return;
        }


        // ----------------------------------------------------
        // SKILLS
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                MODE_SKILLS
            )
        ) {

            screen_number = SCREEN_SKILLS;

            auton_number = AUTON_SKILLS;

            render();

            return;
        }


        return;
    }


    // ========================================================
    // COMPETITION AUTON
    // ========================================================

    if (screen_number == SCREEN_AUTON) {

        // ----------------------------------------------------
        // BLUE 1
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                BLUE_1
            )
        ) {

            auton_number = AUTON_BLUE_1;

            render();

            return;
        }


        // ----------------------------------------------------
        // BLUE 2
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                BLUE_2
            )
        ) {

            auton_number = AUTON_BLUE_2;

            render();

            return;
        }


        // ----------------------------------------------------
        // RED 1
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                RED_1
            )
        ) {

            auton_number = AUTON_RED_1;

            render();

            return;
        }


        // ----------------------------------------------------
        // RED 2
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                RED_2
            )
        ) {

            auton_number = AUTON_RED_2;

            render();

            return;
        }


        // ----------------------------------------------------
        // BACK
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                BACK
            )
        ) {

            screen_number = SCREEN_MODE;

            render();

            return;
        }


        return;
    }


    // ========================================================
    // SKILLS
    // ========================================================

    if (screen_number == SCREEN_SKILLS) {

        // ----------------------------------------------------
        // SKILLS
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                SKILLS
            )
        ) {

            auton_number = AUTON_SKILLS;

            render();

            return;
        }


        // ----------------------------------------------------
        // BACK
        // ----------------------------------------------------

        if (
            isInside(
                x,
                y,
                BACK
            )
        ) {

            screen_number = SCREEN_MODE;

            render();

            return;
        }


        return;
    }
}


// ============================================================
// GET SELECTED AUTON
// ============================================================

int AutonSelector::getSelectedAuton() {

    return auton_number;
}


// ============================================================
// GET CURRENT SCREEN
// ============================================================

int AutonSelector::getScreen() {

    return screen_number;
};