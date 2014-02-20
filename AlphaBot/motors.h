#ifndef motors_h
#define motors_h

#include <Arduino.h>

class Motors
{
  public:
    Motors(int lc, int rc, int lp, int rp, int pan_pin, int tilt_pin);
    Motors();
    void setup();
    
    void forwardFull();
    void forwardHalf();  //FIX: Looks like only full power works. May be because of being on the same circut. May be cuz the numbers for the analogWrite
    void forwardQuarter();
    void brake();
  private:
    // Motor control pins.
    int _left_control;
    int _right_control;
    // Motor power pins.
    int _left_power;
    int _right_power;
    //need to know the pan and tilt pins to stimulate them while moving
    int _pan_pin;
    int _tilt_pin;
    
    void forward(int spd);
    void leftForward(int spd);
    void rightForward(int spd);
    void rightBrake();
    void leftBrake();
    void moveMotor(int control_pin, int power_pin, bool dir, int spd);  //the atomic method for moving something
    
    void stimulateServos();  //this gives a 5v signal to the solar pan and tilt servos so that they dont "freak out". It seems that they need to have a signal while the motors are working
};

#endif
