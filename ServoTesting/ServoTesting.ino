#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  myservo.attach(9);
} 

void loop() {
  myservo.write(map(analogRead(A0),0,1023,0,180));
  delay(100);
} 
