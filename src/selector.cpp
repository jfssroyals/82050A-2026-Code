#include "selector.hpp"

#include "pros/colors.hpp"
#include "pros/screen.hpp"


// ============================================================
// AUTON DEFINITIONS
// ============================================================
//
// Replace these with your actual auton definitions.
//
// Competition:
//
//     Loader
//     NonLoader
//
// Skills:
//
//     Side is ignored.
//
// ============================================================

const AutonSelector::AutonDefinition
AutonSelector::AUTONS[] = {

    {
        AUTON_BLUE_1,
        "BLUE 1",
        AutonMode::Competition,
        AutonSide::Loader
    },

    {
        AUTON_BLUE_2,
        "BLUE 2",
        AutonMode::Competition,
        AutonSide::NonLoader
    },

    {
        AUTON_RED_1,
        "RED 1",
        AutonMode::Competition,
        AutonSide::Loader
    },

    {
        AUTON_RED_2,
        "RED 2",
        AutonMode::Competition,
        AutonSide::NonLoader
    },

    {
        AUTON_SKILLS,
        "SKILLS",
        AutonMode::Skills,
        AutonSide::Loader
    }
};


// ============================================================
// GEOMETRY
// ============================================================

bool AutonSelector::isInside(
    int x,
    int y,
    const Button& button
) {

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
// CONSTRUCTOR
// ============================================================

AutonSelector::AutonSelector()
    : screen_number{SCREEN_MODE},
      selected_mode{AutonMode::Competition},
      selected_side{AutonSide::Loader},
      auton_number{-1},
      touch_was_pressed{false},
      touch_task{nullptr},
      scroll_offset{0}
{
}


// ============================================================
// DRAW BUTTON
// ============================================================

void AutonSelector::drawButton(
    const Button& button,
    const char* text,
    pros::Color color,
    bool selected
) {

    // --------------------------------------------------------
    // BUTTON COLOR
    // --------------------------------------------------------

    if (selected) {

        pros::screen::set_pen(
            pros::Color::green
        );

    } else {

        pros::screen::set_pen(
            color
        );
    }


    // --------------------------------------------------------
    // BUTTON RECTANGLE
    // --------------------------------------------------------

    pros::screen::fill_rect(
        button.left,
        button.top,
        button.right - 1,
        button.bottom - 1
    );


    // --------------------------------------------------------
    // TEXT
    // --------------------------------------------------------

    pros::screen::set_pen(
        pros::Color::white
    );


    pros::screen::print(
        pros::E_TEXT_MEDIUM,
        getCenterX(button) - 45,
        getCenterY(button) - 8,
        "%s",
        text
    );
}


// ============================================================
// INITIALIZE
// ============================================================

void AutonSelector::initialize() {

    screen_number =
        SCREEN_MODE;


    selected_mode =
        AutonMode::Competition;


    selected_side =
        AutonSide::Loader;


    auton_number =
        -1;


    touch_was_pressed =
        false;


    scroll_offset =
        0;


    // --------------------------------------------------------
    // DRAW INITIAL SCREEN
    // --------------------------------------------------------

    render();


    // --------------------------------------------------------
    // START TOUCH TASK
    // --------------------------------------------------------
    //
    // This is what makes the selector continuously process
    // touchscreen input.
    //
    // initialize() should only be called ONCE.
    //
    // --------------------------------------------------------

    if (
        touch_task == nullptr
    ) {

        touch_task =
            new pros::Task(
                [this] {
                    this->touchTaskLoop();
                }
            );
    }
}


// ============================================================
// TOUCH TASK LOOP
// ============================================================

void AutonSelector::touchTaskLoop() {

    while (true) {

        update();

        pros::delay(20);
    }
}


// ============================================================
// UPDATE
// ============================================================

void AutonSelector::update() {

    // --------------------------------------------------------
    // READ TOUCHSCREEN
    // --------------------------------------------------------

    pros::screen_touch_status_s touch =
        pros::screen::touch_status();


    // --------------------------------------------------------
    // NO TOUCH
    // --------------------------------------------------------

    if (
        !touch.touch_status
    ) {

        touch_was_pressed =
            false;

        return;
    }


    // --------------------------------------------------------
    // IGNORE HELD TOUCH
    // --------------------------------------------------------
    //
    // A single press produces one action.
    //
    // Without this check, holding your finger down could
    // repeatedly activate screens every 20ms.
    //
    // --------------------------------------------------------

    if (
        touch_was_pressed
    ) {

        return;
    }


    // --------------------------------------------------------
    // NEW TOUCH
    // --------------------------------------------------------

    touch_was_pressed =
        true;


    // --------------------------------------------------------
    // SEND TOUCH TO CURRENT SCREEN
    // --------------------------------------------------------

    handleTouch(
        touch.x,
        touch.y
    );
}


// ============================================================
// RENDER
// ============================================================

void AutonSelector::render() {

    pros::screen::erase();


    switch (screen_number) {

        case SCREEN_MODE:

            drawModeScreen();

            break;


        case SCREEN_COMPETITION_SIDE:

            drawCompetitionSideScreen();

            break;


        case SCREEN_COMPETITION_AUTONS:

            drawCompetitionAutonScreen();

            break;


        case SCREEN_SKILLS_AUTONS:

            drawSkillsAutonScreen();

            break;


        case SCREEN_CONFIRM:

            drawConfirmScreen();

            break;


        default:

            screen_number =
                SCREEN_MODE;

            selected_mode =
                AutonMode::Competition;

            selected_side =
                AutonSide::Loader;

            auton_number =
                -1;

            scroll_offset =
                0;

            drawModeScreen();

            break;
    }
}


// ============================================================
// MODE SCREEN
// ============================================================

void AutonSelector::drawModeScreen() {

    // --------------------------------------------------------
    // TITLE
    // --------------------------------------------------------

    pros::screen::set_pen(
        pros::Color::white
    );


    pros::screen::print(
        pros::E_TEXT_LARGE,
        150,
        10,
        "SELECT MODE"
    );


    pros::screen::print(
        pros::E_TEXT_SMALL,
        125,
        38,
        "Choose autonomous type"
    );


    // --------------------------------------------------------
    // COMPETITION
    // --------------------------------------------------------

    pros::screen::set_pen(
        pros::Color::blue
    );


    pros::screen::fill_rect(
        MODE_COMPETITION.left,
        MODE_COMPETITION.top,
        MODE_COMPETITION.right - 1,
        MODE_COMPETITION.bottom - 1
    );


    pros::screen::set_pen(
        pros::Color::white
    );


    pros::screen::print(
        pros::E_TEXT_LARGE,
        85,
        100,
        "AUTON"
    );


    pros::screen::print(
        pros::E_TEXT_MEDIUM,
        55,
        140,
        "COMPETITION"
    );


    pros::screen::print(
        pros::E_TEXT_SMALL,
        65,
        175,
        "Tap to select"
    );


    // --------------------------------------------------------
    // DIVIDER
    // --------------------------------------------------------

    pros::screen::fill_rect(
        238,
        65,
        241,
        239
    );


    // --------------------------------------------------------
    // SKILLS
    // --------------------------------------------------------

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
        305,
        100,
        "AUTON SKILLS"
    );


    pros::screen::print(
        pros::E_TEXT_MEDIUM,
        290,
        140,
        "SKILLS CHALLENGE"
    );


    pros::screen::print(
        pros::E_TEXT_SMALL,
        315,
        175,
        "Tap to select"
    );
}


// ============================================================
// COMPETITION SIDE SCREEN
// ============================================================

void AutonSelector::drawCompetitionSideScreen() {

    // --------------------------------------------------------
    // TITLE
    // --------------------------------------------------------

    pros::screen::set_pen(
        pros::Color::white
    );


    pros::screen::print(
        pros::E_TEXT_LARGE,
        135,
        10,
        "COMPETITION"
    );


    pros::screen::print(
        pros::E_TEXT_SMALL,
        145,
        38,
        "SELECT AUTON SIDE"
    );


    // --------------------------------------------------------
    // LOADER
    // --------------------------------------------------------

    drawButton(
        SIDE_LOADER,
        "LOADER",
        pros::Color::blue,
        selected_side == AutonSide::Loader
    );


    // --------------------------------------------------------
    // NON-LOADER
    // --------------------------------------------------------

    drawButton(
        SIDE_NON_LOADER,
        "NON-LOADER",
        pros::Color::blue,
        selected_side == AutonSide::NonLoader
    );


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    drawButton(
        BACK,
        "BACK",
        pros::Color::grey,
        false
    );
}


// ============================================================
// COMPETITION AUTON SCREEN
// ============================================================

void AutonSelector::drawCompetitionAutonScreen() {

    pros::screen::set_pen(
        pros::Color::white
    );


    pros::screen::print(
        pros::E_TEXT_LARGE,
        125,
        8,
        "COMPETITION AUTONS"
    );


    // --------------------------------------------------------
    // SIDE LABEL
    // --------------------------------------------------------

    pros::screen::print(
        pros::E_TEXT_SMALL,
        165,
        35,
        selected_side == AutonSide::Loader
            ? "LOADER SIDE"
            : "NON-LOADER SIDE"
    );


    // --------------------------------------------------------
    // FILTERED LIST
    // --------------------------------------------------------

    drawAutonList(
        AutonMode::Competition,
        selected_side
    );


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    drawButton(
        BACK,
        "BACK",
        pros::Color::grey,
        false
    );
}


// ============================================================
// SKILLS AUTON SCREEN
// ============================================================

void AutonSelector::drawSkillsAutonScreen() {

    pros::screen::set_pen(
        pros::Color::white
    );


    pros::screen::print(
        pros::E_TEXT_LARGE,
        145,
        8,
        "SKILLS AUTONS"
    );


    pros::screen::print(
        pros::E_TEXT_SMALL,
        165,
        35,
        "SELECT A ROUTINE"
    );


    // --------------------------------------------------------
    // SKILLS LIST
    // --------------------------------------------------------
    //
    // Side does NOT matter here.
    //
    // The side parameter is ignored by the filter when the
    // mode is Skills.
    //
    // --------------------------------------------------------

    drawAutonList(
        AutonMode::Skills,
        AutonSide::Loader
    );


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    drawButton(
        BACK,
        "BACK",
        pros::Color::grey,
        false
    );
}


// ============================================================
// CONFIRM SCREEN
// ============================================================

void AutonSelector::drawConfirmScreen() {

    const AutonDefinition* selected =
        nullptr;


    // --------------------------------------------------------
    // FIND SELECTED AUTON
    // --------------------------------------------------------

    for (
        int i = 0;
        i < AUTON_COUNT;
        ++i
    ) {

        if (
            AUTONS[i].id ==
            auton_number
        ) {

            selected =
                &AUTONS[i];

            break;
        }
    }


    // --------------------------------------------------------
    // TITLE
    // --------------------------------------------------------

    pros::screen::set_pen(
        pros::Color::white
    );


    pros::screen::print(
        pros::E_TEXT_LARGE,
        160,
        10,
        "CONFIRM AUTON"
    );


    // --------------------------------------------------------
    // SELECTION
    // --------------------------------------------------------

    if (
        selected != nullptr
    ) {

        pros::screen::print(
            pros::E_TEXT_LARGE,
            180,
            70,
            "%s",
            selected->name
        );


        // ----------------------------------------------------
        // COMPETITION
        // ----------------------------------------------------

        if (
            selected_mode ==
            AutonMode::Competition
        ) {

            pros::screen::print(
                pros::E_TEXT_MEDIUM,
                175,
                105,
                "COMPETITION"
            );


            pros::screen::print(
                pros::E_TEXT_SMALL,
                185,
                135,
                selected_side == AutonSide::Loader
                    ? "LOADER SIDE"
                    : "NON-LOADER SIDE"
            );
        }


        // ----------------------------------------------------
        // SKILLS
        // ----------------------------------------------------

        else {

            pros::screen::print(
                pros::E_TEXT_MEDIUM,
                205,
                105,
                "SKILLS"
            );
        }
    }


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    drawButton(
        BACK,
        "BACK",
        pros::Color::grey,
        false
    );


    // --------------------------------------------------------
    // CONFIRM
    // --------------------------------------------------------

    drawButton(
        CONFIRM,
        "CONFIRM",
        pros::Color::green,
        false
    );
}


// ============================================================
// AUTON FILTER
// ============================================================

bool AutonSelector::autonMatchesSelection(
    const AutonDefinition& auton
) const {

    // --------------------------------------------------------
    // MODE MUST MATCH
    // --------------------------------------------------------

    if (
        auton.mode != selected_mode
    ) {

        return false;
    }


    // --------------------------------------------------------
    // SKILLS
    // --------------------------------------------------------
    //
    // Skills has no side selection.
    //
    // Therefore any Skills auton is valid.
    //
    // --------------------------------------------------------

    if (
        selected_mode ==
        AutonMode::Skills
    ) {

        return true;
    }


    // --------------------------------------------------------
    // COMPETITION
    // --------------------------------------------------------
    //
    // Competition requires the selected side.
    //
    // --------------------------------------------------------

    return (
        auton.side ==
        selected_side
    );
}


// ============================================================
// FILTERED AUTON COUNT
// ============================================================

int AutonSelector::getFilteredAutonCount() const {

    int count = 0;


    for (
        int i = 0;
        i < AUTON_COUNT;
        ++i
    ) {

        if (
            autonMatchesSelection(
                AUTONS[i]
            )
        ) {

            ++count;
        }
    }


    return count;
}


// ============================================================
// GET FILTERED AUTON
// ============================================================

const AutonSelector::AutonDefinition*
AutonSelector::getFilteredAuton(
    int filteredIndex
) const {

    int currentIndex = 0;


    for (
        int i = 0;
        i < AUTON_COUNT;
        ++i
    ) {

        if (
            !autonMatchesSelection(
                AUTONS[i]
            )
        ) {

            continue;
        }


        if (
            currentIndex ==
            filteredIndex
        ) {

            return &AUTONS[i];
        }


        ++currentIndex;
    }


    return nullptr;
}


// ============================================================
// DRAW AUTON LIST
// ============================================================

void AutonSelector::drawAutonList(
    AutonMode mode,
    AutonSide side
) {

    int visibleIndex = 0;


    // --------------------------------------------------------
    // LOOP THROUGH ALL AUTONS
    // --------------------------------------------------------

    for (
        int i = 0;
        i < AUTON_COUNT;
        ++i
    ) {

        const AutonDefinition& auton =
            AUTONS[i];


        // ----------------------------------------------------
        // MODE FILTER
        // ----------------------------------------------------

        if (
            auton.mode != mode
        ) {

            continue;
        }


        // ----------------------------------------------------
        // SIDE FILTER
        // ----------------------------------------------------
        //
        // ONLY Competition uses side.
        //
        // Skills skips this filter.
        //
        // ----------------------------------------------------

        if (
            mode == AutonMode::Competition &&
            auton.side != side
        ) {

            continue;
        }


        // ----------------------------------------------------
        // MATHEMATICAL Y POSITION
        // ----------------------------------------------------

        const int y =
            LIST_TOP +
            visibleIndex * AUTON_ROW_STEP -
            scroll_offset;


        // ----------------------------------------------------
        // DRAW ONLY IF VISIBLE
        // ----------------------------------------------------

        if (
            y < LIST_BOTTOM &&
            y + AUTON_ROW_HEIGHT > LIST_TOP
        ) {

            Button row = {

                LIST_LEFT,
                y,
                LIST_RIGHT,
                y + AUTON_ROW_HEIGHT
            };


            drawButton(
                row,
                auton.name,
                mode == AutonMode::Skills
                    ? pros::Color::green
                    : pros::Color::blue,
                auton.id == auton_number
            );
        }


        ++visibleIndex;
    }


    // --------------------------------------------------------
    // EMPTY LIST
    // --------------------------------------------------------

    if (
        visibleIndex == 0
    ) {

        pros::screen::set_pen(
            pros::Color::white
        );


        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            150,
            100,
            "NO AUTONS AVAILABLE"
        );
    }
}


// ============================================================
// SELECT COMPETITION
// ============================================================

void AutonSelector::selectCompetition() {

    selected_mode =
        AutonMode::Competition;


    selected_side =
        AutonSide::Loader;


    auton_number =
        -1;


    scroll_offset =
        0;


    // --------------------------------------------------------
    // COMPETITION GOES TO SIDE SELECTION
    // --------------------------------------------------------

    screen_number =
        SCREEN_COMPETITION_SIDE;
}


// ============================================================
// SELECT SKILLS
// ============================================================

void AutonSelector::selectSkills() {

    selected_mode =
        AutonMode::Skills;


    auton_number =
        -1;


    scroll_offset =
        0;


    // --------------------------------------------------------
    // SKILLS GOES DIRECTLY TO AUTONS
    // --------------------------------------------------------

    screen_number =
        SCREEN_SKILLS_AUTONS;
}


// ============================================================
// SELECT LOADER
// ============================================================

void AutonSelector::selectLoader() {

    selected_side =
        AutonSide::Loader;


    auton_number =
        -1;


    scroll_offset =
        0;


    screen_number =
        SCREEN_COMPETITION_AUTONS;
}


// ============================================================
// SELECT NON-LOADER
// ============================================================

void AutonSelector::selectNonLoader() {

    selected_side =
        AutonSide::NonLoader;


    auton_number =
        -1;


    scroll_offset =
        0;


    screen_number =
        SCREEN_COMPETITION_AUTONS;
}


// ============================================================
// SELECT AUTON
// ============================================================

void AutonSelector::selectAuton(
    int auton
) {

    for (
        int i = 0;
        i < AUTON_COUNT;
        ++i
    ) {

        // ----------------------------------------------------
        // Find ID
        // ----------------------------------------------------

        if (
            AUTONS[i].id != auton
        ) {

            continue;
        }


        // ----------------------------------------------------
        // Verify it belongs to the current filter.
        // ----------------------------------------------------

        if (
            !autonMatchesSelection(
                AUTONS[i]
            )
        ) {

            return;
        }


        // ----------------------------------------------------
        // Save selection
        // ----------------------------------------------------

        auton_number =
            auton;


        // ----------------------------------------------------
        // Go to confirmation
        // ----------------------------------------------------

        screen_number =
            SCREEN_CONFIRM;


        return;
    }
}


// ============================================================
// SELECT AUTON AT TOUCH POSITION
// ============================================================
//
// Screen coordinate:
//
//     y
//
// is converted to:
//
//     contentY
//
// Then:
//
//     filteredIndex =
//         contentY / AUTON_ROW_STEP
//
// This guarantees the touch calculation uses the same
// geometry as rendering.
//
// ============================================================

bool AutonSelector::selectAutonAt(
    int x,
    int y
) {

    // --------------------------------------------------------
    // X BOUNDARY
    // --------------------------------------------------------

    if (
        x < LIST_LEFT ||
        x >= LIST_RIGHT
    ) {

        return false;
    }


    // --------------------------------------------------------
    // Y BOUNDARY
    // --------------------------------------------------------

    if (
        y < LIST_TOP ||
        y >= LIST_BOTTOM
    ) {

        return false;
    }


    // --------------------------------------------------------
    // SCREEN -> CONTENT COORDINATE
    // --------------------------------------------------------

    const int contentY =
        y -
        LIST_TOP +
        scroll_offset;


    if (
        contentY < 0
    ) {

        return false;
    }


    // --------------------------------------------------------
    // ROW INDEX
    // --------------------------------------------------------

    const int filteredIndex =
        contentY /
        AUTON_ROW_STEP;


    // --------------------------------------------------------
    // POSITION INSIDE ROW
    // --------------------------------------------------------

    const int rowOffset =
        contentY %
        AUTON_ROW_STEP;


    // --------------------------------------------------------
    // TOUCH IN GAP
    // --------------------------------------------------------

    if (
        rowOffset >= AUTON_ROW_HEIGHT
    ) {

        return false;
    }


    // --------------------------------------------------------
    // FIND FILTERED AUTON
    // --------------------------------------------------------

    const AutonDefinition* auton =
        getFilteredAuton(
            filteredIndex
        );


    if (
        auton == nullptr
    ) {

        return false;
    }


    // --------------------------------------------------------
    // SELECT IT
    // --------------------------------------------------------

    selectAuton(
        auton->id
    );


    return true;
}


// ============================================================
// BACK
// ============================================================

void AutonSelector::goBack() {

    switch (screen_number) {

        // ----------------------------------------------------
        // ROOT
        // ----------------------------------------------------

        case SCREEN_MODE:

            // Nothing to go back to.
            break;


        // ----------------------------------------------------
        // COMPETITION SIDE
        // ----------------------------------------------------

        case SCREEN_COMPETITION_SIDE:

            screen_number =
                SCREEN_MODE;

            auton_number =
                -1;

            scroll_offset =
                0;

            break;


        // ----------------------------------------------------
        // COMPETITION AUTONS
        // ----------------------------------------------------

        case SCREEN_COMPETITION_AUTONS:

            screen_number =
                SCREEN_COMPETITION_SIDE;

            auton_number =
                -1;

            scroll_offset =
                0;

            break;


        // ----------------------------------------------------
        // SKILLS AUTONS
        // ----------------------------------------------------

        case SCREEN_SKILLS_AUTONS:

            screen_number =
                SCREEN_MODE;

            auton_number =
                -1;

            scroll_offset =
                0;

            break;


        // ----------------------------------------------------
        // CONFIRM
        // ----------------------------------------------------

        case SCREEN_CONFIRM:

            auton_number =
                -1;


            if (
                selected_mode ==
                AutonMode::Competition
            ) {

                screen_number =
                    SCREEN_COMPETITION_AUTONS;

            } else {

                screen_number =
                    SCREEN_SKILLS_AUTONS;
            }

            break;


        // ----------------------------------------------------
        // INVALID
        // ----------------------------------------------------

        default:

            screen_number =
                SCREEN_MODE;

            auton_number =
                -1;

            scroll_offset =
                0;

            break;
    }
}


// ============================================================
// MODE TOUCH
// ============================================================

bool AutonSelector::handleModeTouch(
    int x,
    int y
) {

    // --------------------------------------------------------
    // COMPETITION
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            MODE_COMPETITION
        )
    ) {

        selectCompetition();

        return true;
    }


    // --------------------------------------------------------
    // SKILLS
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            MODE_SKILLS
        )
    ) {

        selectSkills();

        return true;
    }


    return false;
}


// ============================================================
// COMPETITION SIDE TOUCH
// ============================================================

bool AutonSelector::handleCompetitionSideTouch(
    int x,
    int y
) {

    // --------------------------------------------------------
    // LOADER
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            SIDE_LOADER
        )
    ) {

        selectLoader();

        return true;
    }


    // --------------------------------------------------------
    // NON-LOADER
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            SIDE_NON_LOADER
        )
    ) {

        selectNonLoader();

        return true;
    }


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            BACK
        )
    ) {

        goBack();

        return true;
    }


    return false;
}


// ============================================================
// COMPETITION AUTON TOUCH
// ============================================================

bool AutonSelector::handleCompetitionAutonTouch(
    int x,
    int y
) {

    // --------------------------------------------------------
    // AUTON
    // --------------------------------------------------------

    if (
        selectAutonAt(
            x,
            y
        )
    ) {

        return true;
    }


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            BACK
        )
    ) {

        goBack();

        return true;
    }


    return false;
}


// ============================================================
// SKILLS AUTON TOUCH
// ============================================================

bool AutonSelector::handleSkillsAutonTouch(
    int x,
    int y
) {

    // --------------------------------------------------------
    // AUTON
    // --------------------------------------------------------

    if (
        selectAutonAt(
            x,
            y
        )
    ) {

        return true;
    }


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            BACK
        )
    ) {

        goBack();

        return true;
    }


    return false;
}


// ============================================================
// CONFIRM TOUCH
// ============================================================

bool AutonSelector::handleConfirmTouch(
    int x,
    int y
) {

    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            BACK
        )
    ) {

        goBack();

        return true;
    }


    // --------------------------------------------------------
    // CONFIRM
    // --------------------------------------------------------
    //
    // The selected auton is already stored in auton_number.
    //
    // Your autonomous code can retrieve it with:
    //
    //     getSelectedAuton()
    //
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            CONFIRM
        )
    ) {

        return true;
    }


    return false;
}


// ============================================================
// TOUCH DISPATCHER
// ============================================================

void AutonSelector::handleTouch(
    int x,
    int y
) {

    bool handled =
        false;


    switch (screen_number) {

        // ----------------------------------------------------
        // MODE
        // ----------------------------------------------------

        case SCREEN_MODE:

            handled =
                handleModeTouch(
                    x,
                    y
                );

            break;


        // ----------------------------------------------------
        // COMPETITION SIDE
        // ----------------------------------------------------

        case SCREEN_COMPETITION_SIDE:

            handled =
                handleCompetitionSideTouch(
                    x,
                    y
                );

            break;


        // ----------------------------------------------------
        // COMPETITION AUTONS
        // ----------------------------------------------------

        case SCREEN_COMPETITION_AUTONS:

            handled =
                handleCompetitionAutonTouch(
                    x,
                    y
                );

            break;


        // ----------------------------------------------------
        // SKILLS AUTONS
        // ----------------------------------------------------

        case SCREEN_SKILLS_AUTONS:

            handled =
                handleSkillsAutonTouch(
                    x,
                    y
                );

            break;


        // ----------------------------------------------------
        // CONFIRM
        // ----------------------------------------------------

        case SCREEN_CONFIRM:

            handled =
                handleConfirmTouch(
                    x,
                    y
                );

            break;


        // ----------------------------------------------------
        // INVALID
        // ----------------------------------------------------

        default:

            screen_number =
                SCREEN_MODE;

            auton_number =
                -1;

            scroll_offset =
                0;

            handled =
                true;

            break;
    }


    // --------------------------------------------------------
    // REDRAW AFTER SUCCESSFUL INPUT
    // --------------------------------------------------------

    if (
        handled
    ) {

        render();
    }
}


// ============================================================
// GET SELECTED AUTON
// ============================================================

int AutonSelector::getSelectedAuton() const {

    return auton_number;
}


// ============================================================
// GET SCREEN
// ============================================================

int AutonSelector::getScreen() const {

    return screen_number;
}


// ============================================================
// GET MODE
// ============================================================

AutonSelector::AutonMode
AutonSelector::getSelectedMode() const {

    return selected_mode;
}


// ============================================================
// GET SIDE
// ============================================================

AutonSelector::AutonSide
AutonSelector::getSelectedSide() const {

    return selected_side;
}
