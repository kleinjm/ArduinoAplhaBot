#include <Arduino.h>
#include "ultrasound.h"

Ultrasound::Ultrasound(int pin, int servo_pin) {
  _pin = pin;
  _servo.attach(servo_pin);
}

// modifies the given array to the distances in front of the sensor.
void Ultrasound::sweep(int distances[], int from, int to) {
  for(int i = from; i < to; i++){
    _servo.write(i);
    distances[i] = ping();
    delay(200);
  }
}

// #ping -> Unsigned Long
// Returns the number of centimeters in front of the sensor.
unsigned long Ultrasound::ping() {
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