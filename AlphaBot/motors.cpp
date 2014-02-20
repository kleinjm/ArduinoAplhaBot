#include <Arduino.h>
#include "motors.h"

#define BRAKE 0       // Motor stopped
#define FULL 255    // Max motor speed
#define HALF 127    // Half motor speed
#define QUARTER 63 // Quarter motor speed

#define FORWARD true
#define BACKWARD false

// Constructor for Motors
Motors::Motors() { };
Motors::Motors(int lc, int rc, int lp, int rp, int pan_pin, int tilt_pin){
  _left_control = lc;
  _right_control = rc;
  _left_power = lp;
  _right_power = rp;
  _pan_pin = pan_pin;
  _tilt_pin = tilt_pin;
}

// Function to be called when adding component
// to a bot. ( in setup() )
void Motors::setup() {
  pinMode(_left_control, OUTPUT);
  pinMode(_right_control, OUTPUT);
}

//PUBLIC
//move forward at full speed
void Motors::forwardFull(){
  forward(FULL);
}
//move forward at half speed
void Motors::forwardHalf(){
  forward(HALF);
}
//move forward at quarter speed
void Motors::forwardQuarter(){
  forward(QUARTER);
}
//stop all movement
void Motors::brake(){
  rightBrake();
  leftBrake();
}

//PRIVATE
//move the left motor forward
void Motors::forward(int spd){
  leftForward(spd);
  rightForward(spd);
}
//move the left motor forward
void Motors::leftForward(int spd){
  moveMotor(_left_control, _left_power, FORWARD, spd);
}
//move the right motor forward
void Motors::rightForward(int spd){
  moveMotor(_right_control, _right_power, FORWARD, spd);
}
//brake left
void Motors::leftBrake(){
  analogWrite(_left_power, BRAKE);
}
//brake left
void Motors::rightBrake(){
  analogWrite(_right_power, BRAKE);
}
//move the motor on the specified pin in the specified direction at the specified speed
void Motors::moveMotor(int control_pin, int power_pin, bool dir, int spd){
  stimulateServos();
  if(dir){  // direction must be boolean because LOW and HIGH are defines that cannot be passed
    digitalWrite(control_pin, LOW);
  }else{
     digitalWrite(control_pin, HIGH);
  }
  analogWrite(power_pin, spd);   //Spins the motor on Channel A at full speed
}

//gives a 5v signal to the solar pan and tilt servos so that they dont "freak out". It seems that they need to have a signal while the motors are working
void Motors::stimulateServos(){
  digitalWrite(_pan_pin, HIGH);
  digitalWrite(_tilt_pin, HIGH);
}
