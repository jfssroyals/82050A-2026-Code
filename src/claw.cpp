#include "claw.hpp"


// WHEN PISTON VALUE IS TRUE THE CLAW IS OPEN, WHEN FALSE THE CLAW IS CLOSED
// When PISTON VALUE IS TRUE, THE PISTON IS RETRACTED
Claw::Claw(char port)
    : piston(port) {}

void Claw::open() {
    piston.set_value(true);
    isExtended = true;
}  

void Claw::close() {
    piston.set_value(false);
    isExtended = false;
}
