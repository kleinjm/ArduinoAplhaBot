#include <Arduino.h>
#include "light.h"

// Constructor for a Light.
// pin : pin the light is connencted to.
Light::Light() { };
Light::Light(int pin) {
  _pin = pin;
}

// Function to be called when adding component
// to a bot. ( in setup() )
void Light::setup() {
  if (!_pin) return;  // ensure pin exists
  
  pinMode(_pin, OUTPUT);
  active = false;
}

// Turn on the light.
void Light::on() {
  if (!_pin) return;  // ensure pin exists

  digitalWrite(_pin, HIGH);
  active = true;
}

// Turn off the light.
void Light::off() {
  if (!_pin) return;  // ensure pin exists
  
  digitalWrite(_pin, LOW);
  active = false;
}

// Strobe the light at the given frequency.
void Light::strobe(int freq) {
  if (!_pin) return;  // ensure pin exists
  
  on();
  delay(freq);
  off();
  delay(freq);
}
