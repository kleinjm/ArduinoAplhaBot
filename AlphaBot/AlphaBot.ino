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
#include "IRremote.h"

#define DEBUG true  //will cause diag infor to print in serial monitor for classes and methods that take this variable

// The ALPHABOT :
// - left_control on 12
// - right_control on 13
// - left_power on 3
// - right_power on 11
Bot abot(12, 13, 3, 11);

String mode = "nuetral"; //what the bot is currently doing

int IRpin = 2;  // pin for the IR sensor
IRrecv irrecv(IRpin);
decode_results results;

void setup() {
  Serial.begin(9600);

  // attach sensors to bot.
  abot.attach_ultrasound(5, 4);
  abot.attach_solar_panel(DEBUG, 6, 7, A4, A5, A3, A2); //must use A2 - A5 to get the right sensitivity
  abot.attach_light(8);
  
  irrecv.enableIRIn(); // Start the receiver for IR
}

void loop() {
  
//  if(Serial.available()){  // FIX: I think this is the fix (available) needed for the remote read in the serial monitor
//    int input = Serial.read();
//    abot.solar_panel.userControl(input);
//  }
  
//  if(abot.hasSolarPanel && mode == "charge"){
//    abot.solar_panel.trackLight();
//  } else {
//    Serial.println("No Solar Panel Attached");    
//  }¨
  
  if (irrecv.decode(&results)){
    irrecv.resume();   // Receive the next value
  }

  switch (results.value){
    case six_up:          // IR codes in variables.h
      Serial.println("Six up - light on");
      abot.light.on();
      break;
    case six_down:
      Serial.println("Six down - light off");
      abot.light.off();
      break;
    case five_up:
      Serial.println("Five up - charge mode");
      abot.solar_panel.trackLight();
      break;
    case five_down:
      Serial.println("Five down - reset");
      if(!abot.resting){
        abot.resting = abot.solar_panel.reset();
      }
      break;
    case four_up:
      Serial.println("Four up - tilt up");
      abot.solar_panel.tiltUpSafe();
      break;
    case four_down:
      Serial.println("Four down - tilt down");
      abot.solar_panel.tiltDown();
      break;
    case three_up:
      Serial.println("Three up - pan left");
      abot.solar_panel.panLeftSafe();
      break;
    case three_down:
      Serial.println("Three down - pan right");
      abot.solar_panel.panRightSafe();
      break;
    case two_up:
      Serial.println("Two up");
      break;
    case two_down:
      Serial.println("Two down");
      break;
    case one_up:
      Serial.println("One up");
      break;
    case one_down:
      Serial.println("One down");
      break;
  }
}
