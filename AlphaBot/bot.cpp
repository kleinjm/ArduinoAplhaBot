#include <Arduino.h>
#include "bot.h"
#include "ultrasound.h"
#include "variables.h"

#define MOTOR_OFF 0       // Motor stopped
#define MOTOR_FULL 511    // Max motor speed
#define MOTOR_HALF 127    // Half motor speed
#define MOTOR_QUARTER 125 // Quarter motor speed

#define FORWARD HIGH
#define BACKWARD LOW

// Constructor for a Bot.
// without a ultrasound, can only do basic movements.
Bot::Bot(int lc, int rc, int lp, int rp) {
  _left_control  = lc;
  _right_control = rc;
  _left_power    = lp;
  _right_power   = rp;
  _ultrasound = NULL;

  // Set control pins for wheel motors.
  pinMode(_left_control, OUTPUT);
  pinMode(_right_control, OUTPUT);
}

// Constructor for a Bot.
// with a pointer to a Ultrasound. Can use distance collection for
// complex Bots.
Bot::Bot(int lc, int rc, int lp, int rp, Ultrasound* u) {
  _left_control  = lc;
  _right_control = rc;
  _left_power    = lp;
  _right_power   = rp;
  _ultrasound = u;

  // Set control pins for wheel motors.
  pinMode(_left_control, OUTPUT);
  pinMode(_right_control, OUTPUT);
}

//Movements
void Bot::stop() {
  analogWrite(_left_power, MOTOR_OFF);
  analogWrite(_right_power, MOTOR_OFF);
}

void Bot::moveForward() {
  digitalWrite(_left_control, FORWARD);
  digitalWrite(_right_control, FORWARD);
  analogWrite(_left_power, MOTOR_FULL);
  analogWrite(_right_power, MOTOR_FULL);
}
void Bot::moveBackward() {
  digitalWrite(_left_control, BACKWARD);
  digitalWrite(_right_control, BACKWARD);
  analogWrite(_left_power, MOTOR_FULL);
  analogWrite(_right_power, MOTOR_FULL);
}

void Bot::moveLeft() {
  digitalWrite(_left_control, BACKWARD);
  digitalWrite(_right_control, FORWARD);
  analogWrite(_left_power, MOTOR_HALF);
  analogWrite(_right_power, MOTOR_HALF);
}

void Bot::moveRight() {
  digitalWrite(_left_control, FORWARD);
  digitalWrite(_right_control, BACKWARD);
  analogWrite(_left_power, MOTOR_HALF);
  analogWrite(_right_power, MOTOR_HALF);
}

// Slower movements for crawling
void Bot::slowForward() {
  digitalWrite(_left_control, FORWARD);
  digitalWrite(_right_control, FORWARD);
  analogWrite(_left_power, MOTOR_QUARTER);
  analogWrite(_right_power, MOTOR_QUARTER);
}

void Bot::slowBackward() {
  digitalWrite(_left_control, BACKWARD);
  digitalWrite(_right_control, BACKWARD);
  analogWrite(_left_power, MOTOR_QUARTER);
  analogWrite(_right_power, MOTOR_QUARTER);
}

void Bot::slowLeft() {
  digitalWrite(_left_control, BACKWARD);
  digitalWrite(_right_control, FORWARD);
  analogWrite(_left_power, 100);
  analogWrite(_right_power, 100);
}

void Bot::slowRight() {
  digitalWrite(_left_control, FORWARD);
  digitalWrite(_right_control, BACKWARD);
  analogWrite(_left_power, 100);
  analogWrite(_right_power, 100);
}

//if the bot is too close on it's slight left then adjust the motor to straighten out
void Bot::adjustLeft(int closeSides, int slightLeft){
  if(slightLeft < closeSides){
    analogWrite(_right_power, MOTOR_QUARTER - 80);
    analogWrite(_left_power, MOTOR_QUARTER);
    delay(700);
  }
}

//if the bot is too close on it's slight right then adjust the motor to straighten out
void Bot::adjustRight(int closeSides, int slightRight){
  if(slightRight < closeSides){
    analogWrite(_left_power, MOTOR_QUARTER - 80);
    analogWrite(_right_power, MOTOR_QUARTER);
    delay(700);
  }
}

// If the bot is too close then backup and turn the way with the greatest distance
void Bot::tooClose() {
  if (_ultrasound == NULL) return; // Error handling?
  Ultrasound sensor = *_ultrasound;
  int left;
  int right;

  sensor.lookAt(SERVO_RANGE);   // Look left
  left = sensor.ping();
  sensor.lookAt(0);             // Look right
  right = sensor.ping();
  sensor.lookAt(SERVO_RANGE/2); // Look center

  // Turning in direction of greatest distance.
  if (left > right) {
    slowLeft();
    delay(1100);
    stop();
  } else {
    slowRight();
    delay(1100);
    stop();
  }
}


//self navigate at a moderately slow speed
void Bot::slowNav(){
  if (_ultrasound == NULL) return; // Error handling?
  Ultrasound sensor = *_ultrasound;

  int slightRight;
  int slightLeft;
  int forward;
  int closeDistance = 30; //distance directly in front of the bot that causes it to stop
  int closeSides = 50; //distance that the bot is too close to something slightly right or left
  int distServoPosition = 85; // start the ultrasound looking forward.
  int distServoCenter = 85; //forward looking sensor
  int distServoSlightLeft = 115;  //+30 from center
  int distServoSlightRight = 55;   //-30 from center
  int scanSpeed = 20; //delay in scan

  while(distServoPosition < distServoSlightLeft){ //scan left
    slowForward();
    distServoPosition++;
    sensor.lookAt(distServoPosition);
    if(distServoPosition == distServoCenter){
      forward = sensor.ping();
      if(forward < closeDistance){
        slowBackward();
        delay(600);
        stop();
        delay(50);
        tooClose();  //run procedure if bot is too close
      }else{
        slowForward();
      }
    }
    if(distServoPosition == distServoSlightLeft){
      slightLeft = sensor.ping();
      adjustLeft(closeSides, slightLeft);
    }
    delay(scanSpeed);
  }
  while(distServoPosition > distServoSlightRight){ //scan right
    slowForward();
    distServoPosition--;
    sensor.lookAt(distServoPosition);
    if(distServoPosition == distServoCenter){
      forward = sensor.ping();
      if(forward < closeDistance){
        slowBackward();
        delay(600);
        stop();
        delay(50);
        tooClose();  //run procedure if bot is too close
      }else{
        slowForward();
      }
    }
    if(distServoPosition == distServoSlightRight){
      slightRight = sensor.ping();
      adjustRight(closeSides, slightRight);
    }
    delay(scanSpeed);
  }
}

//Self navigates at full speed without solar charging
void Bot::selfNavigate() {
  if (_ultrasound == NULL) return; // Error handling?
  Ultrasound sensor = *_ultrasound;

  int right = 0;  //right distance
  int left = 0; //left distance
  sensor.lookAt(83); //set sensor forward

  // mobilePanel(); //set panel in stationary position

  int x = sensor.ping();  //x represtents the distance returned by the distance sensor code
  delay(125);             //delay 1/8 seconds
  if (x < 40) {
    stop();
    delay(1000);
    sensor.lookAt(0); //turn servo right fully
    delay(500); //allow time to move servo
    right = sensor.ping(); //get distance again
    delay(125); //delay 1/8 seconds

    sensor.lookAt(166); //turn servo left fully
    delay(500); //allow time to move servo
    left = sensor.ping(); //get distance again
    delay(500); //delay 1/8 seconds
    sensor.lookAt(83); //set sensor forward
    delay(250);

    if (right >= left) {
      moveRight();
      delay(500);
      stop();
      delay(125);
    } else {
      moveLeft();
      delay(500);
      stop();
      delay(125);
    }
  } else {
    moveForward();
  }
}
