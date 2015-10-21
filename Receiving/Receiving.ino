// Arduino Submarine Project
// Boat Code
// Created by: Josh V.
// 
// Purpose
//   Receive values from transmitting arduino and control motors with it
//
#include <SoftwareSerial.h>
#include <Servo.h>

// Setup xbee pins 0.
SoftwareSerial xbee(2,3);

// Setup servo variables
Servo left;
Servo right;
Servo front;

// Potentiometer values from joysticks and joystick buttons
int pot1x;
int pot1y;
int pot2x;
int pot2y;
boolean joy1;
boolean joy2;

// Time variables for determining if there's a connection
long time;
long lastMessage;

// Time without receiving a message until determining no connection (1 sec = 1000)
int timeout = 5000;

// Value to store received data
unsigned int value;

void setup() {
  // Open up serial and xbee for data transmission
  Serial.begin(9600);
  xbee.begin(9600);
  
  // Open up servo pins
  left.attach(5);
  right.attach(6);
  front.attach(9);
  pinMode(12, OUTPUT); 
}


void loop() {
  // Get current time
  time = millis();
 
  // Wait until a message is received
  if (xbee.available()){
    // Get time last recieved a message
    lastMessage = millis();
    
    // Read and store recieved value
    value = xbee.parseInt();
    xbee.flush();
    
    // Make sure value isn't null (or zero)
    if(value!=0){
      // Interpret the value using the digit from the thousandth place and assign it to its cooresponding variable
      if(value/1000 == 1)
        pot1x = value%1000;
      if(value/1000 == 2)
        pot1y = value%1000;
      if(value/1000 == 4)
        pot2y = value%1000;
    }
    xbee.println(420);
    
    // Return value to confirm connection
  }
  
  // Prevents motor movement unless there is a connection
  
  
  if ((abs(lastMessage-time) < timeout)){
    front.write(pot1x);
  }
  else {
    front.write(90);
  }
  delay(50);
}

