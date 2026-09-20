#include "selector.hpp"
#include <exception>
#include <vector>
#include "pros/colors.hpp"
#include "pros/screen.hpp"

#include "autons.hpp"


// ============================================================
// AUTON DEFINITIONS
// ============================================================
//
// Add autons HERE.
//
// You do NOT need to update AUTONS.size().
//
// The compiler calculates the count automatically.
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

namespace {

constexpr int FOURPIN_RED1 = 0;
constexpr int AUTON_BLUE_2 = 1;
constexpr int AUTON_RED_1 = 2;
constexpr int AUTON_RED_2 = 3;
constexpr int AUTON_SKILLS = 4;
constexpr int BRRR = 5;
constexpr int CHOTTU = 6;
constexpr int FILLER = 7;
constexpr int lebron = 8;
constexpr int dingalinga = 9;
constexpr int imcracked = 10;
}

// List of the Autons
// do this later
const std::vector<AutonSelector::AutonDefinition> AutonSelector::AUTONS = {

    {
        FOURPIN_RED1,
        "FOURPIN_RED1",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::Loader
    },

    {
        AUTON_BLUE_2,
        "BLUE 2",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::NonLoader
    },

    {
        AUTON_RED_1,
        "RED 1",    
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::Loader
    },

    {
        AUTON_RED_2,
        "RED 2",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::NonLoader
    },

    {
        AUTON_SKILLS,
        "SKILLS",
        AutonSelector::AutonMode::Skills,
        AutonSelector::AutonSide::Loader
    },
     {
        BRRR,
        "BRRR",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::NonLoader
     },

     {
        CHOTTU,
        "CHOTTU",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::NonLoader
     },

     {
        FILLER,
        "FILLER",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::NonLoader
     },

     {
        lebron,
        "lebron",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::NonLoader
     },

     {
        dingalinga,
        "dingalinga",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::NonLoader
     },

     {
        imcracked,
        "imcracked",
        AutonSelector::AutonMode::Competition,
        AutonSelector::AutonSide::Loader
     }
};


// ============================================================
// DYNAMIC AUTON COUNT
// ============================================================
//
// This is automatically calculated from AUTONS[].
//
// If you add another auton to AUTONS[], this automatically
// increases.
//
// If you remove one, this automatically decreases.
//
// ============================================================

// use the getter function to expose AUTONS


// ============================================================
// GEOMETRY
// ============================================================

bool AutonSelector::isInside(int x, int y, const Button& button) {

    return (x >= button.left && x < button.right && y >= button.top && y < button.bottom);
}


// ============================================================
// CENTER X
// ============================================================

int AutonSelector::getCenterX(const Button& button) {

    return (button.left + (button.right - button.left) / 2);
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
    : scroll_offset{0},
      touch_was_pressed{false},
      touch_start_x{0},
      touch_start_y{0},
      touch_current_x{0},
      touch_current_y{0},
      touch_task{nullptr},
      screen_number{SCREEN_MODE},
      selected_mode{AutonMode::Competition},
      selected_side{AutonSide::Loader},
      auton_number{-1},
      auton_confirmed{false}
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
    // BUTTON
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


    const int centerX =
        getCenterX(button);

    const int centerY =
        getCenterY(button);


    // PROS text width isn't reliably exposed as a simple
    // function across all versions, so use a conservative
    // centered starting point based on text length.
    //
    // This is much better than hard-coding every button's
    // text position.

    const int textLength = 8;

    const int textX =
        centerX - (textLength * 5);


    pros::screen::print(
        pros::E_TEXT_MEDIUM,
        textX,
        centerY - 8,
        "%s",
        text
    );
}


// ============================================================
// INITIALIZE
// ============================================================

void AutonSelector::initialize() {

    // --------------------------------------------------------
    // RESET STATE
    // --------------------------------------------------------

    screen_number =
        SCREEN_MODE;

    selected_mode =
        AutonMode::Competition;

    selected_side =
        AutonSide::Loader;

    auton_number =
        -1;

    auton_confirmed =
        false;

    scroll_offset =
        0;

    touch_was_pressed =
        false;


    // --------------------------------------------------------
    // DRAW
    // --------------------------------------------------------

    render();


    // --------------------------------------------------------
    // START TOUCH TASK
    // --------------------------------------------------------

    if (touch_task == nullptr) {

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
//
// This handles:
//
//     1. Finger down
//     2. Finger movement
//     3. Finger release
//
// A tap and swipe are treated differently.
//
// ============================================================

void AutonSelector::update() {

    pros::screen_touch_status_s touch =  pros::screen::touch_status();


    // ========================================================
    // FINGER IS DOWN
    // ========================================================

    if (touch.touch_status) {

        // ----------------------------------------------------
        // NEW TOUCH
        // ----------------------------------------------------

        if (!touch_was_pressed) {

            touch_was_pressed =
                true;

            touch_start_x =
                touch.x;

            touch_start_y =
                touch.y;
        }


        // ----------------------------------------------------
        // SAVE CURRENT POSITION
        // ----------------------------------------------------

        touch_current_x = touch.x;

        touch_current_y = touch.y;


        return;
    }


    // ========================================================
    // FINGER WAS RELEASED
    // ========================================================

    if (!touch_was_pressed) {

        return;
    }


    // --------------------------------------------------------
    // TOUCH ENDED
    // --------------------------------------------------------

    touch_was_pressed =
        false;


    // --------------------------------------------------------
    // CALCULATE MOVEMENT
    // --------------------------------------------------------

    const int deltaX =
        touch_current_x -
        touch_start_x;

    const int deltaY =
        touch_current_y -
        touch_start_y;


    // ========================================================
    // SWIPE DETECTION
    // ========================================================

    if (screen_number == SCREEN_COMPETITION_AUTONS || screen_number == SCREEN_SKILLS_AUTONS) 
    
    {

        // ----------------------------------------------------
        // VERTICAL SWIPE
        // ----------------------------------------------------

        if (deltaY <= -SWIPE_THRESHOLD) {

            // Finger moved upward.
            //
            // Content should move upward,
            // therefore scroll downward.

            scrollBy(AUTON_ROW_STEP * 2);

            render();

            return;
        }


        if (deltaY >= SWIPE_THRESHOLD) {

            // Finger moved downward.
            //
            // Content should move downward,
            // therefore scroll upward.

            scrollBy(-AUTON_ROW_STEP * 2);

            render();

            return;
        }
    }


    // ========================================================
    // TAP
    // ========================================================
    //
    // If movement was small enough, treat it as a tap.
    //
    // ========================================================

    if (
        std::abs(deltaX) < SWIPE_THRESHOLD &&
        std::abs(deltaY) < SWIPE_THRESHOLD
    ) {

        handleTouch(
            touch_current_x,
            touch_current_y
        );
    }
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

            auton_confirmed =
                false;

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

    drawButton(
        MODE_COMPETITION,
        "COMPETITION",
        pros::Color::blue,
        selected_mode == AutonMode::Competition
    );


    // --------------------------------------------------------
    // SKILLS
    // --------------------------------------------------------

    drawButton(
        MODE_SKILLS,
        "SKILLS",
        pros::Color::green,
        selected_mode == AutonMode::Skills
    );
}


// ============================================================
// COMPETITION SIDE SCREEN
// ============================================================

void AutonSelector::drawCompetitionSideScreen() {

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
        pros::Color::grey
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


    pros::screen::print(
        pros::E_TEXT_SMALL,
        165,
        35,
        selected_side == AutonSide::Loader
            ? "LOADER SIDE"
            : "NON-LOADER SIDE"
    );


    // --------------------------------------------------------
    // DYNAMIC LIST
    // --------------------------------------------------------

    drawAutonList();


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    drawButton(
        BACK,
        "BACK",
        pros::Color::grey
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
    // DYNAMIC LIST
    // --------------------------------------------------------

    drawAutonList();


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    drawButton(
        BACK,
        "BACK",
        pros::Color::grey
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

    for (int i = 0; i < AUTONS.size(); ++i) {

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
        150,
        10,
        "CONFIRM AUTON"
    );


    // --------------------------------------------------------
    // SELECTED AUTON
    // --------------------------------------------------------

    if (selected != nullptr) {

        pros::screen::print(
            pros::E_TEXT_LARGE,
            180,
            65,
            "%s",
            selected->name
        );


        // ----------------------------------------------------
        // MODE
        // ----------------------------------------------------

        if (
            selected_mode ==
            AutonMode::Competition
        ) {

            pros::screen::print(
                pros::E_TEXT_MEDIUM,
                175,
                100,
                "COMPETITION"
            );


            pros::screen::print(
                pros::E_TEXT_SMALL,
                185,
                130,
                selected_side == AutonSide::Loader
                    ? "LOADER SIDE"
                    : "NON-LOADER SIDE"
            );

        } else {

            pros::screen::print(
                pros::E_TEXT_MEDIUM,
                205,
                100,
                "SKILLS"
            );
        }


        // ----------------------------------------------------
        // STATUS
        // ----------------------------------------------------

        if (auton_confirmed) {

            pros::screen::set_pen(
                pros::Color::green
            );

            pros::screen::print(
                pros::E_TEXT_SMALL,
                180,
                160,
                "AUTON CONFIRMED"
            );

        } else {

            pros::screen::set_pen(
                pros::Color::yellow
            );

            pros::screen::print(
                pros::E_TEXT_SMALL,
                170,
                160,
                "PRESS CONFIRM"
            );
        }
    }


    // --------------------------------------------------------
    // BACK
    // --------------------------------------------------------

    drawButton(
        BACK,
        "BACK",
        pros::Color::grey
    );


    // --------------------------------------------------------
    // CONFIRM
    // --------------------------------------------------------

    drawButton(
        CONFIRM,
        auton_confirmed
            ? "CONFIRMED"
            : "CONFIRM",
        pros::Color::green,
        auton_confirmed
    );
}


// ============================================================
// AUTON MATCHING
// ============================================================

bool AutonSelector::autonMatchesSelection(const AutonDefinition& auton) const {

    // --------------------------------------------------------
    // MODE
    // --------------------------------------------------------

    if (
        auton.mode !=
        selected_mode
    ) {

        return false;
    }


    // --------------------------------------------------------
    // SKILLS
    // --------------------------------------------------------
    //
    // Skills does not care about side.
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

    return (
        auton.side ==
        selected_side
    );
}


// ============================================================
// FILTERED COUNT
// ============================================================

int AutonSelector::getFilteredAutonCount() const {

    int count = 0;


    for (int i = 0; i < AUTONS.size(); ++i) {

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

    if (filteredIndex < 0) {

        return nullptr;
    }


    int currentIndex = 0;


    for (int i = 0; i < AUTONS.size(); ++i) {

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
// MAXIMUM SCROLL
// ============================================================
//
// This calculates how far the list is actually allowed to
// scroll.
//
// Nothing is hard-coded based on the number of autons.
//
// ============================================================

int AutonSelector::getMaxScrollOffset() const {

    const int count =
        getFilteredAutonCount();


    if (count <= 0) {

        return 0;
    }


    const int contentHeight =
        AUTON_ROW_STEP * count;


    const int visibleHeight =
        LIST_BOTTOM - LIST_TOP;


    const int maxScroll =
        contentHeight -
        visibleHeight;


    if (maxScroll < 0) {

        return 0;
    }


    return maxScroll;
}


// ============================================================
// CLAMP SCROLL
// ============================================================

void AutonSelector::clampScroll() {

    const int maxScroll =
        getMaxScrollOffset();


    if (scroll_offset < 0) {

        scroll_offset = 0;
    }


    if (scroll_offset > maxScroll) {

        scroll_offset =
            maxScroll;
    }
}


// ============================================================
// SCROLL BY
// ============================================================

void AutonSelector::scrollBy(
    int amount
) {

    scroll_offset += amount;

    clampScroll();
}


// ============================================================
// SCROLL UP
// ============================================================

void AutonSelector::scrollUp() {

    scrollBy(
        -AUTON_ROW_STEP
    );
}


// ============================================================
// SCROLL DOWN
// ============================================================

void AutonSelector::scrollDown() {

    scrollBy(
        AUTON_ROW_STEP
    );
}


// ============================================================
// DRAW AUTON LIST
// ============================================================

void AutonSelector::drawAutonList() {

    const int count =
        getFilteredAutonCount();


    // --------------------------------------------------------
    // EMPTY LIST
    // --------------------------------------------------------

    if (count == 0) {

        pros::screen::set_pen(
            pros::Color::white
        );

        pros::screen::print(
            pros::E_TEXT_MEDIUM,
            150,
            105,
            "NO AUTONS AVAILABLE"
        );

        return;
    }


    // --------------------------------------------------------
    // KEEP SCROLL VALID
    // --------------------------------------------------------

    clampScroll();


    // --------------------------------------------------------
    // DRAW LIST
    // --------------------------------------------------------

    int visibleIndex = 0;


    for (int i = 0; i < AUTONS.size(); ++i) {

        const AutonDefinition& auton =
            AUTONS[i];


        // ----------------------------------------------------
        // FILTER
        // ----------------------------------------------------

        if (
            !autonMatchesSelection(
                auton
            )
        ) {

            continue;
        }


        // ----------------------------------------------------
        // CALCULATE Y
        // ----------------------------------------------------

        const int y =
            LIST_TOP +
            visibleIndex * AUTON_ROW_STEP -
            scroll_offset;


        // ----------------------------------------------------
        // DRAW IF VISIBLE
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
                selected_mode == AutonMode::Skills
                    ? pros::Color::green
                    : pros::Color::blue,
                auton.id == auton_number
            );
        }


        ++visibleIndex;
    }


    // ========================================================
    // SCROLL CONTROLS
    // ========================================================

    const int maxScroll =
        getMaxScrollOffset();


    if (maxScroll > 0) {

        // ----------------------------------------------------
        // UP
        // ----------------------------------------------------

        drawButton(
            SCROLL_UP,
            "^",
            pros::Color::grey
        );


        // ----------------------------------------------------
        // DOWN
        // ----------------------------------------------------

        drawButton(
            SCROLL_DOWN,
            "v",
            pros::Color::grey
        );
    }


    // ========================================================
    // SCROLL POSITION
    // ========================================================

    if (maxScroll > 0) {

        const int percentage =
            (
                scroll_offset * 100
            ) /
            maxScroll;


        pros::screen::set_pen(
            pros::Color::white
        );


        pros::screen::print(
            pros::E_TEXT_SMALL,
            30,
            188,
            "%d%%",
            percentage
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

    auton_confirmed =
        false;

    scroll_offset =
        0;

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

    auton_confirmed =
        false;

    scroll_offset =
        0;

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

    auton_confirmed =
        false;

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

    auton_confirmed =
        false;

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

    for (int i = 0; i < AUTONS.size(); ++i) {

        // ----------------------------------------------------
        // FIND AUTON
        // ----------------------------------------------------

        if (
            AUTONS[i].id != auton
        ) {

            continue;
        }


        // ----------------------------------------------------
        // MAKE SURE IT BELONGS TO CURRENT FILTER
        // ----------------------------------------------------

        if (
            !autonMatchesSelection(
                AUTONS[i]
            )
        ) {

            return;
        }


        // ----------------------------------------------------
        // SAVE PENDING SELECTION
        // ----------------------------------------------------

        auton_number =
            auton;


        // ----------------------------------------------------
        // NOT CONFIRMED YET
        // ----------------------------------------------------

        auton_confirmed =
            false;


        // ----------------------------------------------------
        // GO TO CONFIRM SCREEN
        // ----------------------------------------------------

        screen_number =
            SCREEN_CONFIRM;


        return;
    }
}


// ============================================================
// SELECT AUTON AT TOUCH POSITION
// ============================================================

bool AutonSelector::selectAutonAt(
    int x,
    int y
) {

    // --------------------------------------------------------
    // LIST X
    // --------------------------------------------------------

    if (
        x < LIST_LEFT ||
        x >= LIST_RIGHT
    ) {

        return false;
    }


    // --------------------------------------------------------
    // LIST Y
    // --------------------------------------------------------

    if (
        y < LIST_TOP ||
        y >= LIST_BOTTOM
    ) {

        return false;
    }


    // --------------------------------------------------------
    // SCREEN → CONTENT
    // --------------------------------------------------------

    const int contentY =
        y -
        LIST_TOP +
        scroll_offset;


    if (contentY < 0) {

        return false;
    }


    // --------------------------------------------------------
    // FIND ROW
    // --------------------------------------------------------

    const int filteredIndex =
        contentY /
        AUTON_ROW_STEP;


    // --------------------------------------------------------
    // FIND POSITION INSIDE ROW
    // --------------------------------------------------------

    const int rowOffset =
        contentY %
        AUTON_ROW_STEP;


    // --------------------------------------------------------
    // TOUCH WAS IN GAP
    // --------------------------------------------------------

    if (
        rowOffset >= AUTON_ROW_HEIGHT
    ) {

        return false;
    }


    // --------------------------------------------------------
    // FIND ACTUAL AUTON
    // --------------------------------------------------------

    const AutonDefinition* auton =
        getFilteredAuton(
            filteredIndex
        );


    if (auton == nullptr) {

        return false;
    }


    // --------------------------------------------------------
    // SELECT
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
        // MODE
        // ----------------------------------------------------

        case SCREEN_MODE:

            break;


        // ----------------------------------------------------
        // COMPETITION SIDE
        // ----------------------------------------------------

        case SCREEN_COMPETITION_SIDE:

            screen_number =
                SCREEN_MODE;

            auton_number =
                -1;

            auton_confirmed =
                false;

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

            auton_confirmed =
                false;

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

            auton_confirmed =
                false;

            scroll_offset =
                0;

            break;


        // ----------------------------------------------------
        // CONFIRM
        // ----------------------------------------------------

        case SCREEN_CONFIRM:

            // The pending auton is discarded.
            auton_number =
                -1;

            auton_confirmed =
                false;


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

            selected_mode =
                AutonMode::Competition;

            selected_side =
                AutonSide::Loader;

            auton_number =
                -1;

            auton_confirmed =
                false;

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
    // UP
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            SCROLL_UP
        )
    ) {

        scrollUp();

        return true;
    }


    // --------------------------------------------------------
    // DOWN
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            SCROLL_DOWN
        )
    ) {

        scrollDown();

        return true;
    }


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
    // UP
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            SCROLL_UP
        )
    ) {

        scrollUp();

        return true;
    }


    // --------------------------------------------------------
    // DOWN
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            SCROLL_DOWN
        )
    ) {

        scrollDown();

        return true;
    }


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

    if (isInside(x, y, BACK)) {

        goBack();

        return true;
    }


    // --------------------------------------------------------
    // CONFIRM
    // --------------------------------------------------------

    if (
        isInside(
            x,
            y,
            CONFIRM
        )
    ) {

        // ----------------------------------------------------
        // Can't confirm nothing.
        // ----------------------------------------------------

        if (
            auton_number == -1
        ) {

            return false;
        }


        // ----------------------------------------------------
        // ACTUALLY CONFIRM
        // ----------------------------------------------------

        auton_confirmed =
            true;


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

            selected_mode =
                AutonMode::Competition;

            selected_side =
                AutonSide::Loader;

            auton_number =
                -1;

            auton_confirmed =
                false;

            scroll_offset =
                0;

            handled =
                true;

            break;
    }


    // --------------------------------------------------------
    // REDRAW
    // --------------------------------------------------------

    if (handled) {

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


// ============================================================
// GET CONFIRMATION
// ============================================================

bool AutonSelector::isAutonConfirmed() const {

    return auton_confirmed;
}


// ============================================================
// CLEAR CONFIRMATION
// ============================================================

void AutonSelector::clearConfirmation() {

    auton_confirmed =
        false;
}
