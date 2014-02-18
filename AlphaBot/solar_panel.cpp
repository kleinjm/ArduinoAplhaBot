//Note: For photosensors, must use A2 - A5 to get the right sensitivity
#include <Arduino.h>
#include "solar_panel.h"

Servo _pan_servo;
Servo _tilt_servo;

// Constructor for an SolarPanel.
// pan    : pan servo's pin
// tilt   : tilt servo's pin
// top    : top photoresistor's pin
// right  : right photoresistor's pin
// bottom : bottom photoresistor's pin
// left   : left photoresistor's pin
SolarPanel::SolarPanel() { };

//Constructor used for photocensors only and not the servos
SolarPanel::SolarPanel(int top, int right, int bottom, int left) {
  _top_photoresistor    = top;
  _right_photoresistor  = right;
  _bottom_photoresistor = bottom;
  _left_photoresistor   = left;
}
SolarPanel::SolarPanel(boolean db, int pan, int tilt, int top, int right, int bottom, int left) {
  debug = db;
  _pan_servo_pin = pan;
  _tilt_servo_pin = tilt;
  _top_photoresistor    = top;
  _right_photoresistor  = right;
  _bottom_photoresistor = bottom;
  _left_photoresistor   = left;
}

// Function to be called when adding component
// to a bot. ( in setup() )
void SolarPanel::setup() {
  if(_pan_servo_pin != NULL && _tilt_servo_pin != NULL){
    if (!_pan_servo_pin) return;  // ensure something exists
    
    connectPan(true);
    connectTilt(true);
  }
}

//attach or detach the given servo to the given pin and set position variable. This makes for short and fast attach/detach actions, ie. saving power
//servName must be "pan" or "tilt"
void SolarPanel::connectServo(boolean con, Servo serv, int pin, String servName){
  if(con){
    if(debug){ Serial.println("Attempting to attach " + servName + " servo..."); }
    if(!serv.attached()){
      serv.attach(pin);
      if(servName == "pan"){ _pan_pos = serv.read(); }
      else if (servName == "tilt"){ _tilt_pos = serv.read(); }
      if(debug){ Serial.println(servName + " servo attached successfully"); }
    }else{
      if(debug){ Serial.println(servName + " servo already attached"); }
    }
  }else{
    if(debug){ Serial.println("Attempting to detach " + servName + " servo..."); }
    if(_pan_servo.attached()){
      _pan_servo.detach();
      if(debug){ Serial.println(servName + " servo detached successfully"); }
    }else{
      if(debug){ Serial.println(servName + " servo not attached"); }
    }
  }
}
//attach/detach the tilt servo to the pin specified in the constructor and set position variable. This makes for short and fast attach/detach actions, ie. saving power
void SolarPanel::connectTilt(boolean con){
  connectServo(con, _tilt_servo, _tilt_servo_pin, "tilt");
}
//attach/detach the pan servo to the pin specified in the constructor and set position variable. This makes for short and fast attach/detach actions, ie. saving power
void SolarPanel::connectPan(boolean con){
  connectServo(con, _pan_servo, _pan_servo_pin, "pan");
}

//set the pan servo to the given position. This is not a smooth movement, it's a snap to position
void SolarPanel::pan(int pos){
  if(_pan_servo_pin != NULL){
    _pan_servo.write(pos);
    _pan_pos = pos;
  }else{
    Serial.println("No pan servo attached");
  }
}

//set the tilt servo to the given position. This is not a smooth movement, it's a snap to position
void SolarPanel::tilt(int pos){
  if(_tilt_servo_pin != NULL){
    _tilt_servo.write(pos);
    _tilt_pos = pos;
  }else{
    Serial.println("No tilt servo attached");
  }
}

//place panel on top and center of the bot
void SolarPanel::reset() {
  if (!_pan_servo_pin) return;  // ensure something exists
  
  int tilt_to = 130;
  int servo_delay = 50;
  int pan_angle = _pan_servo.read();
  int tilt_angle = _tilt_servo.read();

  if (tilt_angle < tilt_to && pan_angle != 90) {
    // _pan_servo.attach(panServoPin);
    // _tilt_servo.attach(tiltServoPin);

    while (pan_angle != 90) {
      if (pan_angle > 90) {
        pan(pan_angle--);
      } else {
        pan(pan_angle++);
      }
      delay(servo_delay);
    }

    while (tilt_angle < tilt_to) {
      tilt(tilt_angle++);
      delay(servo_delay);
    }
  }
  // _pan_servo.detach();
  // _tilt_servo.detach();
}

//Print out the sensors reading on the serial monitor
void SolarPanel::readoutSensors(){
  String intro = "Photosensor values - Top: ";
  String bottom = ", Bottom: ";
  String right = ", Right: ";
  String left = ", Left: ";
  Serial.println(intro + analogRead(_top_photoresistor) + bottom + analogRead(_bottom_photoresistor) + right + analogRead(_right_photoresistor) + left + analogRead(_left_photoresistor));
  delay(100);
}

//adjust the solar panel position based on the light's location. Turn bot if it is past 180 degree access
// FIX: Detaching the servo isn't perfect. Still sounds like it is load bearing.
void SolarPanel::trackLight(){
  int TBRANGE = 50; //difference in values that will determine the center light between the top and bottom resistors
  int RLRANGE = 50; //difference in values that will determine the center light between the right and left resistors
  int SPEED = 50; //rate at which servos move to follow light (in milliseconds)
  
  //FEATURE: Moving the bot once panel is too far to one side
//  if(analogRead(_top_photoresistor) > analogRead(_bottom_photoresistor) + range && tiltServoPosition >= 180){
//    //turn the bot 180
//    turnLeft();
//    delay(750);
//    stopMovement();
//    //set the panel down a bit
//    tiltServoPosition = 150;
//    tiltServo.attach(tiltServoPin);  // attaches the servo on servo pin to the servo object
//    tiltServo.write(tiltServoPosition);
//    delay(1000);
//  }else{
//    if(analogRead(_top_photoresistor) > analogRead(_bottom_photoresistor) + range && tiltServoPosition < 160){
//      tiltServoPosition = tiltServoPosition + servoSpeed;
//      tiltServo.attach(tiltServoPin);  // attaches the servo on servo pin to the servo object
//      tiltServo.write(tiltServoPosition);
//    }else if(analogRead(_bottom_photoresistor_right_photoresistor) > analogRead(_top_photoresistor) + range && tiltServoPosition > 74){
//      tiltServoPosition = tiltServoPosition - servoSpeed;
//      tiltServo.attach(tiltServoPin);  // attaches the servo on servo pin to the servo object
//      tiltServo.write(tiltServoPosition);
//    }else {
//      tiltServo.detach();
//    }
//  }
//  if(panServoPosition <= 10){
//    //turn the bot right to allow it to get more light in that direction
//    turnRight();
//    delay(300);
//    stopMovement();
//    //reset the panel back a bit
//    panServoPosition = 20;
//    panServo.attach(panServoPin); 
//    panServo.write(panServoPosition);
//    delay(1000);
//  }else if(panServoPosition >= 170){
//    //turn the bot left
//    turnLeft();
//    delay(300);
//    stopMovement();
//    //reset the panel position left a bit
//    panServoPosition = 160;
//    panServo.attach(panServoPin);
//    panServo.write(panServoPosition);
//    delay(1000);
//  }else{
  
  if(debug){
    String str1 = "Pan servo reads ";
    String str2 = " and Tilt servo reads ";
    Serial.println(str1 + _pan_pos + str2 + _tilt_pos);
    //readoutSensors();
  }
  if(analogRead(_right_photoresistor) < analogRead(_left_photoresistor) - RLRANGE && _pan_pos > 10){
    if(debug){ Serial.println("Turn panel right"); }
    connectPan(true);
    pan(_pan_pos + 1);
  }else if(analogRead(_left_photoresistor) < analogRead(_right_photoresistor) - RLRANGE && _pan_pos < 170){
    if(debug){ Serial.println("Turn panel left"); }
    connectPan(true);
    pan(_pan_pos - 1);
  }else{
    connectPan(false);
  }
  delay(SPEED);
  if(analogRead(_top_photoresistor) > analogRead(_bottom_photoresistor) + TBRANGE && _tilt_pos > 100){
    if(debug){ Serial.println("Turn panel up"); }
    connectTilt(true);
    tilt(_tilt_pos - 1);
  }else if(analogRead(_bottom_photoresistor) > analogRead(_top_photoresistor) + TBRANGE && _tilt_pos < 180){
    if(debug){ Serial.println("Turn panel down"); }
    connectTilt(true);
    tilt(_tilt_pos + 1);
  }else{
    connectTilt(false);
  }
  delay(SPEED);
}

//user control the panel with the given input. WASD
//Can take multiple strings in Serial monitor (ie. aaaaaa moves left 6 degrees)
void SolarPanel::userControl(int input){
  Serial.print(input);
  switch (input){
    case 119:  //down
      tilt(_tilt_pos - 1);
      if(debug){ Serial.println("User moved panel UP"); }
      break;
    case 115:  //up
      tilt(_tilt_pos + 1);
      if(debug){ Serial.println("User moved panel DOWN"); }
      break;
    case 100:  //left
      pan(_pan_pos + 1);
      if(debug){ Serial.println("User moved panel LEFT"); }
      break;
    case 97:  //right
      pan(_pan_pos - 1);
      if(debug){ Serial.println("User moved panel RIGHT"); }
      break;
  }
}
