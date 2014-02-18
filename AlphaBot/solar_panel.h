#ifndef SolarPanel_h
#define SolarPanel_h

#include <Arduino.h>
#include <Servo.h>

class SolarPanel
{
  public:
    SolarPanel(boolean debug, int pan, int tilt, int top, int right, int bottom, int left);
    SolarPanel(int top, int right, int bottom, int left);
    SolarPanel();
    void setup();
    void reset();
    void readoutSensors();
    void pan(int pos);
    void tilt(int pos);
    void trackLight();
    void userControl(int input); // move the panel based on input. WASD
  private:
    boolean debug;
    int _top_photoresistor;
    int _right_photoresistor;
    int _bottom_photoresistor;
    int _left_photoresistor;
    int _pan_servo_pin;
    int _tilt_servo_pin;
    int _pan_pos;  // the servo positions are not only from reading the servo because they may need to attach or detach
    int _tilt_pos;
    void connectPan(boolean con);
    void connectTilt(boolean con);
    void connectServo(boolean con, Servo serv, int pin, String servName); // the base method of attaching a servo
};

#endif
