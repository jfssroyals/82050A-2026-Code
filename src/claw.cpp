#include "claw.hpp"


// WHEN PISTON VALUE IS TRUE THE CLAW IS OPEN, WHEN FALSE THE CLAW IS CLOSED
// When PISTON VALUE IS TRUE, THE PISTON IS RETRACTED
Claw::Claw(char port)
    : piston(port) {}

void Claw::open() {
    printf("open()\n");
    piston.set_value(true);
    isExtended = true;
}  

void Claw::close() {
    printf("close()\n");
    piston.set_value(false);
    isExtended = false;
}


void Claw::toggle() {
    isExtended = !isExtended;
    piston.set_value(isExtended); // true opens
}
