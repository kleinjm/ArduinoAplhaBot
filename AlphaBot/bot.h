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

    void attach_ultrasound(Ultrasound* ptr);
    void attach_solar_panel(SolarPanel* ptr);
    void attach_light(Light* ptr);

    void tooClose();
    void slowNav();
    void selfNavigate();
  private:
    Ultrasound* _ultrasound_ptr;
    SolarPanel* _solar_panel_ptr;
    Light* _light_ptr;

    // Motor control pins.
    int _left_control;
    int _right_control;
    // Motor power pins.
    int _left_power;
    int _right_power;

};

#endif