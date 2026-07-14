#pragma once
#include "main.h"

class Intake {
    private:
        pros::Motor motor;

    public:
        Intake(int motorPort);

        // Functions
        void spinInward();
        void spinOutward();
        void stop();
        
        // Function to handle driver inputs 
        void update(pros::Controller& controller);
};