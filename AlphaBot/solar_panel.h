#ifndef SolarPanel_h
#define SolarPanel_h

#include <Arduino.h>
#include <Servo.h>

class SolarPanel
{
  public:
    SolarPanel(int pan, int tilt, int top, int right, int bottom, int left);
    void reset();
  private:
    int _top_photoresistor;
    int _right_photoresistor;
    int _bottom_photoresistor;
    int _left_photoresistor;
    Servo _pan_servo;
    Servo _tilt_servo;
};

#endif