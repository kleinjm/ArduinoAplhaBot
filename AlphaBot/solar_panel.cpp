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
SolarPanel::SolarPanel(int pan, int tilt, int top, int right, int bottom, int left) {
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
  if (!_pan_servo_pin) return;  // ensure something exists
  
  _pan_servo.attach(_pan_servo_pin);
  _tilt_servo.attach(_tilt_servo_pin);
}

//place panel on top and center of the bot
void SolarPanel::reset() {
  if (!_pan_servo_pin) return;  // ensure something exists
  
  int tilt_to = 120;
  int servo_delay = 50;
  int pan_angle = _pan_servo.read();
  int tilt_angle = _tilt_servo.read();

  if (tilt_angle < tilt_to && pan_angle != 90) {
    // _pan_servo.attach(panServoPin);
    // _tilt_servo.attach(tiltServoPin);

    while (pan_angle != 90) {
      if (pan_angle > 90) {
        _pan_servo.write(pan_angle--);
      } else {
        _pan_servo.write(pan_angle++);
      }
      delay(servo_delay);
    }

    while (tilt_angle < tilt_to) {
      _tilt_servo.write(tilt_angle++);
      delay(servo_delay);
    }
  }
  // _pan_servo.detach();
  // _tilt_servo.detach();
}
