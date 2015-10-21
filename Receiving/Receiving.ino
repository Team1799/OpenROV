#include <SoftwareSerial.h>
#include <Servo.h> 


Servo myservo;
SoftwareSerial xbee(2,3); // RX, TX

void setup()  
{
  Serial.begin(9600);
  //Serial.println("Goodnight moon!");
  xbee.begin(9600);
  
  myservo.attach(5);
  //pinMode(5, OUTPUT);
  //xbee.println("Hello, world?");
}

void loop() // run over and over
{
  
  if (xbee.available()){
    int value = xbee.parseInt();
    if(value!=0){
      // Subtract the 1 added from transmitting end
      value -= 1;
      
      myservo.write(value);
      //analogWrite(5,value);
      Serial.println(value);
    }
  }
    //Serial.write(xbee.read());
  //if (Serial.available())
  //  xbee.write(Serial.read());
}

