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
#include "ultrasound.h"
#include "variables.h"

Ultrasound frontUltrasound(7, 6);

void setup() {
  Serial.begin(9600);
}

void loop() {

  Serial.println("Sweep Start");
  int distances[SERVO_RANGE];
  frontUltrasound.sweep(distances, 0, SERVO_RANGE);
  Serial.println("Sweep Done");

}