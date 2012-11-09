#include "Blink.h"
#include "variables.h"

Blinker morse(LED_PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  morse.sayHi();
  delay(myNumber);
  morse.dot(); morse.dot(); morse.dot();
  morse.dash(); morse.dash(); morse.dash();
  morse.dot(); morse.dot(); morse.dot();
  delay(3000);
}
