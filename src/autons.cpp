#include "../VEXtensions/autons.hpp"
#include "../VEXtensions/profiles.hpp"
#include "../VEXtensions/constants.hpp"
#include "vex.h"
using namespace vex;

/* Uses Legacy Code from https://www.vexforum.com/t/auton-selector/71511/5 (more specifically within the class notarized as "Button" and the lambda function that follows as well as lines 105 to 123 inside of the entry point)
int autonToRun = 0;

class Button {
  public:
    int x, y, width, height;
    std::string text;
    vex::color buttonColor, textColor;
    
    Button(int x, int y, int width, int height, std::string text, vex::color buttonColor, vex::color textColor)
    : x(x), y(y), width(width), height(height), text(text), buttonColor(buttonColor), textColor(textColor){}

    void render() {
      Brain.Screen.drawRectangle(x, y, width, height, buttonColor);
      Brain.Screen.printAt(x + 10, y + 10, false, text.c_str());
    }

    bool isClicked() {
      if(Brain.Screen.pressing() && Brain.Screen.xPosition() >= x && Brain.Screen.xPosition() <= x + width &&
      Brain.Screen.yPosition() >= y && Brain.Screen.yPosition() <= y + width) return true;
      return false;
    }
};

Button autonButtons[] = {
  Button(10, 10, 150, 50, "Auton Red 1", vex::green, vex::black),
  Button(170, 10, 150, 50, "Auton Red 2", vex::white, vex::black),
  Button(10, 70, 150, 50, "Auton Blue 1", vex::white, vex::black),
  Button(170, 70, 150, 50, "Auton Blue 2", vex::white, vex::black)
};

brain Brain;

int autonSelected = 0;

void drawSelector() {
    Brain.Screen.clearScreen();

    Brain.Screen.setFillColor(color::blue);
    Brain.Screen.drawRectangle(20, 60, 130, 100);

    Brain.Screen.setFillColor(color::red);
    Brain.Screen.drawRectangle(170, 60, 130, 100);

    Brain.Screen.setPenColor(color::white);
    Brain.Screen.printAt(55, 115, "LEFT");
    Brain.Screen.printAt(210, 115, "RIGHT");

    Brain.Screen.printAt(110, 220, "Selected: %s",
        autonSelected == 0 ? "LEFT" : "RIGHT");
}

void pre_auton() {
    vexcodeInit();

    drawSelector();

    while (!Competition.isEnabled()) {
        if (Brain.Screen.pressing()) {
            int x = Brain.Screen.xPosition();
            int y = Brain.Screen.yPosition();

            if (x >= 20 && x <= 150 && y >= 60 && y <= 160)
                autonSelected = 0;

            if (x >= 170 && x <= 300 && y >= 60 && y <= 160)
                autonSelected = 1;

            drawSelector();

            while (Brain.Screen.pressing())
                wait(20, msec);
        }

        wait(20, msec);
    }
}

void autonomous() {
    if (autonSelected == 0) {
    } else {
    }
}

void usercontrol() {
    while (true) {
        wait(20, msec);
    }
}

competition Competition;

int main() {
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);

    pre_auton();

    while(true) {
        Brain.Screen.clearScreen(vex::white);

        if(!Competition.isEnabled()) {
            for(int i = 0; i < 4; i++) {
                autonButtons[i].render();
                if(autonButtons[i].isClicked()) {
                autonButtons[autonToRun].buttonColor = vex::white;
                autonButtons[i].buttonColor = vex::green;
                autonToRun = i;
                }
            }
        }

        Brain.Screen.render();
        vex::task::sleep(7);
        wait(100, msec);
    }
} */

int autonSelection = 0;

const char* autonNames[] = {"Path 1: Auton Left", "Path 2: Auton Right", "Path 3: Skills"};
int numAutons = sizeof(autonNames) / sizeof(autonNames[0]);

void runAutonLeft() {
    Drivetrain.driveFor(forward, 24, inches);
    Intake.spin(forward, 100, percent);
    wait(1.0, sec);
    
    Drivetrain.driveFor(reverse, 12, inches);
    Drivetrain.turnFor(right, 90, degrees);
    
    Drivetrain.driveFor(forward, 18, inches);
    
    TogglePneumatic.set(true); 
    wait(0.5, sec);
    Intake.stop();
}

void runAutonRight() {
    Drivetrain.driveFor(forward, 36, inches);
    Intake.spin(forward, 100, percent);
    wait(1.2, sec);
    
    Drivetrain.turnFor(left, 45, degrees);
    Drivetrain.driveFor(reverse, 15, inches);
    Intake.stop();
}

void runSkills() {
    Intake.spin(forward, 100, percent);
    Drivetrain.driveFor(forward, 24, inches);
    Drivetrain.turnFor(right, 90, degrees);
    TogglePneumatic.set(true);
    wait(0.5, sec);
    
    Drivetrain.driveFor(forward, 48, inches);

    Drivetrain.turnFor(left, 90, degrees);
    Drivetrain.driveFor(forward, 36, inches); 
    HangMechanism.spinFor(forward, 2, turns);
}

void drawAutonSelector() {
    Brain.Screen.clearScreen();
    Brain.Screen.setFillColor(color::black);
    
    // draws "Previous" button
    Brain.Screen.setPenColor(color::white);
    Brain.Screen.drawRect(20, 80, 100, 50);
    Brain.Screen.printAt(45, 110, "< Prev");

    // draws "Next" button
    Brain.Screen.drawRect(360, 80, 100, 50);
    Brain.Screen.printAt(390, 110, "Next >");

    // displays the Current Selection
    Brain.Screen.setFont(fontType::mono20);
    Brain.Screen.printAt(150, 40, "Selected Auton:");
    Brain.Screen.printAt(150, 70, autonNames[autonSelection]);
}

void handleScreenPresses() {
    int x = Brain.Screen.xPosition();
    int y = Brain.Screen.yPosition();

    // check if "< Prev" BTN was pressed
    if (x >= 20 && x <= 120 && y >= 80 && y <= 130) {
        autonSelection--;
        if (autonSelection < 0) autonSelection = numAutons - 1; 
    }
    // check if "Next >" BTN was pressed
    if (x >= 360 && x <= 460 && y >= 80 && y <= 130) {
        autonSelection++;
        if (autonSelection >= numAutons) autonSelection = 0;
    }
    
    drawAutonSelector();
}

void pre_auton(void) {
    // initializing hardware
    vexcodeInit();
    
    // draws initial selector on screen
    drawAutonSelector();
    
    // register screen touch events
    Brain.Screen.pressed(handleScreenPresses);
}

void autonomous(void) {
    // stop the brain screen from intercepting presses during the match
    Brain.Screen.pressed(NULL); 

    // execution callback: runs code block tied to your UI choices
    switch(autonSelection) {
        case 0:
            runAutonLeft();
            break;
        case 1:
            runAutonRight();
            break;
        case 2:
            runSkills();
            break;
        default:
            // safety fallback: stops motors if index glitches out
            Drivetrain.stop();
            Intake.stop();
            break;
    }
}