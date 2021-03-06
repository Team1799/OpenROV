// Arduino Submarine Project
// Transmitting Code
// Created by: Josh V.
// 
// Purpose
//   Retrieve values from joysticks and button(s) and transmit them to another Arduino using Xbees
// 
// Features
//   - Retrieve and correct values from the joystick's potentiometers
//   - Efficiently transmit potentiometer values
//   - "Beast Mode" to output the full range of the joystick when necessary
//   - Displays connection status through an LED
//

// Library to create a serial input/output for Xbee
#include <SoftwareSerial.h>

// Setup serial for Xbee 
SoftwareSerial xbee(2,3);

// Time without receiving a message until determining no connection (1 sec = 1000)
long timeout = 1000;

// Time interval before retransmitting the same potentiometer value
int interval = 500;

// Potentiometer values from joysticks and joystick buttons
int pot1x;
int pot1y;
int pot2y;
int joy1;
int led;

// Older values of potentiometers to determine if they've change
int pot1xOld;
int pot1yOld;
int pot2yOld;
int joy1Old;
int ledOld;

// Time since last potentiometer value changed
long pot1xLC = 0 - interval;
long pot1yLC = 0 - interval;
long pot2yLC = 0 - interval;

// Time variables for determining if there's a connection
long time;
long lastMessage = 0;
long lastButton = 0;

// Boolean values to determine if conencted to boat
boolean isConnected = false;
boolean beastMode = false;

// Value to temporarily store received data. Used to determine connection status
int value;

void setup() {
  // Open up serial ports for computer and xbee for data transmission
  Serial.begin(9600);
  xbee.begin(9600);
  
  // Setup arduino pins for input and output
  pinMode(A0, INPUT);  // pot1x
  pinMode(A1, INPUT);  // pot1y
  pinMode(A3, INPUT);  // pot2y
  pinMode(4, INPUT);   // joy1
  pinMode(10, OUTPUT); // beast mode LED
  pinMode(11, OUTPUT); // yellow connection LED
  pinMode(12, OUTPUT); // green connection LED
  
  led = LOW;
  digitalWrite(4, HIGH); // enable pull-up resistor for joystick button
}


void loop() {
  // Record current time
  time = millis();
  
  // Wait until a message is received
  if (xbee.available()){
    // Read and store recieved value
    value = xbee.parseInt();
    
    // "Flush" previously recieved data to not overwhelm the arduino
    xbee.flush();
    // If retrieved value is 420, then confirm connection status
    if(value == 420){
      // Get time last recieved a message and set connection status to true
      lastMessage = millis();
      isConnected = true;
    }
  }
  
  // Takes potentiometer value, and if it's different than the previous value, or if it's 
  // been past the interval amount of time without changing values, then transmit the value
  pot1x = adjustRange(analogRead(A0), 1, 493, 494, 1020, 1);
  if(( pot1xOld != pot1x ) || ( abs(time-pot1xLC) > interval )){
    xbee.println(1000+pot1x);
    pot1xLC = time;
    Serial.println(pot1x);
  }
  
  
  pot1y = adjustRange(analogRead(A1), 1, 505, 505, 1021, 2);
  if(( pot1yOld != pot1y ) || ( abs(time-pot1yLC) > interval )){
    xbee.println(2000+pot1y);
    pot1yLC = time;
  }

  // Repeat same process as pot1x, except sending as a smaller integer
  pot2y = adjustRange(analogRead(A3), 3, 501, 501, 1018, 4);
  if(( pot2yOld != pot2y ) || ( abs(time-pot2yLC) > interval )){
    xbee.println(4000+pot2y);
    pot2yLC = time;
  }
  
  
  // Take joystick button and treat it as a switch, switching beast mode values each time
  // it is pressed. Uses similar process as previous potentiometers
  joy1 = digitalRead(4);
  if(joy1 == HIGH && joy1Old == LOW && time - lastButton > 100){
    if (beastMode)
      beastMode = false;
    else
      beastMode = true;
    lastButton = millis();
  }
  
  // Set current values as the old values to determine if future values have changed
  pot1xOld = pot1x;
  pot1yOld = pot1y;
  pot2yOld = pot2y;
  joy1Old = joy1;
  
  // If last message received is longer than timeout, set connection status to false
  if ((abs(lastMessage-time) > timeout)){
    isConnected = false;
  }

  // Turn connection LED on or off if connected
  if (isConnected){
    digitalWrite(12, HIGH);
  } else { 
    if (time - ledOld >= 300) {
      ledOld = time;
      
      if (led == LOW)
        led = HIGH;
      else
        led = LOW;

      digitalWrite(12, led);
    }
  }
  
  // Turn beastmode LED on or off if enabled
  if (beastMode) {
    digitalWrite(10, HIGH);
  } else {
    digitalWrite(10, LOW);
  }
  
  // Delay for accurate pot readings and to not overload the receiving arduino
  delay(150);
}

// Funtion to adjust range from potentiometers where the resting point isn't in the middle
int adjustRange(int raw, int low, int midLow, int midHigh, int high, int type){
   // Value to be manipulated and returned in the end
   int i;
   
   // Readjust the median from the potentiometer
   if (raw < midLow){
     i = map(raw,low,midLow,0,512);
   } else if (raw > midHigh){
     i = map(raw,midHigh,high,512,1023);
   } else {
     i = 512;
   }
   
   // Beastmode to determine whether to use the full range of the potentiometer or not
   if (beastMode || type == 4){
     i = map(i,0,1023,0,180);
   } else {
     i = map(i,0,1023,45,135);
   }
   
   // Adjust range specifically to pot1x and the motor's specifications
   if (type == 1){
     // Flip x-axis from 180-0 to 0-180
     i = 180 - i;
     
     if (i < 90){
       i = map(i,0,90,20,83);
     } else if (i > 90){
       i = map(i,90,180,98,180);
     } else {
       i = 90;
     }
   }
   
   // Adjust range specifically to pot1y and the motor's specifications
   if (type == 2){
     if (i > 90){
       i = map(i,90,180,98,180);
     } else {
       i = 90;
     }
   }
   
   // Return adjusted value back into the program
   return i;
}


