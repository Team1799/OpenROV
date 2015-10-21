//int motMin = 0;
//int motMax = 180;

void setup(){
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

void loop(){
  int value = analogRead(A0);
  Serial.println(adjustRange(value, 1, 493, 494, 1020));
  Serial.println(value);
  //Serial.println(0);
  delay(1000);
}
int adjustRange(int raw, int low, int midLow, int midHigh, int high){
   value = 
   if (raw < midLow){
     return map(raw,low,midLow,0,512);
   }
   else if (raw > midHigh){
     return map(raw,midHigh,high,512,1023);
   }
   return 512;
}
