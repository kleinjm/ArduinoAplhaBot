#ifndef helpers_h
#define helpers_h

#include <Arduino.h>

//This file and the idea of this file is incomplete and uncertain. Possibly delete. Can't decide if should be dynamic or static and if it should take the debug variable. Looking to extract the debug stuff
class Helper
{
  public:
    void debug(String msg); //prints the debug message if debug is on
  private:
};

#endif
