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


// Potentiometer values from joysticks and joystick buttons
int pot1x;
int pot1y;
int pot2x;
int pot2y;
boolean joy1;
boolean joy2;

// Motor max and min values to scale potentiometer values
int motMin = 0;
int motMax = 180;

// Time variables for determining if there's a connection
long time;
long lastMessage;

// Time without receiving a message until determining no connection (1 sec = 1000)
long timeout = 5000;

// Boolean to determine if conencted to boat
boolean isConnected = false;

// Value to store received data
unsigned int value;

void setup() {
  // Open up serial and xbee for data transmission
  Serial.begin(9600);
  xbee.begin(9600);
  
  // Setup pins for input
  pinMode(A0, INPUT);  // pot1x
  pinMode(A1, INPUT);  // pot1y
  pinMode(A2, INPUT);  // pot2x
  pinMode(A3, INPUT);  // pot2y
  pinMode(4, INPUT);   // joy1
  pinMode(5, INPUT);   // joy2
  pinMode(11, OUTPUT); // LEDs for displaying connection status
  pinMode(12, OUTPUT); 
}


void loop() {
  // Get current time
  time = millis();
  
  // Wait until a message is received
  if (xbee.available()){
    // Read and store recieved value
    value = xbee.parseInt();
    
    // Make sure value isn't null (or zero)
    if(value == 420){
      // Get time last recieved a message and set connection status to true
      lastMessage = millis();
      isConnected = true;
    }
  }
  
  
  pot1x = 180-adjustRange(analogRead(A0), 1, 493, 494, 1020);
  //pot1x = 90;
  xbee.println(1000+pot1x);
  Serial.println(pot1x);
  
  
  pot1y = adjustRange(analogRead(A1), 1, 505, 505, 1021);
  xbee.println(2000+pot1y);
  //Serial.println(pot1y);
  
  pot2y = adjustRange(analogRead(A3), 3, 501, 501, 1018);
  xbee.println(4000+pot2y);
  Serial.println(pot2y);
  //Serial.println(0);
  
  // If last message received is longer than timeout, set connection status to false
  if ((abs(lastMessage-time) > timeout)){
    isConnected = false;
  }

  if(isConnected==true){digitalWrite(12, HIGH);digitalWrite(11, LOW);}
  else{digitalWrite(12, LOW);digitalWrite(11, HIGH);}
  // Delay for accurate pot readings
  delay(100);
}

int adjustRange(int raw, int low, int midLow, int midHigh, int high){
   if (raw < midLow){
     return map(raw,low,midLow,motMin,(motMax-motMin)/2);
   }
   else if (raw > midHigh){
     return map(raw,midHigh,high,(motMax-motMin)/2,motMax);
   }
   return (motMax-motMin)/2;
}

