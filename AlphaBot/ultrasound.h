#ifndef Ultrasound_h
#define Ultrasound_h

#include <Arduino.h>
#include <Servo.h>

class Ultrasound
{
  public:
    Ultrasound(int pin, int servo_pin);
    Ultrasound();
    void setup();
    void attach();
    void detach();
    void sweep(int distances[], int from, int to);
    unsigned long ping();
    void rotate(int pos);
  private:
    int _pin;
    int _servo_pin;
};

#endif
