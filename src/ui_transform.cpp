#include "ui_transform.hpp"

namespace ui {
Orientation currentOrientation = Orientation::Landscape;

int width() {
    return currentOrientation == Orientation::Landscape
        ? PHYSICAL_WIDTH : PHYSICAL_HEIGHT;
}

int height() {
    return currentOrientation == Orientation::Landscape
        ? PHYSICAL_HEIGHT : PHYSICAL_WIDTH;
}

Point toPhysical(int x, int y) {
    if (currentOrientation == Orientation::Landscape) {
        return {x, y};
    }

    // Clockwise rotation of the 240x480 virtual portrait canvas.
    return {PHYSICAL_WIDTH - 1 - y, x};
}

Point toVirtual(int physicalX, int physicalY) {
    if (currentOrientation == Orientation::Landscape) {
        return {physicalX, physicalY};
    }

    // Inverse of toPhysical().
    return {physicalY, PHYSICAL_WIDTH - 1 - physicalX};
}
}
