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
#include "ultrasound.h"
#include "solar_panel.h"
#include "light.h"

// The ALPHABOT :
// - left_control on 12
// - right_control on 13
// - left_power on 3
// - right_power on 11
Bot alphabot(12, 13, 3, 11);

// Front Ultrasound :
// - ultrasound is on pin 7
// - servo is on pin 6.
Ultrasound frontUltrasound(7, 6);

// Solar Panel
// - pan servo is on pin 4
// - tilt servo is on pin 5
// - right photoresistor on pin A5
// - top photoresistor on pin A4
// - bottom photoresistor on pin A3
// - left photoresistor on pin A2
SolarPanel panel(4, 5, A4, A5, A3, A2);

// Light
// - light array is on pin 10
Light light(10);

void setup() {
  Serial.begin(9600);
}

void loop() {

  int distances[SERVO_RANGE];
  frontUltrasound.sweep(distances, 0, SERVO_RANGE);

}
