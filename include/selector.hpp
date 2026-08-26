#pragma once

#include "main.h"

class AutonSelector {

private:

    // =========================================================
    // SCREEN CONSTANTS
    // =========================================================

    static constexpr int SCREEN_WIDTH = 480;
    static constexpr int SCREEN_HEIGHT = 240;


    // =========================================================
    // SCREEN STATES
    // =========================================================

    static constexpr int SCREEN_MODE = 0;
    static constexpr int SCREEN_AUTON = 1;
    static constexpr int SCREEN_SKILLS = 2;


    // =========================================================
    // AUTONOMOUS SELECTIONS
    // =========================================================

    static constexpr int AUTON_BLUE_1 = 0;
    static constexpr int AUTON_BLUE_2 = 1;
    static constexpr int AUTON_RED_1 = 2;
    static constexpr int AUTON_RED_2 = 3;
    static constexpr int AUTON_SKILLS = 4;


    // =========================================================
    // STATE
    // =========================================================

    int screen_number;
    int auton_number;


    // =========================================================
    // TOUCH STATE
    // =========================================================

    bool touch_was_pressed;


    // =========================================================
    // RECTANGLE
    // =========================================================

    struct Button {

        int left;
        int top;
        int right;
        int bottom;
    };


    // =========================================================
    // BUTTON GEOMETRY
    // =========================================================

    static constexpr Button MODE_AUTON = {
        0, 65, 240, 240
    };

    static constexpr Button MODE_SKILLS = {
        240, 65, 480, 240
    };


    static constexpr Button BLUE_1 = {
        20, 55, 225, 115
    };

    static constexpr Button BLUE_2 = {
        255, 55, 460, 115
    };

    static constexpr Button RED_1 = {
        20, 125, 225, 185
    };

    static constexpr Button RED_2 = {
        255, 125, 460, 185
    };


    static constexpr Button SKILLS = {
        70, 65, 410, 155
    };


    static constexpr Button BACK = {
        20, 195, 140, 225
    };


    // =========================================================
    // GEOMETRY FUNCTIONS
    // =========================================================

    bool isInside(
        int x,
        int y,
        const Button& button
    );

    int getCenterX(
        const Button& button
    );

    int getCenterY(
        const Button& button
    );


public:

    void initialize();

    void render();

    void update();

    int getSelectedAuton();

    int getScreen();
};