

#include <SoftwareSerial.h>

#define base 200
#define coil1 6
#define coil2 5
int one = 0;              //one defines Minute_Hand
int two = 0;             //two defines Hour_Hand
bool Condition = true;  //Two Hands Working Condition

void setup() {

  Serial.begin(9600);

  pinMode (coil1, OUTPUT);
  pinMode (coil2, OUTPUT);
 
 }

void loop()
{
  while (Condition)
  {
   // one = analogRead(A0);
    two = analogRead(A1);
   
    
    
   // Serial.println(one);
   //Serial.print(", ");
    Serial.println(two);

    digitalWrite(coil1, HIGH);
    digitalWrite(coil2, LOW);
    delay(150);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, LOW);
    //delay(800);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, HIGH);
    delay(150);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, LOW);
    //delay(800);
    

    if (two>=10)
    {
      Condition = false;
      delay(100);
    }
  }

 
}



