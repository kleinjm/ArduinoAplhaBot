#include <Arduino.h>
#include "ultrasound.h"

Servo _servo;

// Constructor for an Ultrasound.
// pin       : sensor's pin
// servo_pin : servo's control pin
Ultrasound::Ultrasound() { };
Ultrasound::Ultrasound(int pin, int servo_pin) {
  _pin = pin;
  _servo_pin = servo_pin;
}

// Function to be called when adding component
// to a bot. ( in setup() )
void Ultrasound::setup() {
  if (!_pin) return;  // ensure something exists
  
  pinMode(_pin, OUTPUT);     // set the pin mode for the ultrasound
  _servo.attach(_servo_pin); // attach the servo (wastes battery?)
}

void Ultrasound::attach() {
  if (!_pin) return;  // ensure something exists
  
  _servo.attach(_servo_pin);
}

void Ultrasound::detach() {
  if (!_pin) return;  // ensure something exists
  
  _servo.detach();
}

// modifies the given array to the distances in front of the sensor.
void Ultrasound::sweep(int distances[], int from, int to) {
  if (!_pin) return;  // ensure something exists
  
  for(int i = from; i < to; i++){
    _servo.write(i);
    distances[i] = ping();
    delay(200);
  }
}

// #ping -> Unsigned Long
// Returns the number of centimeters in front of the sensor.
unsigned long Ultrasound::ping() {
  if (!_pin) return NULL;  // ensure something exists
  
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(_pin, LOW);
  pinMode(_pin, INPUT);
  digitalWrite(_pin, HIGH);
  return pulseIn(_pin, HIGH) / 5.8138;
}

void Ultrasound::rotate(int position) {
  if (!_pin) return;  // ensure something exists
  
  _servo.write(position);
}
