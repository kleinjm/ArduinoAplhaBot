#ifndef Bot_h
#define Bot_h

#include <Arduino.h>
#include "ultrasound.h"
#include "variables.h"

class Bot
{
  public:
    Bot(int lc, int rc, int lp, int rp);
    Bot(int lc, int rc, int lp, int rp, Ultrasound* u);
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
  private:
    Ultrasound* _ultrasound;

    // Motor control pins.
    int _left_control;
    int _right_control;
    // Motor power pins.
    int _left_power;
    int _right_power;

};

#endif