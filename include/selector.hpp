    
#pragma once

#include "api.h"
#include "pros/colors.hpp"
#include "pros/screen.hpp"
#include "ui_transform.hpp"
#include <vector>

class AutonSelector {
public:
    enum class AutonMode { Competition, Skills };
    enum class AutonSide { Loader, NonLoader };
    using Orientation = ui::Orientation;

    struct AutonDefinition {
        int id;
        const char* name;
        AutonMode mode;
        AutonSide side;
    };

    struct Button {
        int left;
        int top;
        int right;
        int bottom;
    };

    AutonSelector();

    void initialize();
    void update();
    void render();

    void setOrientation(Orientation orientation);
    void toggleOrientation();
    Orientation getCurrentOrientation() const;

    int getSelectedAuton() const;
    int getConfirmedAuton() const;
    int getScreen() const;
    AutonMode getSelectedMode() const;
    AutonSide getSelectedSide() const;
    bool isAutonConfirmed() const;
    void clearConfirmation();

    const std::vector<AutonDefinition>& autons_public() const {
        return AUTONS;
    }

private:
    static const std::vector<AutonDefinition> AUTONS;

    static constexpr int SCREEN_MODE = 0;
    static constexpr int SCREEN_COMPETITION_SIDE = 1;
    static constexpr int SCREEN_COMPETITION_AUTONS = 2;
    static constexpr int SCREEN_SKILLS_AUTONS = 3;
    static constexpr int SCREEN_CONFIRM = 4;

    enum class Element {
        ModeCompetition,
        ModeSkills,
        SideLoader,
        SideNonLoader,
        Back,
        Confirm,
        ScrollUp,
        ScrollDown,
        Rotate
    };

    Button layout(Element element) const;

    int listLeft() const;
    int listRight() const;
    int listTop() const;
    int listBottom() const;
    int rowHeight() const;
    int rowStep() const;

    int scroll_offset = 0;
    bool touch_was_pressed = false;
    int touch_start_x = 0;
    int touch_start_y = 0;
    int touch_current_x = 0;
    int touch_current_y = 0;
    static constexpr int SWIPE_THRESHOLD = 20;

    pros::Task* touch_task = nullptr;
    void touchTaskLoop();

    int screen_number = SCREEN_MODE;
    AutonMode selected_mode = AutonMode::Competition;
    AutonSide selected_side = AutonSide::Loader;
    int auton_number = -1;
    bool auton_confirmed = false;

    static bool isInside(int x, int y, const Button& button);
    static int getCenterX(const Button& button);
    static int getCenterY(const Button& button);

    void drawButton(const Button& button, const char* text,
                    pros::Color color, bool selected = false);
    void drawRotateButton();

    void drawModeScreen();
    void drawCompetitionSideScreen();
    void drawCompetitionAutonScreen();
    void drawSkillsAutonScreen();
    void drawConfirmScreen();
    void drawAutonList();

    bool autonMatchesSelection(const AutonDefinition& auton) const;
    int getFilteredAutonCount() const;
    const AutonDefinition* getFilteredAuton(int filteredIndex) const;
    int getMaxScrollOffset() const;
    void clampScroll();
    void scrollBy(int amount);
    void scrollUp();
    void scrollDown();

    void selectCompetition();
    void selectSkills();
    void selectLoader();
    void selectNonLoader();
    void selectAuton(int auton);
    bool selectAutonAt(int x, int y);

    void goBack();
    void handleTouch(int x, int y);
    bool handleModeTouch(int x, int y);
    bool handleCompetitionSideTouch(int x, int y);
    bool handleCompetitionAutonTouch(int x, int y);
    bool handleSkillsAutonTouch(int x, int y);
    bool handleConfirmTouch(int x, int y);
};
