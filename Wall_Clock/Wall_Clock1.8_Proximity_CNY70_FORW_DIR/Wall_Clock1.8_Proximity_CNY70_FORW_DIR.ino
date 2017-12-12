

#include <SoftwareSerial.h>



#define hw false
#define sw true

bool stoggle = false;       //condition for sec
bool toggle = false;
SoftwareSerial bt(10, 11);

#define base 200
#define coil1 6
#define coil2 5

int one = 0;            //one defines Minute_Hand
int two = 0;            //two defines Hour_Hand
bool Condition = true;  //Two Hands Working Condition
void setup() {

  Serial.begin(9600);
  bt.begin(9600);
  pinMode (coil1, OUTPUT);
  pinMode (coil2, OUTPUT);


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
    difference = ((destHours - 12) * 3600) + ((destMins - 00) * 60);
  }
  else
  {
    difference = ((destHours - 00) * 3600) + ((destMins - 00) * 60);
  }

  Serial.println("Zero Setting Started");
  bt.println("Connected with Clock");
  while (Condition)
  {
    one = analogRead(A0);
    two = analogRead(A1);


    Serial.println(one);
    Serial.println(two);

    digitalWrite(coil1, HIGH);
    digitalWrite(coil2, LOW);
    delay(25);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, LOW);
    //delay(10);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, HIGH);
    delay(25);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, LOW);
    //delay(10);

    if ((one >=50) && (two>=50 && two<=150 ))
    {
      Condition = false;
      delay(100);
    }
  }
  delay(500);
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
      sweepOneSec(25);
      // delay(10);

    }
  }
  else if (difference < 0)
  {
    Serial.println("Travel Back...");
    for (int i = 0; i < abs(difference); i++)
    {
      sweepOneSec(25);
      // delay(10);

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

void OneSecond_Ticking()
{

  if (stoggle == false)
  {
    digitalWrite(coil1, HIGH);
    digitalWrite(coil2, LOW);
    delay(100);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, LOW);
    delay(900);
    stoggle = true;

  }
  else
  {
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, HIGH);
    delay(100);
    digitalWrite(coil1, LOW);
    digitalWrite(coil2, LOW);
    delay(900);

    stoggle = false;

  }


}



