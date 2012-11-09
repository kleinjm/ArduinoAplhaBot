#include <Arduino.h>
#include "light.h"

// Constructor for a Light.
// pin : pin the light is connencted to.
Light::Light(int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

// Turn on the light.
void Light::on() {
  digitalWrite(_pin, HIGH);
}

// Turn off the light.
void Light::off() {
  digitalWrite(_pin, LOW);
}

// Strobe the light at the given frequency.
void Light::strobe(int freq) {
  digitalWrite(_pin, HIGH);
  delay(freq);
  digitalWrite(_pin, LOW);
  delay(freq);
}