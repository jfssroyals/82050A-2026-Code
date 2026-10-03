#include "wrist.hpp"

Wrist::Wrist(char port)
    : piston(port) {}

void Wrist::moveUP(){
    piston.set_value(false);
    isUp = true;
}

void Wrist::moveDOWN(){
    piston.set_value(true);
    isUp = false;
}