#include "Arduino.h"
#include "Blink.h"
#include "variables.h"

Blinker::Blinker(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Blinker::dot()
{
  digitalWrite(_pin, HIGH);
  delay(250);
  digitalWrite(_pin, LOW);
  delay(250);
}

void Blinker::dash()
{
  digitalWrite(_pin, HIGH);
  delay(1000);
  digitalWrite(_pin, LOW);
  delay(250);
}
