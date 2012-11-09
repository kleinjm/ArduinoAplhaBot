#ifndef Morse_h
#define Morse_h

#include "Arduino.h"

class Blinker
{
  public:
    Blinker(int pin);
    void dot();
    void dash();
  private:
    int _pin;
};

#endif
