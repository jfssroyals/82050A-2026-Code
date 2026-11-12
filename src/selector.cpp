#include "selector.hpp"
#include "ui_renderer.hpp"

#include <cstdio>
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
constexpr int LEBRON = 8;
constexpr int DINGALINGA = 9;
constexpr int IMCRACKED = 10;

void centeredText(int centerX, int y, const char* text,
                  pros::Color color = pros::Color::white) {
    if (text == nullptr) return;
    const int textWidth = static_cast<int>(std::strlen(text)) * 6;
    ui::drawText(centerX - textWidth / 2, y, text, color);
}
}

const std::vector<AutonSelector::AutonDefinition> AutonSelector::AUTONS = {
    {FOURPIN_RED1, "FOURPIN_RED1", AutonMode::Competition, AutonSide::Loader},
    {AUTON_BLUE_2, "BLUE 2", AutonMode::Competition, AutonSide::NonLoader},
    {AUTON_RED_1, "RED 1", AutonMode::Competition, AutonSide::Loader},
    {AUTON_RED_2, "RED 2", AutonMode::Competition, AutonSide::NonLoader},
    {AUTON_SKILLS, "SKILLS", AutonMode::Skills, AutonSide::Loader},
    {BRRR, "BRRR", AutonMode::Competition, AutonSide::NonLoader},
    {CHOTTU, "CHOTTU", AutonMode::Competition, AutonSide::NonLoader},
    {FILLER, "FILLER", AutonMode::Competition, AutonSide::NonLoader},
    {LEBRON, "LEBRON", AutonMode::Competition, AutonSide::NonLoader},
    {DINGALINGA, "DINGALINGA", AutonMode::Competition, AutonSide::NonLoader},
    {IMCRACKED, "IMCRACKED", AutonMode::Competition, AutonSide::Loader}
};

AutonSelector::AutonSelector() = default;

void AutonSelector::setOrientation(Orientation orientation) {
    ui::currentOrientation = orientation;
    render();
}

void AutonSelector::toggleOrientation() {
    ui::currentOrientation =
        ui::currentOrientation == Orientation::Landscape
            ? Orientation::Portrait
            : Orientation::Landscape;
    render();
}

AutonSelector::Orientation AutonSelector::getCurrentOrientation() const {
    return ui::currentOrientation;
}

AutonSelector::Button AutonSelector::layout(Element element) const {
    const bool portrait = ui::currentOrientation == Orientation::Portrait;

    if (element == Element::Rotate) {
        return portrait ? Button{158, 5, 235, 31}
                        : Button{390, 5, 475, 30};
    }

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
            case Element::Rotate:          break;
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
            case Element::Rotate:          break;
        }
    }
    return {0, 0, 0, 0};
}

int AutonSelector::listLeft() const {
    return ui::currentOrientation == Orientation::Portrait ? 8 : 20;
}
int AutonSelector::listRight() const {
    return ui::currentOrientation == Orientation::Portrait ? 192 : 420;
}
int AutonSelector::listTop() const {
    return ui::currentOrientation == Orientation::Portrait ? 78 : 55;
}
int AutonSelector::listBottom() const {
    return ui::currentOrientation == Orientation::Portrait ? 410 : 180;
}
int AutonSelector::rowHeight() const {
    return ui::currentOrientation == Orientation::Portrait ? 28 : 35;
}
int AutonSelector::rowStep() const {
    return ui::currentOrientation == Orientation::Portrait ? 33 : 42;
}

bool AutonSelector::isInside(int x, int y, const Button& b) {
    return x >= b.left && x < b.right && y >= b.top && y < b.bottom;
}
int AutonSelector::getCenterX(const Button& b) {
    return b.left + (b.right - b.left) / 2;
}
int AutonSelector::getCenterY(const Button& b) {
    return b.top + (b.bottom - b.top) / 2;
}

void AutonSelector::drawButton(const Button& b, const char* text,
                               pros::Color color, bool selected) {
    if (text == nullptr) return;
    const pros::Color fill = selected ? pros::Color::green : color;
    ui::fillRect(b.left, b.top, b.right - 1, b.bottom - 1, fill);
    ui::drawRect(b.left, b.top, b.right - 1, b.bottom - 1,
                 pros::Color::white);
    const int textWidth = static_cast<int>(std::strlen(text)) * 6;
    ui::drawText(getCenterX(b) - textWidth / 2, getCenterY(b) - 3,
                 text, pros::Color::white);
}

void AutonSelector::drawRotateButton() {
    drawButton(layout(Element::Rotate), "ROTATE", pros::Color::grey);
}

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
        touch_task = new pros::Task([this] { touchTaskLoop(); });
    }
}

void AutonSelector::touchTaskLoop() {
    while (true) {
        update();
        pros::delay(20);
    }
}

void AutonSelector::update() {
    const pros::screen_touch_status_s touch = pros::screen::touch_status();

    if (touch.touch_status) {
        const ui::Point p = ui::toVirtual(touch.x, touch.y);
        if (!touch_was_pressed) {
            touch_was_pressed = true;
            touch_start_x = p.x;
            touch_start_y = p.y;
        }
        touch_current_x = p.x;
        touch_current_y = p.y;
        return;
    }

    if (!touch_was_pressed) return;
    touch_was_pressed = false;

    const int dx = touch_current_x - touch_start_x;
    const int dy = touch_current_y - touch_start_y;
    const bool onList = screen_number == SCREEN_COMPETITION_AUTONS ||
                        screen_number == SCREEN_SKILLS_AUTONS;

    if (onList && std::abs(dx) < SWIPE_THRESHOLD &&
        std::abs(dy) >= SWIPE_THRESHOLD) {
        scrollBy(dy < 0 ? rowStep() * 2 : -rowStep() * 2);
        render();
        return;
    }

    if (std::abs(dx) < SWIPE_THRESHOLD && std::abs(dy) < SWIPE_THRESHOLD) {
        handleTouch(touch_current_x, touch_current_y);
    }
}

void AutonSelector::render() {
    ui::clear();

    switch (screen_number) {
        case SCREEN_MODE: drawModeScreen(); break;
        case SCREEN_COMPETITION_SIDE: drawCompetitionSideScreen(); break;
        case SCREEN_COMPETITION_AUTONS: drawCompetitionAutonScreen(); break;
        case SCREEN_SKILLS_AUTONS: drawSkillsAutonScreen(); break;
        case SCREEN_CONFIRM: drawConfirmScreen(); break;
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
    drawRotateButton();
}

void AutonSelector::drawModeScreen() {
    const bool p = ui::currentOrientation == Orientation::Portrait;
    centeredText(ui::width() / 2, p ? 35 : 12, "SELECT MODE");
    centeredText(ui::width() / 2, p ? 55 : 35, "CHOOSE AUTON TYPE");
    drawButton(layout(Element::ModeCompetition), "COMPETITION",
               pros::Color::blue, selected_mode == AutonMode::Competition);
    drawButton(layout(Element::ModeSkills), "SKILLS",
               pros::Color::green, selected_mode == AutonMode::Skills);
}

void AutonSelector::drawCompetitionSideScreen() {
    const bool p = ui::currentOrientation == Orientation::Portrait;
    centeredText(ui::width() / 2, p ? 45 : 12, "COMPETITION");
    centeredText(ui::width() / 2, p ? 63 : 35, "SELECT AUTON SIDE");
    drawButton(layout(Element::SideLoader), "LOADER", pros::Color::blue,
               selected_side == AutonSide::Loader);
    drawButton(layout(Element::SideNonLoader), "NON-LOADER", pros::Color::blue,
               selected_side == AutonSide::NonLoader);
    drawButton(layout(Element::Back), "BACK", pros::Color::grey);
}

void AutonSelector::drawCompetitionAutonScreen() {
    const bool p = ui::currentOrientation == Orientation::Portrait;
    centeredText(ui::width() / 2, p ? 20 : 7, "COMPETITION AUTONS");
    centeredText(ui::width() / 2, p ? 42 : 29,
                 selected_side == AutonSide::Loader ? "LOADER SIDE" : "NON-LOADER SIDE");
    drawAutonList();
    drawButton(layout(Element::Back), "BACK", pros::Color::grey);
}

void AutonSelector::drawSkillsAutonScreen() {
    const bool p = ui::currentOrientation == Orientation::Portrait;
    centeredText(ui::width() / 2, p ? 20 : 7, "SKILLS AUTONS");
    centeredText(ui::width() / 2, p ? 42 : 29, "SELECT A ROUTINE");
    drawAutonList();
    drawButton(layout(Element::Back), "BACK", pros::Color::grey);
}

void AutonSelector::drawConfirmScreen() {
    const bool p = ui::currentOrientation == Orientation::Portrait;
    centeredText(ui::width() / 2, p ? 35 : 12, "CONFIRM AUTON");

    const AutonDefinition* chosen = nullptr;
    for (const auto& a : AUTONS) {
        if (a.id == auton_number) { chosen = &a; break; }
    }

    if (chosen != nullptr) {
        centeredText(ui::width() / 2, p ? 120 : 65, chosen->name);
        centeredText(ui::width() / 2, p ? 155 : 100,
                     selected_mode == AutonMode::Competition ? "COMPETITION" : "SKILLS");
        if (selected_mode == AutonMode::Competition) {
            centeredText(ui::width() / 2, p ? 180 : 125,
                         selected_side == AutonSide::Loader ? "LOADER SIDE" : "NON-LOADER SIDE");
        }
        centeredText(ui::width() / 2, p ? 230 : 160,
                     auton_confirmed ? "AUTON CONFIRMED" : "PRESS CONFIRM",
                     auton_confirmed ? pros::Color::green : pros::Color::yellow);
    }

    drawButton(layout(Element::Back), "BACK", pros::Color::grey);
    drawButton(layout(Element::Confirm), auton_confirmed ? "CONFIRMED" : "CONFIRM",
               pros::Color::green, auton_confirmed);
}

bool AutonSelector::autonMatchesSelection(const AutonDefinition& a) const {
    if (a.mode != selected_mode) return false;
    if (selected_mode == AutonMode::Skills) return true;
    return a.side == selected_side;
}

int AutonSelector::getFilteredAutonCount() const {
    int n = 0;
    for (const auto& a : AUTONS) if (autonMatchesSelection(a)) ++n;
    return n;
}

const AutonSelector::AutonDefinition*
AutonSelector::getFilteredAuton(int filteredIndex) const {
    if (filteredIndex < 0) return nullptr;
    int index = 0;
    for (const auto& a : AUTONS) {
        if (!autonMatchesSelection(a)) continue;
        if (index == filteredIndex) return &a;
        ++index;
    }
    return nullptr;
}

int AutonSelector::getMaxScrollOffset() const {
    const int contentHeight = rowStep() * getFilteredAutonCount();
    const int visibleHeight = listBottom() - listTop();
    const int max = contentHeight - visibleHeight;
    return max > 0 ? max : 0;
}

void AutonSelector::clampScroll() {
    if (scroll_offset < 0) scroll_offset = 0;
    const int max = getMaxScrollOffset();
    if (scroll_offset > max) scroll_offset = max;
}
void AutonSelector::scrollBy(int amount) {
    scroll_offset += amount;
    clampScroll();
}
void AutonSelector::scrollUp() { scrollBy(-rowStep()); }
void AutonSelector::scrollDown() { scrollBy(rowStep()); }

void AutonSelector::drawAutonList() {
    const int count = getFilteredAutonCount();
    if (count == 0) {
        centeredText(ui::width() / 2, ui::height() / 2, "NO AUTONS");
        return;
    }

    clampScroll();
    int index = 0;
    for (const auto& a : AUTONS) {
        if (!autonMatchesSelection(a)) continue;
        const int y = listTop() + index * rowStep() - scroll_offset;
        if (y < listBottom() && y + rowHeight() > listTop()) {
            const Button row{listLeft(), y, listRight(), y + rowHeight()};
            drawButton(row, a.name,
                       selected_mode == AutonMode::Skills ? pros::Color::green : pros::Color::blue,
                       a.id == auton_number);
        }
        ++index;
    }

    if (getMaxScrollOffset() > 0) {
        drawButton(layout(Element::ScrollUp), "^", pros::Color::grey);
        drawButton(layout(Element::ScrollDown), "V", pros::Color::grey);
        const int pct = scroll_offset * 100 / getMaxScrollOffset();
        char text[8];
        std::snprintf(text, sizeof(text), "%d%%", pct);
        ui::drawText(listLeft(), listBottom() + 3, text, pros::Color::white);
    }
}

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

void AutonSelector::selectAuton(int id) {
    for (const auto& a : AUTONS) {
        if (a.id != id) continue;
        if (!autonMatchesSelection(a)) return;
        auton_number = id;
        auton_confirmed = false;
        screen_number = SCREEN_CONFIRM;
        return;
    }
}

bool AutonSelector::selectAutonAt(int x, int y) {
    if (x < listLeft() || x >= listRight() || y < listTop() || y >= listBottom()) {
        return false;
    }
    const int contentY = y - listTop() + scroll_offset;
    const int filteredIndex = contentY / rowStep();
    if (contentY % rowStep() >= rowHeight()) return false;
    const AutonDefinition* a = getFilteredAuton(filteredIndex);
    if (a == nullptr) return false;
    selectAuton(a->id);
    return true;
}

void AutonSelector::goBack() {
    switch (screen_number) {
        case SCREEN_MODE:
            return;
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
                ? SCREEN_COMPETITION_AUTONS : SCREEN_SKILLS_AUTONS;
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

bool AutonSelector::handleModeTouch(int x, int y) {
    if (isInside(x, y, layout(Element::ModeCompetition))) { selectCompetition(); return true; }
    if (isInside(x, y, layout(Element::ModeSkills))) { selectSkills(); return true; }
    return false;
}
bool AutonSelector::handleCompetitionSideTouch(int x, int y) {
    if (isInside(x, y, layout(Element::SideLoader))) { selectLoader(); return true; }
    if (isInside(x, y, layout(Element::SideNonLoader))) { selectNonLoader(); return true; }
    if (isInside(x, y, layout(Element::Back))) { goBack(); return true; }
    return false;
}
bool AutonSelector::handleCompetitionAutonTouch(int x, int y) {
    if (getMaxScrollOffset() > 0 && isInside(x, y, layout(Element::ScrollUp))) { scrollUp(); return true; }
    if (getMaxScrollOffset() > 0 && isInside(x, y, layout(Element::ScrollDown))) { scrollDown(); return true; }
    if (selectAutonAt(x, y)) return true;
    if (isInside(x, y, layout(Element::Back))) { goBack(); return true; }
    return false;
}
bool AutonSelector::handleSkillsAutonTouch(int x, int y) {
    if (getMaxScrollOffset() > 0 && isInside(x, y, layout(Element::ScrollUp))) { scrollUp(); return true; }
    if (getMaxScrollOffset() > 0 && isInside(x, y, layout(Element::ScrollDown))) { scrollDown(); return true; }
    if (selectAutonAt(x, y)) return true;
    if (isInside(x, y, layout(Element::Back))) { goBack(); return true; }
    return false;
}
bool AutonSelector::handleConfirmTouch(int x, int y) {
    if (isInside(x, y, layout(Element::Back))) { goBack(); return true; }
    if (isInside(x, y, layout(Element::Confirm))) {
        if (auton_number < 0) return false;
        auton_confirmed = true;
        return true;
    }
    return false;
}

void AutonSelector::handleTouch(int x, int y) {
    // Check Rotate before screen-specific controls so it works on every screen.
    if (isInside(x, y, layout(Element::Rotate))) {
        toggleOrientation();
        return;
    }

    bool handled = false;
    switch (screen_number) {
        case SCREEN_MODE: handled = handleModeTouch(x, y); break;
        case SCREEN_COMPETITION_SIDE: handled = handleCompetitionSideTouch(x, y); break;
        case SCREEN_COMPETITION_AUTONS: handled = handleCompetitionAutonTouch(x, y); break;
        case SCREEN_SKILLS_AUTONS: handled = handleSkillsAutonTouch(x, y); break;
        case SCREEN_CONFIRM: handled = handleConfirmTouch(x, y); break;
    }
    if (handled) render();
}

int AutonSelector::getSelectedAuton() const { return auton_number; }
int AutonSelector::getConfirmedAuton() const {
    return auton_confirmed ? auton_number : -1;
}
int AutonSelector::getScreen() const { return screen_number; }
AutonSelector::AutonMode AutonSelector::getSelectedMode() const { return selected_mode; }
AutonSelector::AutonSide AutonSelector::getSelectedSide() const { return selected_side; }
bool AutonSelector::isAutonConfirmed() const { return auton_confirmed; }
void AutonSelector::clearConfirmation() { auton_confirmed = false; }
