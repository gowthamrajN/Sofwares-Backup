

#include <SoftwareSerial.h>

#define ON_TIME 80
#define OFF_TIME 300

#define hw false
#define sw true

bool stoggle = false;       //condition for sec
bool mtoggle = false;
int scounter = 0;



bool toggle = false;
SoftwareSerial bt(10, 11);

#define base 200
#define coil1 6
#define coil2 5
#define coil3 2
#define coil4 3




int one = 0;            //one defines Minute_Hand
int two = 0;            //two defines Hour_Hand
bool Condition = true;  //Two Hands Working Condition
void setup() {

  Serial.begin(9600);
  bt.begin(9600);
  pinMode (coil1, OUTPUT);
  pinMode (coil2, OUTPUT);
  pinMode (coil3, OUTPUT);
  pinMode (coil4, OUTPUT);


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
    OneSecond_Ticking();
  }
}

void processSerial(bool source)
{
  String inString;
  long difference;
  if (source == hw)
    inString = Serial.readString();
  else if (source == sw)
    inString = bt.readString();

  String dh = inString.substring(0, 2);
  String dm = inString.substring(2, 4);

  int destHours = dh.toInt();
  int destMins = dm.toInt();
  if ((destHours == 12) && (destMins >= 0))
  {
    difference = ((destHours - 12) * 360) + ((destMins - 00) * 6);
  }
  else
  {
    difference = ((destHours - 00) * 360) + ((destMins - 00) * 6);
  }

  Serial.println("Zero Setting Started");
  bt.println("Connected with Clock");
  while (Condition)
  {
    one = analogRead(A0);
    two = analogRead(A1);
    one = (one - base);
    one = abs(one);
    two = two - base;
    two = abs(two);
    Serial.println(one);
    Serial.println(two);

    digitalWrite(coil1, HIGH);
    digitalWrite(coil2, LOW);
    delay(40);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, LOW);
    delay(10);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, HIGH);
    delay(40);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, LOW);
    delay(10);

    if ((one >= 15) && (two >= 15))
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
 
  delay(500);
 // if (difference > 0)
  {
    Serial.println("Travel Ahead...");
    for (int i = 0; i < difference; i++)
    {
      sweepOneSec(40);
      delay(10);

    }
  }
//  else if (difference < 0)
//  {
//    Serial.println("Travel Back...");
//    for (int i = 0; i < abs(difference); i++)
//    {
//      sweepOneSec(55);
//      delay(40);
//
//    }
//  }
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

void OneSecond_Ticking()
{

  if (stoggle == false)
  {
    digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
    delay(60);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(940);
    stoggle = true;
    scounter++;
  }
  else
  {
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, HIGH);
    delay(60);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(940);

    stoggle = false;
    scounter++;
  }

  if (scounter >= 10)
  {


    if (mtoggle == false)
    {
      digitalWrite(coil1, HIGH);
      digitalWrite(coil2, LOW);
      delay(50);
      digitalWrite(coil1, LOW);
      digitalWrite(coil2, LOW);
      delay(40);
      mtoggle = true;
      scounter = 0;
    }
    else
    {
      digitalWrite(coil1, LOW);
      digitalWrite(coil2, HIGH);
      delay(50);
      digitalWrite(coil1, LOW);
      digitalWrite(coil2, LOW);
      delay(40);

      mtoggle = false;
      scounter = 0;
    }

  }
}



