
#include "selector.hpp"
#include "ui_renderer.hpp"

#include <cstdlib>
#include <cstring>

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

void centeredText(int centerX, int y, const char* text,
                  pros::Color color = pros::Color::white) {
    const int length = static_cast<int>(std::strlen(text));
    const int textWidth = length * 6;
    ui::drawText(centerX - textWidth / 2, y, text, color);
}
}

const std::vector<AutonSelector::AutonDefinition>
AutonSelector::AUTONS = {
    {FOURPIN_RED1, "FOURPIN_RED1", AutonMode::Competition, AutonSide::Loader},
    {AUTON_BLUE_2, "BLUE 2", AutonMode::Competition, AutonSide::NonLoader},
    {AUTON_RED_1, "RED 1", AutonMode::Competition, AutonSide::Loader},
    {AUTON_RED_2, "RED 2", AutonMode::Competition, AutonSide::NonLoader},
    {AUTON_SKILLS, "SKILLS", AutonMode::Skills, AutonSide::Loader},
    {BRRR, "BRRR", AutonMode::Competition, AutonSide::NonLoader},
    {CHOTTU, "CHOTTU", AutonMode::Competition, AutonSide::NonLoader},
    {FILLER, "FILLER", AutonMode::Competition, AutonSide::NonLoader},
    {lebron, "lebron", AutonMode::Competition, AutonSide::NonLoader},
    {dingalinga, "dingalinga", AutonMode::Competition, AutonSide::NonLoader},
    {imcracked, "imcracked", AutonMode::Competition, AutonSide::Loader}
};

AutonSelector::AutonSelector() = default;

// ------------------------------------------------------------
// ORIENTATION
// ------------------------------------------------------------

void AutonSelector::setOrientation(Orientation orientation) {
    ui::currentOrientation = orientation;
    render();
}

AutonSelector::Orientation AutonSelector::getCurrentOrientation() const {
    return ui::currentOrientation;
}

// ------------------------------------------------------------
// LAYOUT
// ------------------------------------------------------------

AutonSelector::Button AutonSelector::layout(Element element) const {
    const bool portrait =
        ui::currentOrientation == ui::Orientation::Portrait;

    if (!portrait) {
        switch (element) {
            case Element::ModeCompetition: return {10, 65, 235, 230};
            case Element::ModeSkills:      return {245, 65, 470, 230};
            case Element::SideLoader:      return {40, 70, 220, 135};
            case Element::SideNonLoader:   return {260, 70, 440, 135};
            case Element::Back:            return {20, 195, 140, 230};
            case Element::Confirm:         return {340, 195, 460, 230};
            case Element::ScrollUp:        return {440, 55, 475, 105};
            case Element::ScrollDown:      return {440, 125, 475, 175};
        }
    } else {
        switch (element) {
            case Element::ModeCompetition: return {15, 100, 225, 215};
            case Element::ModeSkills:      return {15, 250, 225, 365};
            case Element::SideLoader:      return {15, 125, 225, 215};
            case Element::SideNonLoader:   return {15, 245, 225, 335};
            case Element::Back:            return {10, 430, 105, 470};
            case Element::Confirm:         return {130, 430, 230, 470};
            case Element::ScrollUp:        return {198, 100, 235, 145};
            case Element::ScrollDown:      return {198, 320, 235, 365};
        }
    }

    return {0, 0, 0, 0};
}

int AutonSelector::listLeft() const {
    return ui::currentOrientation == ui::Orientation::Portrait ? 8 : 20;
}

int AutonSelector::listRight() const {
    return ui::currentOrientation == ui::Orientation::Portrait ? 192 : 420;
}

int AutonSelector::listTop() const {
    return ui::currentOrientation == ui::Orientation::Portrait ? 78 : 55;
}

int AutonSelector::listBottom() const {
    return ui::currentOrientation == ui::Orientation::Portrait ? 410 : 180;
}

int AutonSelector::rowHeight() const {
    return ui::currentOrientation == ui::Orientation::Portrait ? 28 : 35;
}

int AutonSelector::rowStep() const {
    return ui::currentOrientation == ui::Orientation::Portrait ? 33 : 42;
}

// ------------------------------------------------------------
// GEOMETRY
// ------------------------------------------------------------

bool AutonSelector::isInside(int x, int y, const Button& button) {
    return x >= button.left && x < button.right
        && y >= button.top && y < button.bottom;
}

int AutonSelector::getCenterX(const Button& button) {
    return button.left + (button.right - button.left) / 2;
}

int AutonSelector::getCenterY(const Button& button) {
    return button.top + (button.bottom - button.top) / 2;
}

// ------------------------------------------------------------
// BUTTON DRAWING
// ------------------------------------------------------------

void AutonSelector::drawButton(
    const Button& button,
    const char* text,
    pros::Color color,
    bool selected
) {
    const pros::Color fill =
        selected ? pros::Color::green : color;

    ui::fillRect(
        button.left, button.top,
        button.right - 1, button.bottom - 1,
        fill
    );

    ui::drawRect(
        button.left, button.top,
        button.right - 1, button.bottom - 1,
        pros::Color::white
    );

    const int textWidth =
        static_cast<int>(std::strlen(text)) * 6;

    const int textX =
        getCenterX(button) - textWidth / 2;

    const int textY =
        getCenterY(button) - 3;

    ui::drawText(textX, textY, text, pros::Color::white);
}

// ------------------------------------------------------------
// INITIALIZATION AND TOUCH TASK
// ------------------------------------------------------------

void AutonSelector::initialize() {
    screen_number = SCREEN_MODE;
    selected_mode = AutonMode::Competition;
    selected_side = AutonSide::Loader;
    auton_number = -1;
    auton_confirmed = false;
    scroll_offset = 0;

    touch_was_pressed = false;
    touch_start_x = touch_start_y = 0;
    touch_current_x = touch_current_y = 0;

    render();

    if (touch_task == nullptr) {
        touch_task = new pros::Task([this] {
            touchTaskLoop();
        });
    }
}

void AutonSelector::touchTaskLoop() {
    while (true) {
        update();
        pros::delay(20);
    }
}

void AutonSelector::update() {
    const pros::screen_touch_status_s touch =
        pros::screen::touch_status();

    if (touch.touch_status) {
        const ui::Point point = ui::toVirtual(touch.x, touch.y);

        if (!touch_was_pressed) {
            touch_was_pressed = true;
            touch_start_x = point.x;
            touch_start_y = point.y;
        }

        touch_current_x = point.x;
        touch_current_y = point.y;
        return;
    }

    if (!touch_was_pressed) {
        return;
    }

    touch_was_pressed = false;

    const int deltaX = touch_current_x - touch_start_x;
    const int deltaY = touch_current_y - touch_start_y;

    const bool onList =
        screen_number == SCREEN_COMPETITION_AUTONS ||
        screen_number == SCREEN_SKILLS_AUTONS;

    if (onList && std::abs(deltaX) < SWIPE_THRESHOLD &&
        std::abs(deltaY) >= SWIPE_THRESHOLD) {
        // Moving a finger up moves the list toward later entries.
        if (deltaY < 0) {
            scrollBy(rowStep() * 2);
        } else {
            scrollBy(-rowStep() * 2);
        }

        render();
        return;
    }

    if (std::abs(deltaX) < SWIPE_THRESHOLD &&
        std::abs(deltaY) < SWIPE_THRESHOLD) {
        handleTouch(touch_current_x, touch_current_y);
    }
}

// ------------------------------------------------------------
// RENDER
// ------------------------------------------------------------

void AutonSelector::render() {
    ui::clear();

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
            screen_number = SCREEN_MODE;
            selected_mode = AutonMode::Competition;
            selected_side = AutonSide::Loader;
            auton_number = -1;
            auton_confirmed = false;
            scroll_offset = 0;
            drawModeScreen();
            break;
    }
}

// ------------------------------------------------------------
// SCREENS
// ------------------------------------------------------------

void AutonSelector::drawModeScreen() {
    const bool portrait =
        ui::currentOrientation == ui::Orientation::Portrait;

    centeredText(ui::width() / 2, portrait ? 35 : 12, "SELECT MODE");
    centeredText(
        ui::width() / 2,
        portrait ? 55 : 35,
        "CHOOSE AUTON TYPE"
    );

    drawButton(
        layout(Element::ModeCompetition),
        "COMPETITION",
        pros::Color::blue,
        selected_mode == AutonMode::Competition
    );

    drawButton(
        layout(Element::ModeSkills),
        "SKILLS",
        pros::Color::green,
        selected_mode == AutonMode::Skills
    );
}

void AutonSelector::drawCompetitionSideScreen() {
    const bool portrait =
        ui::currentOrientation == ui::Orientation::Portrait;

    centeredText(ui::width() / 2, portrait ? 45 : 12, "COMPETITION");
    centeredText(ui::width() / 2, portrait ? 63 : 35, "SELECT AUTON SIDE");

    drawButton(
        layout(Element::SideLoader),
        "LOADER",
        pros::Color::blue,
        selected_side == AutonSide::Loader
    );

    drawButton(
        layout(Element::SideNonLoader),
        "NON-LOADER",
        pros::Color::blue,
        selected_side == AutonSide::NonLoader
    );

    drawButton(layout(Element::Back), "BACK", pros::Color::grey);
}

void AutonSelector::drawCompetitionAutonScreen() {
    const bool portrait =
        ui::currentOrientation == ui::Orientation::Portrait;

    centeredText(
        ui::width() / 2,
        portrait ? 20 : 7,
        "COMPETITION AUTONS"
    );

    centeredText(
        ui::width() / 2,
        portrait ? 42 : 29,
        selected_side == AutonSide::Loader
            ? "LOADER SIDE" : "NON-LOADER SIDE"
    );

    drawAutonList();
    drawButton(layout(Element::Back), "BACK", pros::Color::grey);
}

void AutonSelector::drawSkillsAutonScreen() {
    const bool portrait =
        ui::currentOrientation == ui::Orientation::Portrait;

    centeredText(ui::width() / 2, portrait ? 20 : 7, "SKILLS AUTONS");
    centeredText(ui::width() / 2, portrait ? 42 : 29, "SELECT A ROUTINE");

    drawAutonList();
    drawButton(layout(Element::Back), "BACK", pros::Color::grey);
}

void AutonSelector::drawConfirmScreen() {
    const bool portrait =
        ui::currentOrientation == ui::Orientation::Portrait;

    centeredText(ui::width() / 2, portrait ? 35 : 12, "CONFIRM AUTON");

    const AutonDefinition* selected = nullptr;

    for (const auto& auton : AUTONS) {
        if (auton.id == auton_number) {
            selected = &auton;
            break;
        }
    }

    if (selected != nullptr) {
        centeredText(
            ui::width() / 2,
            portrait ? 120 : 65,
            selected->name
        );

        centeredText(
            ui::width() / 2,
            portrait ? 155 : 100,
            selected_mode == AutonMode::Competition
                ? "COMPETITION" : "SKILLS"
        );

        if (selected_mode == AutonMode::Competition) {
            centeredText(
                ui::width() / 2,
                portrait ? 180 : 125,
                selected_side == AutonSide::Loader
                    ? "LOADER SIDE" : "NON-LOADER SIDE"
            );
        }

        centeredText(
            ui::width() / 2,
            portrait ? 230 : 160,
            auton_confirmed ? "AUTON CONFIRMED" : "PRESS CONFIRM",
            auton_confirmed ? pros::Color::green : pros::Color::yellow
        );
    }

    drawButton(layout(Element::Back), "BACK", pros::Color::grey);

    drawButton(
        layout(Element::Confirm),
        auton_confirmed ? "CONFIRMED" : "CONFIRM",
        pros::Color::green,
        auton_confirmed
    );
}

// ------------------------------------------------------------
// FILTERING
// ------------------------------------------------------------

bool AutonSelector::autonMatchesSelection(
    const AutonDefinition& auton
) const {
    if (auton.mode != selected_mode) {
        return false;
    }

    if (selected_mode == AutonMode::Skills) {
        return true;
    }

    return auton.side == selected_side;
}

int AutonSelector::getFilteredAutonCount() const {
    int count = 0;

    for (const auto& auton : AUTONS) {
        if (autonMatchesSelection(auton)) {
            ++count;
        }
    }

    return count;
}

const AutonSelector::AutonDefinition*
AutonSelector::getFilteredAuton(int filteredIndex) const {
    if (filteredIndex < 0) {
        return nullptr;
    }

    int index = 0;

    for (const auto& auton : AUTONS) {
        if (!autonMatchesSelection(auton)) {
            continue;
        }

        if (index == filteredIndex) {
            return &auton;
        }

        ++index;
    }

    return nullptr;
}

// ------------------------------------------------------------
// SCROLLING
// ------------------------------------------------------------

int AutonSelector::getMaxScrollOffset() const {
    const int contentHeight = rowStep() * getFilteredAutonCount();
    const int visibleHeight = listBottom() - listTop();
    const int maximum = contentHeight - visibleHeight;

    return maximum > 0 ? maximum : 0;
}

void AutonSelector::clampScroll() {
    if (scroll_offset < 0) {
        scroll_offset = 0;
    }

    const int maximum = getMaxScrollOffset();

    if (scroll_offset > maximum) {
        scroll_offset = maximum;
    }
}

void AutonSelector::scrollBy(int amount) {
    scroll_offset += amount;
    clampScroll();
}

void AutonSelector::scrollUp() {
    scrollBy(-rowStep());
}

void AutonSelector::scrollDown() {
    scrollBy(rowStep());
}

void AutonSelector::drawAutonList() {
    const int count = getFilteredAutonCount();

    if (count == 0) {
        centeredText(ui::width() / 2, ui::height() / 2, "NO AUTONS");
        return;
    }

    clampScroll();

    int index = 0;

    for (const auto& auton : AUTONS) {
        if (!autonMatchesSelection(auton)) {
            continue;
        }

        const int y = listTop() + index * rowStep() - scroll_offset;

        if (y < listBottom() && y + rowHeight() > listTop()) {
            const Button row = {
                listLeft(),
                y,
                listRight(),
                y + rowHeight()
            };

            drawButton(
                row,
                auton.name,
                selected_mode == AutonMode::Skills
                    ? pros::Color::green : pros::Color::blue,
                auton.id == auton_number
            );
        }

        ++index;
    }

    const int maximum = getMaxScrollOffset();

    if (maximum > 0) {
        drawButton(layout(Element::ScrollUp), "^", pros::Color::grey);
        drawButton(layout(Element::ScrollDown), "V", pros::Color::grey);

        const int percentage = scroll_offset * 100 / maximum;
        char percentageText[8];

        std::snprintf(
            percentageText,
            sizeof(percentageText),
            "%d%%",
            percentage
        );

        ui::drawText(
            listLeft(),
            listBottom() + 3,
            percentageText,
            pros::Color::white
        );
    }
}

// ------------------------------------------------------------
// SELECTION
// ------------------------------------------------------------

void AutonSelector::selectCompetition() {
    selected_mode = AutonMode::Competition;
    selected_side = AutonSide::Loader;
    auton_number = -1;
    auton_confirmed = false;
    scroll_offset = 0;
    screen_number = SCREEN_COMPETITION_SIDE;
}

void AutonSelector::selectSkills() {
    selected_mode = AutonMode::Skills;
    auton_number = -1;
    auton_confirmed = false;
    scroll_offset = 0;
    screen_number = SCREEN_SKILLS_AUTONS;
}

void AutonSelector::selectLoader() {
    selected_side = AutonSide::Loader;
    auton_number = -1;
    auton_confirmed = false;
    scroll_offset = 0;
    screen_number = SCREEN_COMPETITION_AUTONS;
}

void AutonSelector::selectNonLoader() {
    selected_side = AutonSide::NonLoader;
    auton_number = -1;
    auton_confirmed = false;
    scroll_offset = 0;
    screen_number = SCREEN_COMPETITION_AUTONS;
}

void AutonSelector::selectAuton(int autonId) {
    for (const auto& auton : AUTONS) {
        if (auton.id != autonId) {
            continue;
        }

        if (!autonMatchesSelection(auton)) {
            return;
        }

        auton_number = autonId;
        auton_confirmed = false;
        screen_number = SCREEN_CONFIRM;
        return;
    }
}

bool AutonSelector::selectAutonAt(int x, int y) {
    if (x < listLeft() || x >= listRight() ||
        y < listTop() || y >= listBottom()) {
        return false;
    }

    const int contentY = y - listTop() + scroll_offset;
    const int filteredIndex = contentY / rowStep();
    const int rowOffset = contentY % rowStep();

    if (rowOffset >= rowHeight()) {
        return false;
    }

    const AutonDefinition* auton = getFilteredAuton(filteredIndex);

    if (auton == nullptr) {
        return false;
    }

    selectAuton(auton->id);
    return true;
}

// ------------------------------------------------------------
// BACK NAVIGATION
// ------------------------------------------------------------

void AutonSelector::goBack() {
    switch (screen_number) {
        case SCREEN_MODE:
            break;

        case SCREEN_COMPETITION_SIDE:
            screen_number = SCREEN_MODE;
            break;

        case SCREEN_COMPETITION_AUTONS:
            screen_number = SCREEN_COMPETITION_SIDE;
            break;

        case SCREEN_SKILLS_AUTONS:
            screen_number = SCREEN_MODE;
            break;

        case SCREEN_CONFIRM:
            screen_number = selected_mode == AutonMode::Competition
                ? SCREEN_COMPETITION_AUTONS
                : SCREEN_SKILLS_AUTONS;
            break;

        default:
            screen_number = SCREEN_MODE;
            selected_mode = AutonMode::Competition;
            selected_side = AutonSide::Loader;
            break;
    }

    auton_number = -1;
    auton_confirmed = false;
    scroll_offset = 0;
}

// ------------------------------------------------------------
// TOUCH HANDLERS
// ------------------------------------------------------------

bool AutonSelector::handleModeTouch(int x, int y) {
    if (isInside(x, y, layout(Element::ModeCompetition))) {
        selectCompetition();
        return true;
    }

    if (isInside(x, y, layout(Element::ModeSkills))) {
        selectSkills();
        return true;
    }

    return false;
}

bool AutonSelector::handleCompetitionSideTouch(int x, int y) {
    if (isInside(x, y, layout(Element::SideLoader))) {
        selectLoader();
        return true;
    }

    if (isInside(x, y, layout(Element::SideNonLoader))) {
        selectNonLoader();
        return true;
    }

    if (isInside(x, y, layout(Element::Back))) {
        goBack();
        return true;
    }

    return false;
}

bool AutonSelector::handleCompetitionAutonTouch(int x, int y) {
    if (isInside(x, y, layout(Element::ScrollUp))) {
        scrollUp();
        return true;
    }

    if (isInside(x, y, layout(Element::ScrollDown))) {
        scrollDown();
        return true;
    }

    if (selectAutonAt(x, y)) {
        return true;
    }

    if (isInside(x, y, layout(Element::Back))) {
        goBack();
        return true;
    }

    return false;
}

bool AutonSelector::handleSkillsAutonTouch(int x, int y) {
    if (isInside(x, y, layout(Element::ScrollUp))) {
        scrollUp();
        return true;
    }

    if (isInside(x, y, layout(Element::ScrollDown))) {
        scrollDown();
        return true;
    }

    if (selectAutonAt(x, y)) {
        return true;
    }

    if (isInside(x, y, layout(Element::Back))) {
        goBack();
        return true;
    }

    return false;
}

bool AutonSelector::handleConfirmTouch(int x, int y) {
    if (isInside(x, y, layout(Element::Back))) {
        goBack();
        return true;
    }

    if (isInside(x, y, layout(Element::Confirm))) {
        if (auton_number == -1) {
            return false;
        }

        auton_confirmed = true;
        return true;
    }

    return false;
}

void AutonSelector::handleTouch(int x, int y) {
    bool handled = false;

    switch (screen_number) {
        case SCREEN_MODE:
            handled = handleModeTouch(x, y);
            break;

        case SCREEN_COMPETITION_SIDE:
            handled = handleCompetitionSideTouch(x, y);
            break;

        case SCREEN_COMPETITION_AUTONS:
            handled = handleCompetitionAutonTouch(x, y);
            break;

        case SCREEN_SKILLS_AUTONS:
            handled = handleSkillsAutonTouch(x, y);
            break;

        case SCREEN_CONFIRM:
            handled = handleConfirmTouch(x, y);
            break;
    }

    if (handled) {
        render();
    }
}

// ------------------------------------------------------------
// GETTERS
// ------------------------------------------------------------

int AutonSelector::getSelectedAuton() const {
    return auton_number;
}

int AutonSelector::getScreen() const {
    return screen_number;
}

AutonSelector::AutonMode AutonSelector::getSelectedMode() const {
    return selected_mode;
}

AutonSelector::AutonSide AutonSelector::getSelectedSide() const {
    return selected_side;
}

bool AutonSelector::isAutonConfirmed() const {
    return auton_confirmed;
}

void AutonSelector::clearConfirmation() {
    auton_confirmed = false;
}
