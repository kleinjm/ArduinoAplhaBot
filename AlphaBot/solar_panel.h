#ifndef SolarPanel_h
#define SolarPanel_h

#include <Arduino.h>
#include <Servo.h>

//ANY movement of a servo should return its new position

class SolarPanel
{
  public:
    SolarPanel(boolean debug, int pan, int tilt, int top, int right, int bottom, int left);
    SolarPanel(int top, int right, int bottom, int left);
    SolarPanel();
    void setup();
    bool reset();    //returns true for success
    void readoutSensors();
    int pan(int pos);
    int panSafe(int pos); //panning that makes sure that the tilt is past the clearance value. Good for user control
    int tilt(int pos);
    void trackLight();
    void userControl(int input); // move the panel based on input. WASD
    int tiltUp();  //these four methods move the panel one degree
    int tiltUpSafe();
    int tiltDown();
    int panRight();
    int panLeft();
    int panRightSafe(); //tilt to the clearance height first
    int panLeftSafe();
    int tiltSmooth(int pos); //move to the pos smoothly, not a snap
    int tiltSmooth(int pos, int spd);
    int panSmooth(int pos); //move to the pos smoothly, not a snap
    int panSmooth(int pos, int spd);
  private:
    boolean debug;
    int _top_photoresistor;
    int _right_photoresistor;
    int _bottom_photoresistor;
    int _left_photoresistor;
    int _pan_servo_pin;
    int _tilt_servo_pin;
    int connectPan(boolean con);  //attach the servo and return its position or detach the servo and return -1
    int connectTilt(boolean con);
    int connectServo(boolean con, Servo serv, int pin); // the base method of attaching a servo
};

#endif
