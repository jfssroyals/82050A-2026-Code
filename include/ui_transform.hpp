#pragma once

namespace ui {
constexpr int PHYSICAL_WIDTH = 480;
constexpr int PHYSICAL_HEIGHT = 240;

enum class Orientation {
    Landscape,
    Portrait
};

struct Point {
    int x;
    int y;
};

// One shared orientation state for the whole UI.
extern Orientation currentOrientation;

int width();
int height();

// Convert virtual UI coordinates to the physical 480x240 touchscreen.
Point toPhysical(int x, int y);

// Convert physical touchscreen coordinates to virtual UI coordinates.
Point toVirtual(int physicalX, int physicalY);
}
