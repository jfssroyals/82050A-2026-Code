#pragma once

#include "api.h"
#include "pros/colors.hpp"
#include "pros/screen.hpp"

class AutonSelector {

public:

    // ========================================================
    // ENUMS
    // ========================================================

    enum class AutonMode {
        Competition,
        Skills
    };

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
    // PUBLIC CONTROL
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

    bool isAutonConfirmed() const;


    // ========================================================
    // CONFIRMATION
    // ========================================================

    void clearConfirmation();

    const std::vector<AutonDefinition>& autons_public() 
    {   
        return AUTONS;
    }

private:

    // ========================================================
    // AUTON DATA
    // ========================================================
    //
    // The array itself is defined in selector.cpp.
    //
    // AUTON_COUNT is NOT manually defined.
    // It is calculated automatically in selector.cpp.
    //
    // ========================================================

    static const std::vector<AutonDefinition> AUTONS;


    // ========================================================
    // SCREEN STATES
    // ========================================================

    static constexpr int SCREEN_MODE = 0;

    static constexpr int SCREEN_COMPETITION_SIDE = 1;

    static constexpr int SCREEN_COMPETITION_AUTONS = 2;

    static constexpr int SCREEN_SKILLS_AUTONS = 3;

    static constexpr int SCREEN_CONFIRM = 4;


    // ========================================================
    // SCREEN DIMENSIONS
    // ========================================================

    static constexpr int SCREEN_WIDTH = 480;

    static constexpr int SCREEN_HEIGHT = 240;


    // ========================================================
    // MODE BUTTONS
    // ========================================================

    static constexpr struct Button {

        int left;
        int top;
        int right;
        int bottom;

    } MODE_COMPETITION = {
        10,
        65,
        235,
        230
    };


    static constexpr Button MODE_SKILLS = {
        245,
        65,
        470,
        230
    };


    // ========================================================
    // SIDE BUTTONS
    // ========================================================

    static constexpr Button SIDE_LOADER = {
        40,
        70,
        220,
        135
    };


    static constexpr Button SIDE_NON_LOADER = {
        260,
        70,
        440,
        135
    };


    // ========================================================
    // GENERAL BUTTONS
    // ========================================================

    static constexpr Button BACK = {
        20,
        195,
        140,
        230
    };


    static constexpr Button CONFIRM = {
        340,
        195,
        460,
        230
    };


    // ========================================================
    // LIST BUTTONS
    // ========================================================

    static constexpr int LIST_LEFT = 30;

    static constexpr int LIST_RIGHT = 450;

    static constexpr int LIST_TOP = 55;

    static constexpr int LIST_BOTTOM = 185;

    static constexpr int AUTON_ROW_HEIGHT = 35;

    static constexpr int AUTON_ROW_STEP = 42;


    // ========================================================
    // SCROLL BUTTONS
    // ========================================================

    static constexpr Button SCROLL_UP = {
        440,
        55,
        475,
        105
    };


    static constexpr Button SCROLL_DOWN = {
        440,
        135,
        475,
        185
    };


    // ========================================================
    // SCROLLING
    // ========================================================

    int scroll_offset;

    int getMaxScrollOffset() const;

    void scrollUp();

    void scrollDown();

    void scrollBy(int amount);

    void clampScroll();


    // ========================================================
    // TOUCH / GESTURE STATE
    // ========================================================

    bool touch_was_pressed;

    int touch_start_x;

    int touch_start_y;

    int touch_current_x;

    int touch_current_y;


    static constexpr int SWIPE_THRESHOLD = 20;


    // ========================================================
    // TOUCH TASK
    // ========================================================

    pros::Task* touch_task;

    void touchTaskLoop();


    // ========================================================
    // CURRENT STATE
    // ========================================================

    int screen_number;

    AutonMode selected_mode;

    AutonSide selected_side;

    // Current selection waiting for confirmation.
    int auton_number;

    // True only after CONFIRM has been pressed.
    bool auton_confirmed;


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

    void drawButton(
        const Button& button,
        const char* text,
        pros::Color color,
        bool selected = false
    );

    void drawModeScreen();

    void drawCompetitionSideScreen();

    void drawCompetitionAutonScreen();

    void drawSkillsAutonScreen();

    void drawConfirmScreen();

    void drawAutonList();


    // ========================================================
    // AUTON FILTERING
    // ========================================================

    bool autonMatchesSelection(
        const AutonDefinition& auton
    ) const;

    int getFilteredAutonCount() const;

    const AutonDefinition* getFilteredAuton(
        int filteredIndex
    ) const;


    // ========================================================
    // AUTON SELECTION
    // ========================================================

    void selectCompetition();

    void selectSkills();

    void selectLoader();

    void selectNonLoader();

    void selectAuton(int auton);

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
};
