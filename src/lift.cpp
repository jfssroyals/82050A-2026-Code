# include "lift.hpp"

// Defining Motor Groups (1 and -2 are placeholders)
pros::MotorGroup liftMotors({1, -2}, pros::MotorGearset::red);
// 3 is also a placeholder for sender plug
pros::Rotation liftRotation(3);


//Defining Stage Heights with constants

    // 6.5 in 
const int Stage_Height_1 = 2500; 
    // 13 in
const int Stage_Height_2 = 5200;
    //26 in
const int Stage_Height_3 = 10500;
    //39 in
const int Stage_Height_4 = 16000;


// Setting up PID for lemlib with variables
// Variable 1: proportional (placeholder), 2: Integral (placeholder), 3:Derivative (placeholder), 4:Antiwindup, 5: SIgn-Flip not needed for lift
lemlib::PID liftPID(0.018, 0.0, 0.08, 0, false);

// Stores the target sensor value the lift is currently trying to reach
int LiftTargetHeight = 0;

//MAIN FUNCTION 
    //Run by Autonomous decision of stage

void setLiftStage(int Stage){
    if (Stage==1){
        LiftTargetHeight = Stage_Height_1;
    }else if (Stage==2){
        LiftTargetHeight = Stage_Height_2;
    }else if (Stage==3){
        LiftTargetHeight = Stage_Height_3;
    }else if (Stage==4){
        LiftTargetHeight = Stage_Height_4;
    }

}

//Background Loop
void liftcontrolloop(){
    while (true){
        // Current Position of Lift
        double currentPosition = liftRotation.get_position();
        // Calculate the error (Target minus where we are right now)
        double error = LiftTargetHeight - currentPosition;
        // Feed ONLY the error into LemLib's PID calculator
        double motorPower = liftPID.update(error);
        liftMotors.move(motorPower);
        pros::delay(20);
    }
}