#pragma once

#include "main.h"

class AutonSelector {
private:
    int screen_number;
    int auton_number;

public:
    void initialize();
    void render();
    void update();

    int getSelectedAuton();
    int getScreen();
};
