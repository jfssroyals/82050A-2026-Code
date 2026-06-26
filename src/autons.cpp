// #include "../VEXtensions/autons.hpp"
// #include "../VEXtensions/profiles.hpp"
// #include "../VEXtensions/constants.hpp"
// #include "vex.h"
// using namespace vex;

// // Uses Legacy Code from https://www.vexforum.com/t/auton-selector/71511/5 (more specifically within the class notarized as "Button" and the lambda function that follows as well as lines 105 to 123 inside of the entry point)
// int autonToRun = 0;

// class Button {
//   public:
//     int x, y, width, height;
//     std::string text;
//     vex::color buttonColor, textColor;
    
//     Button(int x, int y, int width, int height, std::string text, vex::color buttonColor, vex::color textColor)
//     : x(x), y(y), width(width), height(height), text(text), buttonColor(buttonColor), textColor(textColor){}

//     void render() {
//       Brain.Screen.drawRectangle(x, y, width, height, buttonColor);
//       Brain.Screen.printAt(x + 10, y + 10, false, text.c_str());
//     }

//     bool isClicked() {
//       if(Brain.Screen.pressing() && Brain.Screen.xPosition() >= x && Brain.Screen.xPosition() <= x + width &&
//       Brain.Screen.yPosition() >= y && Brain.Screen.yPosition() <= y + width) return true;
//       return false;
//     }
// };

// Button autonButtons[] = {
//   Button(10, 10, 150, 50, "Auton Red 1", vex::green, vex::black),
//   Button(170, 10, 150, 50, "Auton Red 2", vex::white, vex::black),
//   Button(10, 70, 150, 50, "Auton Blue 1", vex::white, vex::black),
//   Button(170, 70, 150, 50, "Auton Blue 2", vex::white, vex::black)
// };

// brain Brain;

// int autonSelected = 0;

// void drawSelector() {
//     Brain.Screen.clearScreen();

//     Brain.Screen.setFillColor(color::blue);
//     Brain.Screen.drawRectangle(20, 60, 130, 100);

//     Brain.Screen.setFillColor(color::red);
//     Brain.Screen.drawRectangle(170, 60, 130, 100);

//     Brain.Screen.setPenColor(color::white);
//     Brain.Screen.printAt(55, 115, "LEFT");
//     Brain.Screen.printAt(210, 115, "RIGHT");

//     Brain.Screen.printAt(110, 220, "Selected: %s",
//         autonSelected == 0 ? "LEFT" : "RIGHT");
// }

// void pre_auton() {
//     vexcodeInit();

//     drawSelector();

//     while (!Competition.isEnabled()) {
//         if (Brain.Screen.pressing()) {
//             int x = Brain.Screen.xPosition();
//             int y = Brain.Screen.yPosition();

//             if (x >= 20 && x <= 150 && y >= 60 && y <= 160)
//                 autonSelected = 0;

//             if (x >= 170 && x <= 300 && y >= 60 && y <= 160)
//                 autonSelected = 1;

//             drawSelector();

//             while (Brain.Screen.pressing())
//                 wait(20, msec);
//         }

//         wait(20, msec);
//     }
// }

// void autonomous() {
//     if (autonSelected == 0) {
//     } else {
//     }
// }

// void usercontrol() {
//     while (true) {
//         wait(20, msec);
//     }
// }

// competition Competition;

// int main() {
//     Competition.autonomous(autonomous);
//     Competition.drivercontrol(usercontrol);

//     pre_auton();

//     while(true) {
//         Brain.Screen.clearScreen(vex::white);

//         if(!Competition.isEnabled()) {
//             for(int i = 0; i < 4; i++) {
//                 autonButtons[i].render();
//                 if(autonButtons[i].isClicked()) {
//                 autonButtons[autonToRun].buttonColor = vex::white;
//                 autonButtons[i].buttonColor = vex::green;
//                 autonToRun = i;
//                 }
//             }
//         }

//         Brain.Screen.render();
//         vex::task::sleep(7);
//         wait(100, msec);
//     }
// }