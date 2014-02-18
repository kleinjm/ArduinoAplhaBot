/*
Robot 1 is based off the old RAD platform with 2 motors and built in gear boxes
on each side. The movement is controled by tank treads. Other than this stock
platform which is about a square foot, nothing is yet added for sensors,
electronics, or mechanical funtioning.

Project Start date: 12/8/11
James Klein

Refactoring in progress by Nathan Lilienthal
*/

#include <Servo.h>
#include "variables.h"
#include "bot.h"

#define DEBUG true  //will cause diag infor to print in serial monitor for classes and methods that take this variable

// The ALPHABOT :
// - left_control on 12
// - right_control on 13
// - left_power on 3
// - right_power on 11
Bot abot(12, 13, 3, 11);

void setup() {
  Serial.begin(9600);

  // attach sensors to bot.
  abot.attach_ultrasound(5, 4);
  abot.attach_solar_panel(DEBUG, 6, 7, A4, A5, A3, A2); //must use A2 - A5 to get the right sensitivity
  //abot.attach_light(10);
}

void loop() {
  
  if(Serial.available()){  // FIX: I think this is the fix (available) needed for the remote read in the serial monitor
    int input = Serial.read();
    abot.solar_panel.userControl(input);
  }
  
  if(abot.hasSolarPanel){
    abot.solar_panel.trackLight();
  } else {
    Serial.println("No Solar Panel Attached");    
  }
}
