#ifndef SolarPanel_h
#define SolarPanel_h

#include <Arduino.h>
#include <Servo.h>

class SolarPanel
{
  public:
    SolarPanel(int pan, int tilt, int top, int right, int bottom, int left);
    SolarPanel();
    void setup();
    void reset();
  private:
    int _top_photoresistor;
    int _right_photoresistor;
    int _bottom_photoresistor;
    int _left_photoresistor;
    int _pan_servo_pin;
    int _tilt_servo_pin;
};

#endif
