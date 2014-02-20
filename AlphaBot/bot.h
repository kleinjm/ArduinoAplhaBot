#ifndef Bot_h
#define Bot_h

#include <Arduino.h>
#include "variables.h"
#include "ultrasound.h"
#include "solar_panel.h"
#include "light.h"
#include "ir_sensor.h"
#include "motors.h"

class Bot
{
  public:
    Bot();

    /* COMPONENTS */
    Ultrasound ultrasound;
    bool hasUltrasound;
    void attach_ultrasound(int pin, int servo_pin);
    
    SolarPanel solar_panel;
    bool hasSolarPanel;
    void attach_solar_panel(boolean debug, int pan, int tilt, int top, int right, int bottom, int left);
    void attach_solar_panel_no_servos(int top, int right, int bottom, int left);
    
    Light light;
    bool hasLight;
    void attach_light(int pin);
    
    Motors motors;
    bool hasMotors;
    void attach_motors(int lc, int rc, int lp, int rp, int pan_pin, int tilt_pin);

    void stop();
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void slowForward();
    void slowBackward();
    void slowLeft();
    void slowRight();
    void adjustLeft(int closeSides, int slightLeft);
    void adjustRight(int closeSides, int slightRight);

    void tooClose();
    void slowNav();
    void selfNavigate();
    
    void analogPrint(int pin);
    void autoHeadlight();  //turn on the light if it is dark out and not already on
    
    boolean cycleCheck(unsigned long *lastMillis, unsigned int cycle); //multithreading cycle timer
    bool resting; //tells us if the panel is in resting position. This is set once when we try to rest so that the servos dont need to turn on
  private:
    
};

#endif
