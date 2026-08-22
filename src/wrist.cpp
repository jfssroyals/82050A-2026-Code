#include "wrist.hpp"

Wrist::Wrist(char port)
    : piston(port) {}

void Wrist::up() {
    piston.set_value(true);
    isUp = true;
}

void Wrist::down() {
    piston.set_value(false);
    isUp = false;
}

void Wrist::toggle() {
    if (isUp) {
        down();
    } else {
        up();
    }
}

bool Wrist::getState() const {
    return isUp;
}