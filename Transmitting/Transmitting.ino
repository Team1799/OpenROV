#include <SoftwareSerial.h>

SoftwareSerial xbee(2,3); // RX, TX

int value = 0;
boolean isUp = true;
void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  //Serial.println("Goodnight moon!");
  xbee.begin(9600);
  //xbee.println("Hello, world?");
}

void loop() // run over and over
{
  //if (xbee.available())
  //  Serial.write(xbee.read());
  //if (Serial.available())
  //  xbee.write(Serial.read());
  if(isUp){
    value++; 
  } else {
    value--; 
  }
  if(value==120)
    isUp = false;
  if(value==0)
    isUp = true;
  xbee.println(value+1);
  Serial.println(value);
  delay(20);
}

