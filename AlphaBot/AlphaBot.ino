/* 
Robot 1 is based off the old RAD platform with 2 motors and built in gear boxes
on each side. The movement is controled by tank treads. Other than this stock 
platform which is about a square foot, nothing is yet added for sensors, 
electronics, or mechanical funtioning.

Project Start date: 12/8/11
James Klein
*/

#include <Servo.h> //servo library

//initial global values
int distServoPin = 6; //2; //servo signal pin for servo that the ultrasonic distance sensor is attached to
Servo distSensorServo;  // create front servo object to control a servo
int distServoPosition = 85; // variable to store the servo position. 83 center; 0 far right; 166 far left

int ultraSoundSignal = 7; // Ultrasound signal pin (SIG on sensor)
unsigned long echo = 0; //initialize echo variable to 0
unsigned long ultrasoundValue = 0; //initialize ultrasound variable to 0

int panServoPin = 4; //servo pin for bottom servo on pan-tilt turret
Servo panServo; // create pan servo object
int panServoPosition = 90; //servo position setting. 86 = center; 6 = far right; 166 = far left

int tiltServoPin = 5; //servo pin for top servo on pan-tilt turret
Servo tiltServo; // create tilt servo object
int tiltServoPosition = 100;

int aMotorDir = 12; //motor A direction channel. Backward = LOW; Forwards = HIGH
int bMotorDir = 13; //motor B direction channel. Backward = LOW; Forwards = HIGH
int aMotorPower = 3; //motor A power channel
int bMotorPower = 11; //motor B power channel

int lightPin = 10; //pin for the light on the front servo

//PhotoResistor pins for solar board
int topLight = A4;
int bottomLight = A3;
int leftLight = A2;
int rightLight = A5;

//motor speeds
int motorOff = 0; //off
int motorFull = 511; //full speed
int motorHalf = 127; //half speed
int motorQuarter = 125; //quarter speed

int i = 0;

//IR CODE
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
//END IR CODE

//master run statement when sketch starts
void setup(){
  int ultraSoundSignal = 7; // Ultrasound signal pin (SIG on sensor): This code is here because the program is not recognizing it as a global variable???     
  Serial.begin(9600);     // set up Serial library at 9600 bps. this initalizes
                          //the serial monitor for feedback from the board.
  Serial.println("Check for program and monitor start!"); //check that the program and monitor has started 
  
  //Set pin modes
  pinMode(ultraSoundSignal, OUTPUT); //distance sensor set pin mode
  
  //distSensorServo.attach(distServoPin);  // attaches the servo on servo pin to the servo object
  //panServo.attach(panServoPin);  // attaches the servo on servo pin to the servo object
  //tiltServo.attach(tiltServoPin);  // attaches the servo on servo pin to the servo object
  
  //Setup Motor Channel A
  pinMode(aMotorDir, OUTPUT); //Initiates Motor Channel A pin
  
  //Setup Motor Channel B
  pinMode(bMotorDir, OUTPUT); //Initiates Motor Channel A pin
  
  pinMode(lightPin, OUTPUT); //make the light pin output to turn the relay for the light
}
//master loop statement
void loop(void){
  //testDistServoMovement();
  //testDistServoAndSensor();   
  //testMotors();
  //testPanAndTiltServos();
  //testLightSensors();
  
  //tooClose(40, 30);
  
  //mobilePanel();
  //chargeMode();
  //selfNavigate();
  //slowNav();
  //lightControl();
  
  //IR CODE
  int numberpulses = listenForIR();
  
  Serial.print("Heard ");
  Serial.print(numberpulses);
  Serial.println("-pulse long IR signal");
  commandButton(numberpulses);
  delay(500);
  //END IR CODE
}

//Functions
//Distance senor code
unsigned long ping(){
   int ultraSoundSignal = 7;
   pinMode(ultraSoundSignal, OUTPUT); // Switch signalpin to output
   digitalWrite(ultraSoundSignal, LOW); // Send low pulse
   delayMicroseconds(2); // Wait for 2 microseconds
   digitalWrite(ultraSoundSignal, HIGH); // Send high pulse
   delayMicroseconds(5); // Wait for 5 microseconds
   digitalWrite(ultraSoundSignal, LOW); // Holdoff
   pinMode(ultraSoundSignal, INPUT); // Switch signalpin to input
   digitalWrite(ultraSoundSignal, HIGH); // Turn on pullup resistor
   echo = pulseIn(ultraSoundSignal, HIGH); //Listen for echo
   ultrasoundValue = (echo / 58.138);// * .39; //convert to CM then to inches. Multiply by .39 to get inches.
   return ultrasoundValue;
}
//self navigate at a moderately slow speed
void slowNav(){
  int slightRight;
  int slightLeft;
  int forward;
  int closeDistance = 30; //distance directly in front of the bot that causes it to stop
  int closeSides = 50; //distance that the bot is too close to something slightly right or left
  int distServoCenter = 85; //forward looking sensor
  int distServoSlightLeft = 115;  //+30 from center
  int distServoSlightRight = 55;   //-30 from center
  int scanSpeed = 20; //delay in scan
  
  distSensorServo.attach(distServoPin); //attach the servo for the distance sensor
  while(distServoPosition < distServoSlightLeft){ //scan left
    slowForward();
    distServoPosition++;
    distSensorServo.write(distServoPosition);
    if(distServoPosition == distServoCenter){
      forward = ping();
      if(forward < closeDistance){
        slowBackward();
        delay(600);
        stopMovement();
        delay(50);
        tooClose();  //run procedure if bot is too close
      }else{
        slowForward();
      }
    }
    if(distServoPosition == distServoSlightLeft){
      slightLeft = ping();
      adjustLeft(closeSides, slightLeft);
    }
    delay(scanSpeed);
  }
  while(distServoPosition > distServoSlightRight){ //scan right
    slowForward();
    distServoPosition--;
    distSensorServo.write(distServoPosition);
    if(distServoPosition == distServoCenter){
      forward = ping();
      if(forward < closeDistance){
        slowBackward();
        delay(600);
        stopMovement();
        delay(50);
        tooClose();  //run procedure if bot is too close
      }else{
        slowForward();
      }
    }
    if(distServoPosition == distServoSlightRight){
      slightRight = ping();
      adjustRight(closeSides, slightRight);
    }
    delay(scanSpeed);
  }
}
//if the bot is too close on it's slight left then adjust the motor to straighten out
void adjustLeft(int closeSides, int slightLeft){
  if(slightLeft < closeSides){
    analogWrite(bMotorPower, motorQuarter - 80);
    analogWrite(aMotorPower, motorQuarter);
    delay(700);
  }
}
//if the bot is too close on it's slight right then adjust the motor to straighten out
void adjustRight(int closeSides, int slightRight){
  if(slightRight < closeSides){
    analogWrite(aMotorPower, motorQuarter - 80);
    analogWrite(bMotorPower, motorQuarter);
    delay(700);
  }
}
//if the bot is too close then backup and turn the way with the greatest distance
void tooClose(){
  int right; //right distance
  int left; //left distance
  //distSensorServo.attach(distServoPin); //for testing!

  distServoPosition = 170;  //look left
  distSensorServo.write(distServoPosition);
  delay(800); //time it takes to look left completely
  left = ping();
  distServoPosition = 0;  //look right
  distSensorServo.write(distServoPosition);
  delay(800); //time it takes to look right completely
  right = ping();
  distServoPosition = 85;  //center the position again
  distSensorServo.write(distServoPosition);
  if(left > right){
    slowLeft();
    delay(1100);
    stopMovement();
  }else{
    slowRight();
    delay(1100);
    stopMovement();
  }
}
//turn on the light in front if the surrounding light is below a certain point
void lightControl(){
  int lightLevel = 300; //if the light on all sensors falls below this, turn on light
  
  if(analogRead(topLight) < lightLevel && analogRead(bottomLight) < lightLevel && 
     analogRead(leftLight) < lightLevel && analogRead(rightLight) < lightLevel){
       digitalWrite(lightPin, HIGH);
  }else{
    digitalWrite(lightPin, LOW);
  }
}
//Self navigates at full speed without solar charging
void selfNavigate(){
  int right = 0;  //right distance
  int left = 0; //left distance
  distSensorServo.write(83); //set sensor forward

  mobilePanel(); //set panel in stationary position
  int x = ping(); //x represtents the distance returned by the distance sensor code
  delay(125); //delay 1/8 seconds
  if(x < 40){
    stopMovement();
    delay(1000);
    distSensorServo.write(0); //turn servo right fully
    delay(500); //allow time to move servo
    right = ping(); //get distance again
    delay(125); //delay 1/8 seconds
    
    distSensorServo.write(166); //turn servo left fully
    delay(500); //allow time to move servo
    left = ping(); //get distance again
    delay(500); //delay 1/8 seconds
    distSensorServo.write(83); //set sensor forward
    delay(250);
    
    if(right >= left){
      turnRight();
      delay(500);
      stopMovement();
      delay(125);
    }else{
      turnLeft();
      delay(500);
      stopMovement();
      delay(125);
  }
  }
  else{
    moveForward();
  }
}
//adjust the solar panel position based on the light's location. Turn bot if it is past 180 degree access
void chargeMode(){
  int range = 10; //difference in values that will determine the center light
  int servoSpeed = 2; //rate at which servos move to follow light
  //testLightSensors();   //Read out positions
  
  if(analogRead(topLight) > analogRead(bottomLight) + range && tiltServoPosition >= 160){
    //turn the bot 180
    turnLeft();
    delay(750);
    stopMovement();
    //set the panel down a bit
    tiltServoPosition = 150;
    tiltServo.attach(tiltServoPin);  // attaches the servo on servo pin to the servo object
    tiltServo.write(tiltServoPosition);
    delay(1000);
  }else{
    if(analogRead(topLight) > analogRead(bottomLight) + range && tiltServoPosition < 160){
      tiltServoPosition = tiltServoPosition + servoSpeed;
      tiltServo.attach(tiltServoPin);  // attaches the servo on servo pin to the servo object
      tiltServo.write(tiltServoPosition);
    }else if(analogRead(bottomLight) > analogRead(topLight) + range && tiltServoPosition > 74){
      tiltServoPosition = tiltServoPosition - servoSpeed;
      tiltServo.attach(tiltServoPin);  // attaches the servo on servo pin to the servo object
      tiltServo.write(tiltServoPosition);
    }else {
      tiltServo.detach();
    }
  }
  if(panServoPosition <= 10){
    //turn the bot right to allow it to get more light in that direction
    turnRight();
    delay(300);
    stopMovement();
    //reset the panel back a bit
    panServoPosition = 20;
    panServo.attach(panServoPin); 
    panServo.write(panServoPosition);
    delay(1000);
  }else if(panServoPosition >= 170){
    //turn the bot left
    turnLeft();
    delay(300);
    stopMovement();
    //reset the panel position left a bit
    panServoPosition = 160;
    panServo.attach(panServoPin);
    panServo.write(panServoPosition);
    delay(1000);
  }else{
    if(analogRead(rightLight) > analogRead(leftLight) + range && panServoPosition < 170){
      panServoPosition = panServoPosition + servoSpeed;
      panServo.attach(panServoPin); 
      panServo.write(panServoPosition);
    }else if(analogRead(leftLight) > analogRead(rightLight) + range && panServoPosition > 10){
      panServoPosition = panServoPosition - servoSpeed;
      panServo.attach(panServoPin);
      panServo.write(panServoPosition);
    }else {
      panServo.detach();
    }
  }
  delay(100);
}


//place panel on top and center of the bot
void mobilePanel(){
  tiltServo.attach(tiltServoPin);
  tiltServo.write(130);
  Serial.println(tiltServo.read());
  
  tiltServo.detach();
  Serial.println("done");
  /*
  int tiltBack = 120;
  int servoSpeed = 50; //delay between movements
  Serial.println(tiltServo.read());
  
  if(tiltServo.read() < tiltBack && panServo.read() != 90){
    panServo.attach(panServoPin);
    tiltServo.attach(tiltServoPin);
    while(panServo.read() != 90){
      if(panServo.read() > 90){    
        panServoPosition--;
        panServo.write(panServoPosition);
      }else{        
        panServoPosition++;
        panServo.write(panServoPosition);
      }
      delay(servoSpeed);
    }
    while(tiltServo.read() < tiltBack){
      Serial.println(tiltServo.read());
      tiltServoPosition++;
      tiltServo.write(tiltServoPosition);
      delay(servoSpeed);
    }
  }
  panServo.detach();
  tiltServo.detach();
  */
}
//Movements
void stopMovement (){
analogWrite(aMotorPower, motorOff);
analogWrite(bMotorPower, motorOff);
}
void moveForward () 
{
digitalWrite(aMotorDir, LOW);
digitalWrite(bMotorDir, LOW);
analogWrite(aMotorPower, motorFull);
analogWrite(bMotorPower, motorFull);
}
void moveBackward ()
{
digitalWrite(aMotorDir, HIGH);
digitalWrite(bMotorDir, HIGH);
analogWrite(aMotorPower, motorFull);
analogWrite(bMotorPower, motorFull);
}
void turnLeft ()
{
digitalWrite(aMotorDir, HIGH);
digitalWrite(bMotorDir, LOW);
analogWrite(aMotorPower, motorHalf);
analogWrite(bMotorPower, motorHalf);
}
void turnRight ()
{
digitalWrite(aMotorDir, LOW);
digitalWrite(bMotorDir, HIGH);
analogWrite(aMotorPower, motorHalf);
analogWrite(bMotorPower, motorHalf);
}
//Slower movements for crawling
void slowBackward ()
{
digitalWrite(aMotorDir, HIGH);
digitalWrite(bMotorDir, HIGH);
analogWrite(aMotorPower, motorQuarter);
analogWrite(bMotorPower, motorQuarter);
}
void slowForward ()
{
digitalWrite(aMotorDir, LOW);
digitalWrite(bMotorDir, LOW);
analogWrite(aMotorPower, motorQuarter);
analogWrite(bMotorPower, motorQuarter);
}
void slowLeft ()
{
digitalWrite(aMotorDir, HIGH);
digitalWrite(bMotorDir, LOW);
analogWrite(aMotorPower, 100);
analogWrite(bMotorPower, 100);
}
void slowRight ()
{
digitalWrite(aMotorDir, LOW);
digitalWrite(bMotorDir, HIGH);
analogWrite(aMotorPower, 100);
analogWrite(bMotorPower, 100);
}
//TESTS
void testLightSensors(){
  String top = " Top: ";
  String bottom = " Bottom: ";
  String left = " Left: ";
  String right = " Right: ";
  Serial.println(top + analogRead(topLight) + bottom + analogRead(bottomLight) 
  + left + analogRead(leftLight) + right + analogRead(rightLight));
  delay(125);
}
void testPanAndTiltServos(){
//limits should be even for speed adjustments in chargeMode()
  panServo.write(10); //left limit
  delay(1000);
  panServo.write(170); //right limit
  delay(1000);
  panServo.write(90); //center
  delay(1000);
  tiltServo.write(74);  //bottom limit
  delay(1000);
  tiltServo.write(160);  //top limit  THIS IS FLAT
  delay(1000);
  tiltServo.write(118);  //center
  delay(1000);
}
void testMotors(){
  moveForward();
  delay(1000);
  stopMovement();
  delay(2000);
  moveBackward();
  delay(1000);
  stopMovement();
  delay(2000);
  turnRight();
  delay(1000);
  stopMovement();
  delay(2000);
  turnLeft();
  delay(1000);
  stopMovement();
  delay(2000);
}
void testDistServoMovement(){
  distSensorServo.attach(distServoPin);
  distSensorServo.write(0); //right most position
  delay(1000);
  distSensorServo.write(85); //center position
  delay(1000);
  distSensorServo.write(170); //left most position
  delay(1000);
  distSensorServo.write(85); //center position
  delay(1000);
}
//bad test!!! should use loop or modulo. must make global variable: int i = 0;
void testDistServoAndSensor(){
  String rightDist = "right distance: ";
  String centerDist = "center distance: ";
  //String leftDistance = "left distance: ";    //MESSES UP IR
  if(i == 0){
    distSensorServo.write(i); //right most position
    delay(1000);
    int x = ping(); //x represtents the distance returned by the distance sensor code
    Serial.println(rightDist + x); //show distance in prompt
    delay(750);
    i = 83;
  } else if(i == 83){
    distSensorServo.write(i); //right most position
    delay(1000);
    int x = ping(); //x represtents the distance returned by the distance sensor code
    Serial.println(centerDist + x); //show distance in prompt
    delay(750);
    i = 166;
  }
  else if(i == 166){
    distSensorServo.write(i); //right most position
    delay(1000);
    int x = ping(); //x represtents the distance returned by the distance sensor code
    Serial.println(x); //show distance in prompt
    delay(750);
    i = 0;
  }
}


//IR CODE
//what each button does
void commandButton(int numberpulses){
  if (IRcompare(numberpulses, sixUp,sizeof(sixUp)/4)) {
    //Serial.println("six");
    mobilePanel(); 
  }
  if (IRcompare(numberpulses, sixDown,sizeof(sixDown)/4)) {
    //chargeMode();
  }
  if (IRcompare(numberpulses, fiveUp,sizeof(fiveUp)/4)) {
    //selfNavigate();
  }
  if (IRcompare(numberpulses, fiveDown,sizeof(fiveDown)/4)) {
    //slowNav();
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
//END IR CODE
