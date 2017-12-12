

#include <SoftwareSerial.h>

#define base 200
#define coil1 6
#define coil2 5
#define coil3 3
#define coil4 2

int Minute_Hand = 0;            //one defines Minute_Hand
int Hour_Hand = 0;            //two defines Hour_Hand
bool Condition = true;  //Two Hands Working Condition

void setup() {

  Serial.begin(9600);

  pinMode (coil1, OUTPUT);
  pinMode (coil2, OUTPUT);
  
  pinMode (coil3, OUTPUT);
  pinMode (coil4, OUTPUT);
  
  pinMode(13 , OUTPUT);
  


}

void loop()
{
  while (Condition)
  {
    Minute_Hand = analogRead(A0);
    Hour_Hand = analogRead(A1);
   
    
    
    Serial.print(Minute_Hand);
    Serial.print(", ");
    Serial.println(Hour_Hand);

    /*digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
    delay(30);
    // delay(100);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(5);
    //delay(100);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, HIGH);
    delay(30);
     //elay(100);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(5);
     //delay(100);
    

    /*if ((Minute_Hand >=40 && Hour_Hand<=200) && (Hour_Hand>=50))
    {
      Condition = false;
      delay(100);
    }
  }*/

 
}
}


