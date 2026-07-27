#include "claw.hpp"

Claw::Claw(char port)
    : piston(port) {}

void Claw::open() {
    printf("open()\n");
    piston.set_value(false);
}

void Claw::close() {
    printf("close()\n");
    piston.set_value(true);
}

void Claw::toggle() {
    isOpen = !isOpen;
    piston.set_value(isOpen);
}

bool Claw::isopen() {
    return isOpen;
}