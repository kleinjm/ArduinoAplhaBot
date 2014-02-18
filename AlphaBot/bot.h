#ifndef Bot_h
#define Bot_h

#include <Arduino.h>
#include "variables.h"
#include "ultrasound.h"
#include "solar_panel.h"
#include "light.h"

class Bot
{
  public:
    Bot(int lc, int rc, int lp, int rp);

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
  private:
    // Motor control pins.
    int _left_control;
    int _right_control;
    // Motor power pins.
    int _left_power;
    int _right_power;
};

#endif
