#include <main.h>
#include <source_location>
#include <sys/syslimits.h>
#include "lemlib/api.hpp"
#include "api.h"
#include "pros/adi.h"
#include "pros/adi.hpp"
#define PISTON_PORT 'A'

using pros::E_ADI_DIGITAL_OUT;

// creates the claw as a class of objects
class Claw
{
private:
    // declares the piston
    pros::adi::DigitalOut piston;
    // encapsulates status  
    bool pist_open;

public:
    // constructor 
    // initializes the claw, port, and status of claw 
    // claw starts off open

    Claw() : piston(PISTON_PORT), pist_open(true) {
    piston.set_value(true);
    }

    void open()
    {
        if (!pist_open)
        {
            piston.set_value(true);
            pist_open = true;
        }    
    }

    void close()
    {
        if (pist_open)
        {
            piston.set_value(false);
            pist_open = false; 
        }
    }

    // get rid of it if you want but we can change ts later bro
    void toggle()
    {
        if (pist_open)
        {
            close();
        }

        else
        {
            open();
        }
    }


};


// NOTE: if piston logic is messed up - it starts closed and does the opposite of what it's supposed to do
// just check the 3 functions
// don't touch the constructor 

//piston opens
//    check piston status
  //  change the piston status


// piston closes