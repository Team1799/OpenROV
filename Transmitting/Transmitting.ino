// Arduino Submarine Project
// Controller Code
// Created by: Josh V.
// 
// Purpose
//   Receive values from joystick(s) and transmit it to the other Arduino using xbee's
//
#include <SoftwareSerial.h>

// Setup xbee pins 
SoftwareSerial xbee(2,3);


// Time without receiving a message until determining no connection (1 sec = 1000)
long timeout = 1000;

// Interval time before retransmitting the same value
int interval = 500;

// Potentiometer values from joysticks and joystick buttons
int pot1x;
int pot1y;
int pot2y;
int joy1;

// Older values of joysticks
int pot1xOld;
int pot1yOld;
int pot2yOld;
int joy1Old;

// Last potentiometer change
long pot1xLC = 0 - interval;
long pot1yLC = 0 - interval;
long pot2yLC = 0 - interval;

// Motor max and min values to scale potentiometer values
int motMin = 0;
int motMax = 180;

// Time variables for determining if there's a connection
long time;
long lastMessage = 0;
long lastButton = 0;

// Boolean to determine if conencted to boat
boolean isConnected = false;
boolean beastMode = false;

// Value to store received data
unsigned int value;

void setup() {
  // Open up serial and xbee for data transmission
  Serial.begin(9600);
  xbee.begin(9600);
  
  // Setup pins for input
  pinMode(A0, INPUT);  // pot1x
  pinMode(A1, INPUT);  // pot1y
  //pinMode(A2, INPUT);// pot2x
  pinMode(A3, INPUT);  // pot2y
  pinMode(4, INPUT);   
  //pinMode(5, INPUT); // joy2
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT); // LEDs for displaying connection status
  pinMode(12, OUTPUT); 
  digitalWrite(4, HIGH);
}


void loop() {
  // Get current time
  time = millis();
  
  // Wait until a message is received
  if (xbee.available()){
    // Read and store recieved value
    value = xbee.parseInt();
    xbee.flush();
    // Make sure value isn't null (or zero)
    if(value == 420){
      // Get time last recieved a message and set connection status to true
      lastMessage = millis();
      isConnected = true;
    }
  }
  
  pot1x = 180-adjustRange(analogRead(A0), 1, 493, 494, 1020);
  if(( pot1xOld != pot1x ) || ( abs(time-pot1xLC) > interval )){
    xbee.println(1000+pot1x);
    pot1xLC = time;
    Serial.println(pot1x);
  }
  pot1xOld = pot1x;
  
  
  
  pot1y = adjustRange(analogRead(A1), 1, 505, 505, 1021);
  if(( pot1yOld != pot1y ) || ( abs(time-pot1yLC) > interval )){
    xbee.println(2000+pot1y);
    pot1yLC = time;
  }
  pot1yOld = pot1y;
  //Serial.println(pot1y);
  
  pot2y = adjustRange(analogRead(A3), 3, 501, 501, 1018);
  if(( pot2yOld != pot2y ) || ( abs(time-pot2yLC) > interval )){
    xbee.println(4000+pot2y);
    pot2yLC = time;
  }
  pot2yOld = pot2y;
  //Serial.println(pot2y);
  //Serial.println(0);
  
  joy1 = digitalRead(4);
  if(joy1 == HIGH && joy1Old == LOW && time - lastButton > 200){
    if (beastMode)
      beastMode = false;
    else
      beastMode = true;
    lastButton = millis();
  }
  joy1Old = joy1;
  
    
  // If last message received is longer than timeout, set connection status to false
  if ((abs(lastMessage-time) > timeout)){
    isConnected = false;
  }

  if (isConnected) {
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
  } else {
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
  }
  
  if (beastMode) {
    digitalWrite(10,HIGH); 
  } else {
    digitalWrite(10,LOW);
  }
  
  
  // Delay for accurate pot readings
  delay(150);
}

int adjustRange(int raw, int low, int midLow, int midHigh, int high){
   int i;
   if (raw < midLow){
     i = map(raw,low,midLow,0,512);
   } else if (raw > midHigh){
     i = map(raw,midHigh,high,512,1023);
   } else {
     i = 512;
   }
   
   // If beastmode is on, then use full 180 degrees of freedom
   if (beastMode){
     return map(i,0,1023,0,180);
   }
   
   // Otherwise, use 90 degrees of freedom
   return map(i,0,1023,45,135);
}

