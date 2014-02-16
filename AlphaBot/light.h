#ifndef Light_h
#define Light_h

#include <Arduino.h>

class Light
{
  public:
    Light(int pin);
    Light();
    void setup();
    void on();
    void off();
    void strobe(int freq);
  private:
    int _pin;
};

#endif
