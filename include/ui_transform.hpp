
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

inline Orientation currentOrientation = Orientation::Landscape;

inline int width() {
    return currentOrientation == Orientation::Landscape
        ? PHYSICAL_WIDTH : PHYSICAL_HEIGHT;
}

inline int height() {
    return currentOrientation == Orientation::Landscape
        ? PHYSICAL_HEIGHT : PHYSICAL_WIDTH;
}

inline Point toPhysical(int x, int y) {
    if (currentOrientation == Orientation::Landscape) {
        return {x, y};
    }

    // Clockwise 90-degree rotation.
    return {PHYSICAL_WIDTH - 1 - y, x};
}

inline Point toVirtual(int physicalX, int physicalY) {
    if (currentOrientation == Orientation::Landscape) {
        return {physicalX, physicalY};
    }

    return {physicalY, PHYSICAL_WIDTH - 1 - physicalX};
}

} // namespace ui
