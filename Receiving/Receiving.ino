// Arduino Submarine Project
// Recieving Code
// Created by: Josh V.
// 
// Purpose
//   Receive values from a transmitting arduino and control motors with those values
// 
// Features
//   - Recieve values from Xbee and interpret them
//   - Transmits a constant to inform recieving arduino of connection status
//   - Turns off motors when values have not been received within the timeout for safety
//

// Library to create a serial input/output for Xbee and library to control motors
#include <SoftwareSerial.h>
#include <Servo.h>

// Setup serial for Xbee 
SoftwareSerial xbee(2,3);

// Time without receiving a message until determining no connection (1 sec = 1000)
int timeout = 5000;

// Setup servo (motor) variables
Servo front;
Servo left;
Servo right;
Servo wench;

// Recieved potentiometer values from joysticks and joystick buttons
int pot1x;  // Turning (front motor)
int pot1y;  // Forward Speed (left and right motor)
int pot2y;  // Submarine Reel (wench motor)

// Time variables for determining if there's a connection
long time;
long lastMessage;

// Value to store received data
unsigned int value;

void setup() {
  // Open up serial ports for computer and xbee for data transmission
  Serial.begin(9600);
  xbee.begin(9600);
  
  // Open up servo pins
  wench.attach(5);
  front.attach(9);
  left.attach(10);
  right.attach(11);
}


void loop() {
  // Record current time
  time = millis();
 
  // Wait until a message is received
  if (xbee.available()){
    // Get time last recieved a message
    lastMessage = millis();
    
    // Read and store recieved value
    value = xbee.parseInt();
    
    // "Flush" previously recieved data to not overwhelm the arduino
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
    
    // Return value to confirm connection
    xbee.println(420);
  }
  
  // Run the motors if there is an active connection
  if ((abs(lastMessage-time) < timeout)){
    front.write(pot1x);
    left.write(pot1y);
    right.write(pot1y);
    wench.write(pot2y);
  }
  
  // Turn the motors off if there's no connection to prevent a runaway boat
  else {
    front.write(90);
    left.write(90);
    right.write(90);
    wench.write(90);
  }
  
  // Delay to not overwhelm the arduino
  delay(50);
}

