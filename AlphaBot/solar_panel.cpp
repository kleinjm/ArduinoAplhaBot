//Note: For photosensors, must use A2 - A5 to get the right sensitivity
#include <Arduino.h>
#include "solar_panel.h"

Servo _pan_servo;
Servo _tilt_servo;

#define TILT_MIN 92  // where it hits the stopper (one click past that just in case so it falls ing the range)
#define TILT_MAX 180
#define TILT_RESTING 100 //the tilt position for resting and moving
#define TILT_CLEARANCE 110 //the min height that the tilt servo should be before the pan servo pans
#define PAN_MIN 0    //we may later have something blocking the pan from going all the way
#define PAN_MAX 180
#define PAN_RESTING 90
#define SMOOTH_SPD 25 //the speed that smoth methods delay to not get snap movements

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
    rest();
  }
}

//attach or detach the given servo to the given pin and set position variable. This makes for short and fast attach/detach actions, ie. saving power
//can be used to get postion
int SolarPanel::connectServo(boolean con, Servo serv, int pin){
  if(con){
    if(!serv.attached()){
      serv.attach(pin);
    }
    return serv.read();
  }else{
    if(serv.attached()){
      serv.detach();
    }
    return -1;
  }
}
//attach/detach the tilt servo to the pin specified in the constructor and set position variable. This makes for short and fast attach/detach actions, ie. saving power
int SolarPanel::connectTilt(boolean con){
  return connectServo(con, _tilt_servo, _tilt_servo_pin);
}
//attach/detach the pan servo to the pin specified in the constructor and set position variable. This makes for short and fast attach/detach actions, ie. saving power
int SolarPanel::connectPan(boolean con){
  return connectServo(con, _pan_servo, _pan_servo_pin);
}

//set the pan servo to the given position. This is not a smooth movement, it's a snap to position
int SolarPanel::pan(int pos){
  connectPan(true);
  if(pos <= PAN_MAX && pos >= PAN_MIN ){
    _pan_servo.write(pos);
  }
  return _pan_servo.read();
}
//set the tilt servo to the given position. This is not a smooth movement, it's a snap to position
int SolarPanel::tilt(int pos){
  connectTilt(true);
  if(pos <= TILT_MAX && pos >= TILT_MIN ){  //MUST BE <= and >= because you may want to go to the max or min pos
    _tilt_servo.write(pos);
  }
  return _tilt_servo.read();
}

//panning that makes sure that the tilt is past the clearance value. Good for user control
int SolarPanel::panSafe(int pos){
  if(connectTilt(true) < TILT_CLEARANCE){
    tiltSmooth(TILT_CLEARANCE);
  }
  return pan(pos);
}
//pan the panel one degree right
int SolarPanel::panRight(){
  return pan(connectPan(true) + 1);
}
//pan the panel one degree right
int SolarPanel::panLeft(){
  return pan(connectPan(true) - 1);
}
//pan the panel one degree right
int SolarPanel::panRightSafe(){
  return panSafe(connectPan(true) + 1);
}
//pan the panel one degree right
int SolarPanel::panLeftSafe(){
  return panSafe(connectPan(true) - 1);
}

//pan to the given position smoothly
int SolarPanel::panSmooth(int pos){
  return panSmooth(pos, SMOOTH_SPD);
}
//pan to the given position smoothly at the given speed
int SolarPanel::panSmooth(int pos, int spd){
  int cur_pos = connectPan(true);
  if(cur_pos != pos){
    while(cur_pos < pos){
      cur_pos = panRight();   //these may be backwards
      delay(spd);
    }
    while(cur_pos > pos){
      cur_pos = panLeft();
      delay(spd);
    }
  }
  return _pan_servo.read();
}

//tilt the panel up one degree and keep track of the position
int SolarPanel::tiltUp(){
  return tilt(connectTilt(true) - 1);
}
//tilt the panel down one degree and keep track of the position
int SolarPanel::tiltUpSafe(){
  int cur_pos = connectTilt(true);
  if(cur_pos < TILT_CLEARANCE){
    return cur_pos;
  }else{
    return tilt(connectTilt(true) - 1);
  }
}
//tilt the panel down one degree and keep track of the position
int SolarPanel::tiltDown(){
  return tilt(connectTilt(true) + 1);
}

//tilt to the given position smoothly
int SolarPanel::tiltSmooth(int pos){    //IF this is too fast then call the other one with a set speed. The other one has a speed
  return tiltSmooth(pos, SMOOTH_SPD);
}
//tilt to the given position smoothly at the given speed
int SolarPanel::tiltSmooth(int pos, int spd){
  int cur_pos = connectTilt(true);
  if(cur_pos != pos){
    while(cur_pos < pos){
      cur_pos = tiltDown();
      delay(spd);
    }
    while(cur_pos > pos){
      cur_pos = tiltUp();
      delay(spd);
    }
  }
  return _tilt_servo.read();
}
//move the pan and tilt servos to a position at the same time to make smoth movements
void SolarPanel::positionSmooth(int panPos, int tiltPos){
  int cur_pan_pos = connectPan(true);
  int cur_tilt_pos = connectTilt(true);
  while(cur_pan_pos != panPos && cur_tilt_pos != tiltPos){
    if(cur_pan_pos < panPos){
      cur_pan_pos = panRight();
      delay(SMOOTH_SPD);
    }
    if(cur_pan_pos > panPos){
      cur_pan_pos = panLeft();
      delay(SMOOTH_SPD);
    }
    if(cur_tilt_pos < tiltPos){
      cur_tilt_pos = tiltDown();
      delay(SMOOTH_SPD);
    }
    if(cur_tilt_pos > tiltPos){
      cur_tilt_pos = tiltUp();
      delay(SMOOTH_SPD);
    }
  }
}

//place panel on top and center of the bot
bool SolarPanel::rest() {
  int servo_delay = 50;
  int _tilt_pos = connectTilt(true);
  int _pan_pos = connectPan(true);
  
  if (_tilt_pos != TILT_RESTING || _pan_pos != PAN_RESTING) { //if it's not in position
    if(_tilt_pos < TILT_CLEARANCE){ 
      _tilt_pos = tiltSmooth(TILT_CLEARANCE);
    }
    if(_pan_pos != PAN_RESTING){ 
      _pan_pos = panSmooth(PAN_RESTING);
    }
    if(_tilt_pos > TILT_RESTING){ 
      _tilt_pos = tiltSmooth(TILT_RESTING);
    }
  }
  if(debug){ Serial.println("Solar panel in resting position"); }
  
  connectTilt(false);
  connectPan(false);
  return true; //success
}

//attach servos and check if the panel is in resting position
bool SolarPanel::isResting(bool attachServ){
  bool res = (connectTilt(true) == TILT_RESTING && connectPan(true) == PAN_RESTING);
  if(attachServ){ 
    //Serial.println(res);
    return res;
  }else{ connectTilt(false); connectPan(false); return res; }
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
//returns true if the light from all photoresistors is under a certain threshold
bool SolarPanel::isDark(){
  int th = 250; //the threshold that light must fall below to be declared dark
  if(debug && false){ readoutSensors(); }
  return (analogRead(_top_photoresistor) < th && analogRead(_bottom_photoresistor) < th && analogRead(_right_photoresistor) < th && analogRead(_left_photoresistor) < th);
}

//adjust the solar panel position based on the light's location. Turn bot if it is past 180 degree access
// FIX: Detaching the servo isn't perfect. Still sounds like it is load bearing.
void SolarPanel::trackLight(){
  int TBRANGE = 50; //difference in values that will determine the center light between the top and bottom resistors
  int RLRANGE = 50; //difference in values that will determine the center light between the right and left resistors
  int SPEED = 50; //rate at which servos move to follow light (in milliseconds)
  
  //FEATURE: Moving the bot once panel is too far to one side
  
  if(debug && false){
    readoutSensors();
  }
  if(analogRead(_right_photoresistor) < analogRead(_left_photoresistor) - RLRANGE){
    if(debug){ Serial.println("Turn panel right"); }
    panLeft();
  }else if(analogRead(_left_photoresistor) < analogRead(_right_photoresistor) - RLRANGE){
    if(debug){ Serial.println("Turn panel left"); }
    panRight();
  }else{
    connectPan(false);
  }
  delay(SPEED);
  if(analogRead(_top_photoresistor) > analogRead(_bottom_photoresistor) + TBRANGE){
    if(debug){ Serial.println("Turn panel up"); }
    tiltUp();
  }else if(analogRead(_bottom_photoresistor) > analogRead(_top_photoresistor) + TBRANGE){
    if(debug){ Serial.println("Turn panel down"); }
    tiltDown();
  }else{
    //"FIX: cannot disconnect if there is a load on the sevros. This causes the solar panel to fall and then go back up and seizer.
    connectTilt(true);
  }
  delay(SPEED);
}

//user control the panel with the given input. WASD
//Can take multiple strings in Serial monitor (ie. aaaaaa moves left 6 degrees)
void SolarPanel::userControl(int input){
  Serial.print(input);
  switch (input){
    case 119:  //down
      tiltUp();
      if(debug){ Serial.println("User moved panel UP"); }
      break;
    case 115:  //up
      tiltDown();
      if(debug){ Serial.println("User moved panel DOWN"); }
      break;
    case 100:  //left
      panLeft();
      if(debug){ Serial.println("User moved panel LEFT"); }
      break;
    case 97:  //right
      panRight();
      if(debug){ Serial.println("User moved panel RIGHT"); }
      break;
  }
}
