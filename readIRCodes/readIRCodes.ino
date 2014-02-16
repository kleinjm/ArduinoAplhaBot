/* Raw IR commander
This sketch/program uses the Arduno and a PNA4602 to
decode IR received. It then attempts to match it to a previously
recorded IR signal
Code is public domain, check out www.ladyada.net and adafruit.com
for more tutorials!
*/

// We need to use the 'raw' pin reading methods
// because timing is very important here and the digitalRead()
// procedure is slower!
//uint8_t IRpin = 2;
// Digital pin #2 is the same as Pin D2 see
// http://arduino.cc/en/Hacking/PinMapping168 for the 'raw' pin mapping
#define IRpin_PIN PIND
#define IRpin 2

// the maximum pulse we'll listen for - 65 milliseconds is a long time
#define MAXPULSE 65000
#define NUMPULSES 50

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20

// What percent we will allow in variation to match the same code
#define FUZZINESS 20

// we will store up to 100 pulse pairs (this is -a lot-)
uint16_t pulses[NUMPULSES][2]; // pair is high and low pulse
uint8_t currentpulse = 0; // index for pulses we're storing

#include <irCodes.h>

/******************* our codes ************/

void setup(void) {
  Serial.begin(9600);
  Serial.println("Ready to decode IR!");
}

void loop(void) {
  int numberpulses;
  
  numberpulses = listenForIR();
  
  Serial.print("Heard ");
  Serial.print(numberpulses);
  Serial.println("-pulse long IR signal");
  commandButton(numberpulses);
  delay(500);
}
void commandButton(int numberpulses){
  if (IRcompare(numberpulses, sixUp,sizeof(sixUp)/4)) {
    Serial.println("Six Up");
  }
  if (IRcompare(numberpulses, sixDown,sizeof(sixDown)/4)) {
    Serial.println("Six Down");
  }
  if (IRcompare(numberpulses, fiveUp,sizeof(fiveUp)/4)) {
    Serial.println("Five Up");
  }
  if (IRcompare(numberpulses, fiveDown,sizeof(fiveDown)/4)) {
    Serial.println("Five Down");
  }
  if (IRcompare(numberpulses, fourUp,sizeof(fourUp)/4)) {
    Serial.println("Four Up");
  }
  if (IRcompare(numberpulses, fourDown,sizeof(fourDown)/4)) {
    Serial.println("Four Down");
  }
  if (IRcompare(numberpulses, threeUp,sizeof(threeUp)/4)) {
    Serial.println("Three Up");
  }
  if (IRcompare(numberpulses, threeDown,sizeof(threeDown)/4)) {
    Serial.println("Three Down");
  }
  if (IRcompare(numberpulses, twoUp,sizeof(twoUp)/4)) {
    Serial.println("Two Up");
  }
  if (IRcompare(numberpulses, twoDown,sizeof(twoDown)/4)) {
    Serial.println("Two Down");
  }
  if (IRcompare(numberpulses, oneUp,sizeof(oneUp)/4)) {
    Serial.println("One Up");
  }
  if (IRcompare(numberpulses, oneDown,sizeof(oneDown)/4)) {
    Serial.println("One Down");
  }
}
//KGO: added size of compare sample. Only compare the minimum of the two
boolean IRcompare(int numpulses, int Signal[], int refsize) {
  int count = min(numpulses,refsize);
  for (int i=0; i< count-1; i++) {
    int oncode = pulses[i][1] * RESOLUTION / 10;
    int offcode = pulses[i+1][0] * RESOLUTION / 10;
    
#ifdef DEBUG
    Serial.print(oncode); // the ON signal we heard
    Serial.print(" - ");
    Serial.print(Signal[i*2 + 0]); // the ON signal we want
#endif
    
    // check to make sure the error is less than FUZZINESS percent
    if ( abs(oncode - Signal[i*2 + 0]) <= (Signal[i*2 + 0] * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif
    } else {
#ifdef DEBUG
      Serial.print(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }
    
    
#ifdef DEBUG
    Serial.print(" \t"); // tab
    Serial.print(offcode); // the OFF signal we heard
    Serial.print(" - ");
    Serial.print(Signal[i*2 + 1]); // the OFF signal we want
#endif
    
    if ( abs(offcode - Signal[i*2 + 1]) <= (Signal[i*2 + 1] * FUZZINESS / 100)) {
#ifdef DEBUG
      Serial.print(" (ok)");
#endif
    } else {
#ifdef DEBUG
      Serial.print(" (x)");
#endif
      // we didn't match perfectly, return a false match
      return false;
    }
    
#ifdef DEBUG
    Serial.println();
#endif
  }
  // Everything matched!
  return true;
}

int listenForIR(void) {
  currentpulse = 0;
  
  while (1) {
    uint16_t highpulse, lowpulse; // temporary storage timing
    highpulse = lowpulse = 0; // start out with no pulse length
  
// while (digitalRead(IRpin)) { // this is too slow!
    while (IRpin_PIN & (1 << IRpin)) {
       // pin is still HIGH

       // count off another few microseconds
       highpulse++;
       delayMicroseconds(RESOLUTION);

       // If the pulse is too long, we 'timed out' - either nothing
       // was received or the code is finished, so print what
       // we've grabbed so far, and then reset
       
       // KGO: Added check for end of receive buffer
       if (((highpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    // we didn't time out so lets stash the reading
    pulses[currentpulse][0] = highpulse;
  
    // same as above
    while (! (IRpin_PIN & _BV(IRpin))) {
       // pin is still LOW
       lowpulse++;
       delayMicroseconds(RESOLUTION);
        // KGO: Added check for end of receive buffer
        if (((lowpulse >= MAXPULSE) && (currentpulse != 0))|| currentpulse == NUMPULSES) {
         return currentpulse;
       }
    }
    pulses[currentpulse][1] = lowpulse;

    // we read one high-low pulse successfully, continue!
    currentpulse++;
  }
}

void printpulses(void) {
  Serial.println("\n\r\n\rReceived: \n\rOFF \tON");
  for (uint8_t i = 0; i < currentpulse; i++) {
    Serial.print(pulses[i][0] * RESOLUTION, DEC);
    Serial.print(" usec, ");
    Serial.print(pulses[i][1] * RESOLUTION, DEC);
    Serial.println(" usec");
  }
  
  // print it in a 'array' format
  Serial.println("int IRsignal[] = {");
  Serial.println("// ON, OFF (in 10's of microseconds)");
  for (uint8_t i = 0; i < currentpulse-1; i++) {
    Serial.print("\t"); // tab
    Serial.print(pulses[i][1] * RESOLUTION / 10, DEC);
    Serial.print(", ");
    Serial.print(pulses[i+1][0] * RESOLUTION / 10, DEC);
    Serial.println(",");
  }
  Serial.print("\t"); // tab
  Serial.print(pulses[currentpulse-1][1] * RESOLUTION / 10, DEC);
  Serial.print(", 0};");
}