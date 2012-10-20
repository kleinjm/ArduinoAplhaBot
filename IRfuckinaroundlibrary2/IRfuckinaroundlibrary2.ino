#include <IRremote.h>

int RECV_PIN = 10;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  //Serial.println("Running loop now");
  if (irrecv.decode(&results)) {
    
    Serial.println("Passed if statement");
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
}
