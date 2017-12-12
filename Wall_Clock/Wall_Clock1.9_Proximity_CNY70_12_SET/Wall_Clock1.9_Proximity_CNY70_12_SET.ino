

#include <SoftwareSerial.h>

#define base 200
#define coil1 6
#define coil2 5
int one = 0;            //one defines Minute_Hand
int two = 0;            //two defines Hour_Hand
bool Condition = true;  //Two Hands Working Condition

void setup() {

  Serial.begin(9600);

  pinMode (coil1, OUTPUT);
  pinMode (coil2, OUTPUT);
  pinMode(13 , OUTPUT);
  


}

void loop()
{
  while (Condition)
  {
    one = analogRead(A0);
    two = analogRead(A1);
   
    
    
    Serial.print(one);
    Serial.print(", ");
    Serial.println(two);

    digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
    //delay(25);
      delay(30);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    //delay(10);
     delay(5);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, HIGH);
    //delay(25);
     delay(30);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    //delay(10);
    delay(5);
    
/*
    if ((one >=50) && (two>=50 && two<=150 ))
    {
      Condition = false;
      delay(100);
    }*/
  }

 
}



