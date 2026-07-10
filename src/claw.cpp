#include "claw.hpp"

Claw::Claw(char port)
    : piston(port) {}

void Claw::open() {
    printf("open()\n");
    piston.set_value(true);
    isOpen = true;
}

void Claw::close() {
    printf("close()\n");
    piston.set_value(false);
    isOpen = false;
}

void Claw::toggle() {
    isOpen = !isOpen;
    piston.set_value(isOpen);
}