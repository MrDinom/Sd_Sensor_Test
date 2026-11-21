/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       reese                                                     */
/*    Created:      11/5/2025, 9:38:16 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
vex::brain Brain;

// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.


// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}
 


void vexcodeInit() {

  //Initializing random seed.
  initializeRandomSeed(); 
}

// #include "SD.h"
// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

#pragma endregion VEXcode Generated Robot Configuration

// Include the V5 Library

  
// Allows for easier use of the VEX Library

// A global instance of competition
int Screen_precision = 0, Console_precision = 0, Controller1_precision = 0;

const char* printToController1_numberFormat() {
  // look at the current precision setting to find the format string
  switch(Controller1_precision){
    case 0:  return "%.0f"; // 0 decimal places (1)
    case 1:  return "%.1f"; // 1 decimal place  (0.1)
    case 2:  return "%.2f"; // 2 decimal places (0.01)
    case 3:  return "%.3f"; // 3 decimal places (0.001)
    default: return "%f"; // use the print system default for everthing else
  }
}

competition Competition;
float LiftVariR, LiftVariL, SpeedChange, TurnChange, Twait;
// define your global instances of motors and other devices here
controller Controller1 = controller(primary);
controller Controller2 = controller(partner);
motor FL10 = motor(PORT10, ratio6_1, true);
motor BL16 = motor(PORT16, ratio6_1, true);
motor FR1 = motor(PORT1, ratio6_1, false);
motor BR19 = motor(PORT19, ratio6_1, false);
motor Llift = motor(PORT15, ratio36_1, false);
motor Rlift = motor(PORT16, ratio36_1, false); //reverse later
motor_group Lift = motor_group(Rlift, Llift);
motor claw = motor(PORT2, ratio36_1, true);
motor_group RD = motor_group(FR1, BR19);
motor_group LD = motor_group(FL10, BL16);
drivetrain Drivetrain = drivetrain(LD, RD, 299.24, 320, 40, mm, 0.75);
bool clawOpen = false;

void Liftup() {
  Lift.spin(forward);
  waitUntil((!Controller1.ButtonL1.pressing()));
  Lift.stop(hold);
}

void LiftD() {
  Lift.spin(reverse);
  waitUntil((!Controller1.ButtonL2.pressing()));
  Lift.stop(hold);
}

void clawO() {
  claw.spin(reverse);
  waitUntil((!Controller1.ButtonR1.pressing()));
  claw.stop(hold);
}

void clawC() {
  claw.spin(forward);
  waitUntil((!Controller1.ButtonR2.pressing()));
  claw.stop(hold);
}

void VD() {
  if (Twait == 1) {
    Twait = 0;
  SpeedChange = SpeedChange + -0.10;
      wait(50, msec);
    Twait = 1;
  }
}

void VU() {
  if (Twait == 1) {
    Twait = 0;
  SpeedChange = SpeedChange + 0.10;
      wait(50, msec);
    Twait = 1;
  }
}

void TD() {
  if (Twait == 1) {
    Twait = 0;
    TurnChange = TurnChange + -0.10;
    wait(50, msec);
    Twait = 1;
  }
}

void TU() {
  if (Twait == 1) {
    Twait = 0;
    TurnChange = TurnChange + 0.10;
    wait(50, msec);
    Twait = 1;
  }
}
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  Lift.setVelocity(25, percent);
  Lift.setMaxTorque(100, percent);
  claw.setVelocity(40, percent);
  claw.setMaxTorque(100, percent);
  claw.spin(reverse);
  wait(1000, msec);
  Lift.stop(hold);
  claw.stop();
  SpeedChange = 1;
  TurnChange = 1;
  Controller1_precision = 2;
  Twait = 1;
  while (1) {
    Controller2.Screen.setCursor(1, 1);
    Controller2.Screen.print(printToController1_numberFormat(), static_cast<float>(SpeedChange));
    Controller2.Screen.setCursor(2, 1);
    Controller2.Screen.print(printToController1_numberFormat(), static_cast<float>(TurnChange));
    Controller2.Screen.clearScreen();
    
  }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  Lift.setVelocity(25, percent);
  LD.setVelocity(25, percent);
  RD.setVelocity(25, percent);
  Drivetrain.driveFor(forward, 34, inches);
  claw.spin(forward);  // grab first pin
  wait(500, msec);
  claw.stop(hold);
  Lift.spin(forward);
  wait(0.6, sec);
  Lift.stop(hold);
  Drivetrain.turnFor(-127, degrees);
  Drivetrain.driveFor(forward, 16, inches);
  Lift.setVelocity(10, percent);
  Lift.spin(reverse);
  wait(0.6, sec);
  Lift.setVelocity(25, percent);
  Lift.stop(hold);
  claw.spin(reverse);  // stack first pin
  wait(500, msec);
  claw.stop(hold);
  Drivetrain.driveFor(reverse, 5, inches);
  LD.setVelocity(15, percent);
  RD.setVelocity(15, percent);
  Lift.spin(reverse);
  Drivetrain.turnFor(-45, degrees);
  claw.spin(forward);
  wait(180, msec);
  claw.stop(hold);
  Lift.stop(hold);
  Drivetrain.driveFor(forward, 15, inches);
  Drivetrain.turnFor(18, degrees);
  Drivetrain.driveFor(forward, 12, inches);
  LD.setVelocity(25, percent);
  RD.setVelocity(25, percent);
  claw.spin(forward);  // grab second pin
  Drivetrain.driveFor(reverse, 25, inches);
  Drivetrain.turnFor(39, degrees);
  Lift.spin(forward);
  wait(1, sec);
  Lift.stop(hold);
  Drivetrain.driveFor(forward, 5, inches);
  Lift.spin(reverse);
  claw.spin(reverse);
  wait(500, msec);
  Lift.stop(hold);
  claw.stop(hold);  // stack second pin

}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*        m                                                                  */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
  FL10.setVelocity((((Controller1.Axis3.position( )* SpeedChange) + (Controller1.Axis1.position() * TurnChange)) ), percent);
  BL16.setVelocity((((Controller1.Axis3.position() * SpeedChange) + (Controller1.Axis1.position() * TurnChange)) ), percent);
  FR1.setVelocity((((Controller1.Axis3.position() * SpeedChange) - (Controller1.Axis1.position() * TurnChange)) ), percent);
  BR19.setVelocity((((Controller1.Axis3.position() * SpeedChange) - (Controller1.Axis1.position() * TurnChange)) ), percent);
  FL10.spin(forward);
  FR1.spin(forward);
  BL16.spin(reverse);
  BR19.spin(forward);
  Controller1.ButtonL2.pressed(LiftD);
  Controller1.ButtonL1.pressed(Liftup);
  Controller1.ButtonR2.pressed(clawC);
  Controller1.ButtonR1.pressed(clawO);
  Controller2.ButtonDown.pressed(VD);
  Controller2.ButtonUp.pressed(VU);
  Controller2.ButtonLeft.pressed(TD);
  Controller2.ButtonRight.pressed(TU);

  wait(5, msec);               // prevent wasted resources.
  }
}





// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);

  }
}