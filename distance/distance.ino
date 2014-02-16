
unsigned long echo = 0;
 int ultraSoundSignal = 7
 ; // Ultrasound signal pin
 unsigned long ultrasoundValue = 0;
 
 void setup()
 {
 Serial.begin(9600);
 pinMode(ultraSoundSignal,OUTPUT);
 }
 
 unsigned long ping(){
 pinMode(ultraSoundSignal, OUTPUT); // Switch signalpin to output
 digitalWrite(ultraSoundSignal, LOW); // Send low pulse
 delayMicroseconds(2); // Wait for 2 microseconds
 digitalWrite(ultraSoundSignal, HIGH); // Send high pulse
 delayMicroseconds(5); // Wait for 5 microseconds
 digitalWrite(ultraSoundSignal, LOW); // Holdoff
 pinMode(ultraSoundSignal, INPUT); // Switch signalpin to input
 digitalWrite(ultraSoundSignal, HIGH); // Turn on pullup resistor
  echo = pulseIn(ultraSoundSignal, HIGH); //Listen for echo
  ultrasoundValue = (echo / 58.138) * .39; //convert to CM then to inches. Multiply by .39 to get inches.
 return ultrasoundValue;
 }
 
 
 void loop()
 {
 int x = 0;
 x = ping();
  Serial.println(x);
  delay(250); //delay 1/4 seconds.
 }

