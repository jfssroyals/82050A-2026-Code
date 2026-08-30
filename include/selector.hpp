#pragma once

#include "main.h"

// ============================================================
// AUTONOMOUS SELECTOR
// ============================================================
//
// MODE
// ├── COMPETITION
// │   └── SIDE
// │       ├── LOADER
// │       │   └── COMPETITION LOADER AUTONS
// │       └── NON-LOADER
// │           └── COMPETITION NON-LOADER AUTONS
// │
// └── SKILLS
//     └── SKILLS AUTONS
//
// ============================================================

class AutonSelector {

public:

    // ========================================================
    // SCREEN STATES
    // ========================================================

    static constexpr int SCREEN_MODE = 0;

    static constexpr int SCREEN_COMPETITION_SIDE = 1;

    static constexpr int SCREEN_COMPETITION_AUTONS = 2;

    static constexpr int SCREEN_SKILLS_AUTONS = 3;

    static constexpr int SCREEN_CONFIRM = 4;


    // ========================================================
    // AUTON MODE
    // ========================================================

    enum class AutonMode {

        Competition,

        Skills
    };


    // ========================================================
    // AUTON SIDE
    // ========================================================

    enum class AutonSide {

        Loader,

        NonLoader
    };


    // ========================================================
    // AUTON DEFINITION
    // ========================================================

    struct AutonDefinition {

        int id;

        const char* name;

        AutonMode mode;

        AutonSide side;
    };


    // ========================================================
    // CONSTRUCTOR
    // ========================================================

    AutonSelector();


    // ========================================================
    // LIFECYCLE
    // ========================================================

    void initialize();

    void update();

    void render();


    // ========================================================
    // GETTERS
    // ========================================================

    int getSelectedAuton() const;

    int getScreen() const;

    AutonMode getSelectedMode() const;

    AutonSide getSelectedSide() const;


private:

    // ========================================================
    // AUTON DEFINITIONS
    // ========================================================
    //
    // Replace these with your actual autonomous routines.
    //
    // The important part is:
    //
    // Competition + Loader
    // Competition + NonLoader
    // Skills
    //
    // ========================================================

    static const AutonDefinition AUTONS[];

    static constexpr int AUTON_COUNT = 5;


    // ========================================================
    // AUTON IDs
    // ========================================================

    static constexpr int AUTON_BLUE_1 = 0;

    static constexpr int AUTON_BLUE_2 = 1;

    static constexpr int AUTON_RED_1 = 2;

    static constexpr int AUTON_RED_2 = 3;

    static constexpr int AUTON_SKILLS = 4;


    // ========================================================
    // BUTTON
    // ========================================================

    struct Button {

        int left;

        int top;

        int right;

        int bottom;
    };


    // ========================================================
    // SCREEN STATE
    // ========================================================

    int screen_number;


    // ========================================================
    // SELECTION STATE
    // ========================================================

    AutonMode selected_mode;

    AutonSide selected_side;

    int auton_number;


    // ========================================================
    // TOUCH STATE
    // ========================================================

    bool touch_was_pressed;


    // ========================================================
    // TOUCH TASK
    // ========================================================

    pros::Task* touch_task;


    // ========================================================
    // SCROLL
    // ========================================================

    int scroll_offset;


    // ========================================================
    // SCREEN DIMENSIONS
    // ========================================================

    static constexpr int SCREEN_WIDTH = 480;

    static constexpr int SCREEN_HEIGHT = 240;


    // ========================================================
    // AUTON LIST GEOMETRY
    // ========================================================

    static constexpr int LIST_LEFT = 20;

    static constexpr int LIST_RIGHT = 460;

    static constexpr int LIST_TOP = 60;

    static constexpr int LIST_BOTTOM = 185;

    static constexpr int AUTON_ROW_HEIGHT = 50;

    static constexpr int AUTON_ROW_GAP = 10;

    static constexpr int AUTON_ROW_STEP =
        AUTON_ROW_HEIGHT + AUTON_ROW_GAP;


    // ========================================================
    // MODE BUTTONS
    // ========================================================

    static constexpr Button MODE_COMPETITION = {

        0,
        65,
        240,
        240
    };


    static constexpr Button MODE_SKILLS = {

        240,
        65,
        480,
        240
    };


    // ========================================================
    // SIDE BUTTONS
    // ========================================================

    static constexpr Button SIDE_LOADER = {

        20,
        70,
        225,
        170
    };


    static constexpr Button SIDE_NON_LOADER = {

        255,
        70,
        460,
        170
    };


    // ========================================================
    // NAVIGATION BUTTONS
    // ========================================================

    static constexpr Button BACK = {

        20,
        195,
        140,
        225
    };


    static constexpr Button CONFIRM = {

        255,
        195,
        460,
        225
    };


    // ========================================================
    // GEOMETRY
    // ========================================================

    static bool isInside(
        int x,
        int y,
        const Button& button
    );


    static int getCenterX(
        const Button& button
    );


    static int getCenterY(
        const Button& button
    );


    // ========================================================
    // DRAWING
    // ========================================================

    static void drawButton(
        const Button& button,
        const char* text,
        pros::Color color,
        bool selected
    );


    void drawModeScreen();

    void drawCompetitionSideScreen();

    void drawCompetitionAutonScreen();

    void drawSkillsAutonScreen();

    void drawConfirmScreen();


    // ========================================================
    // AUTON LIST
    // ========================================================

    void drawAutonList(
        AutonMode mode,
        AutonSide side
    );


    // ========================================================
    // FILTERING
    // ========================================================

    bool autonMatchesSelection(
        const AutonDefinition& auton
    ) const;


    int getFilteredAutonCount() const;


    const AutonDefinition* getFilteredAuton(
        int filteredIndex
    ) const;


    // ========================================================
    // MODE SELECTION
    // ========================================================

    void selectCompetition();

    void selectSkills();


    // ========================================================
    // SIDE SELECTION
    // ========================================================

    void selectLoader();

    void selectNonLoader();


    // ========================================================
    // AUTON SELECTION
    // ========================================================

    void selectAuton(
        int auton
    );


    bool selectAutonAt(
        int x,
        int y
    );


    // ========================================================
    // NAVIGATION
    // ========================================================

    void goBack();


    // ========================================================
    // TOUCH DISPATCH
    // ========================================================

    void handleTouch(
        int x,
        int y
    );


    bool handleModeTouch(
        int x,
        int y
    );


    bool handleCompetitionSideTouch(
        int x,
        int y
    );


    bool handleCompetitionAutonTouch(
        int x,
        int y
    );


    bool handleSkillsAutonTouch(
        int x,
        int y
    );


    bool handleConfirmTouch(
        int x,
        int y
    );


    // ========================================================
    // TOUCH TASK
    // ========================================================

    void touchTaskLoop();
};
