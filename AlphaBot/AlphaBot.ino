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

// The ALPHABOT :
// - left_control on 12
// - right_control on 13
// - left_power on 3
// - right_power on 11
Bot alphabot(12, 13, 3, 11);

void setup() {
  Serial.begin(9600);

  // attach sensors to bot.
  alphabot.attach_ultrasound(7, 9);
  alphabot.attach_solar_panel(4, 5, A4, A5, A3, A2);
  alphabot.attach_light(10);
}

void loop() {

  if (alphabot.hasLight) {
    alphabot.light.strobe(100);
  } else {
    Serial.println("No Light Attached");
  }
  
  if (alphabot.hasUltrasound) {
    alphabot.ultrasound.rotate(45);
  } else {
    Serial.println("No Ultrasound Attached");    
  }
  
}
