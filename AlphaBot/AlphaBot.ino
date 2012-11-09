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
#include "ultrasound.h"
#include "bot.h"

// The front ultrasound is on pin 7, with a servo on 6.
Ultrasound frontUltrasound(7, 6);

// The ALPHABOT :
// left_control on 12
// right_control on 13
// left_power on 3
// right_power on 11
Bot alphabot(12, 13, 3, 11);

void setup() {
  Serial.begin(9600);
}

void loop() {

  alphabot.stop();

}