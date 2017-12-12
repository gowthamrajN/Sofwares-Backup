

#include <SoftwareSerial.h>

#define ON_TIME 80
#define OFF_TIME 300

#define hw false
#define sw true

bool toggle = false;
SoftwareSerial bt(10, 11);

#define base 200
#define coil1 6
#define coil2 5

int one=0;
int two=0;
bool Condition = true;
void setup() {
  
  Serial.begin(9600);
  bt.begin(9600);
  pinMode (coil1,OUTPUT);
  pinMode (coil2,OUTPUT); 

}

void loop() 
{
  
Serial.println("IN_LOOP");

 if (Serial.available() > 0)
    processSerial(hw);
  else if (bt.available() > 0)
    processSerial(sw);
  else
  {
    sweepOneSec(70);    //sweepOneSec(ON_Time);
    
    delay(10000-ON_TIME );
  }
}

void processSerial(bool source)
{
  String inString;
  if (source == hw)
    inString = Serial.readString();
  else if (source == sw)
    inString = bt.readString();

  String dh = inString.substring(0, 2);
  String dm = inString.substring(2, 4);

  int destHours = dh.toInt();
  int destMins = dm.toInt();
  long difference = ((destHours - 12) * 360) + ((destMins - 00) * 6);
  
  Serial.println("Zero Setting Started");
  bt.println("Connected with Clock");
  while(Condition)
  {
    one=analogRead(A0);
    two=analogRead(A1);
    one= (one - base);
    one =abs(one);
    two =two-base;
    two =abs(two);
    Serial.println(one);
    Serial.println(two);

    digitalWrite(coil1,HIGH);
    digitalWrite(coil2,LOW);
    delay(55);
    digitalWrite(coil1,LOW);
    digitalWrite(coil2,LOW);
    delay(40);
    digitalWrite(coil1,LOW);
    digitalWrite(coil2,HIGH);
    delay(55);
    digitalWrite(coil1,LOW);
    digitalWrite(coil2,LOW);
    delay(40);

    if((one>10) && (two>10))
    {
      Condition = false;
    }
  }
  Condition = true;
  if (source == hw)
  {

    Serial.print("Setting Time To:- ");
    Serial.print(destHours);
    Serial.print(":");
    Serial.println(destMins);
  }
  else
  {
  
    bt.print("Setting Time To:- ");
    bt.print(destHours);
    bt.print(":");
    bt.println(destMins);
  }
  

  if (difference > 0)
  {
    Serial.println("Travel Ahead...");
    for (int i = 0; i < difference; i++)
    {
      sweepOneSec(55);
      delay(40);
      //delayMicroseconds(700);
    }
  }
  else if (difference < 0)
  {
    Serial.println("Travel Back...");
    for (int i = 0; i < abs(difference); i++)
    {
      sweepOneSec(25);
      //delay(10);
        
    }
  }
}

void sweepOneSec(int onTime)
{
  if (toggle == false)
  {
    digitalWrite(coil1, HIGH);
    digitalWrite(coil2, LOW);
  }
  else
  {
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, HIGH);
  }
  delay(onTime);
  toggle = !toggle;
  digitalWrite(coil1, LOW);
  digitalWrite(coil2, LOW);
}






