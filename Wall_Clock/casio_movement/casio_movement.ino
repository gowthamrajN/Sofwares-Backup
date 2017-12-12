#include <SoftwareSerial.h>

#define ON_TIME 70
#define OFF_TIME 300

#define hw false
#define sw true

bool toggle = false;
SoftwareSerial bt(10, 11);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  bt.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  Serial.println("All set....");
  bt.println("All set....");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0)
    processSerial(hw);
  else if (bt.available() > 0)
    processSerial(sw);
  else
  {
    sweepOneSec(70);  //sweepOneSec(ON_Time);
    
    delay(1000-ON_Time);
  }
}

void processSerial(bool source)
{
  String inString;
  if (source == hw)
    inString = Serial.readString();
  else if (source == sw)
    inString = bt.readString();

  String ch = inString.substring(0, 2);
  String cm = inString.substring(2, 4);
  String dh = inString.substring(4, 6);
  String dm = inString.substring(6, 8);
  int curHours = ch.toInt();
  int curMins = cm.toInt();
  int destHours = dh.toInt();
  int destMins = dm.toInt();
  long difference = ((destHours - curHours) * 3600) + ((destMins - curMins) * 60);
  
  if (source == hw)
  {
    Serial.print("Current Time:- ");
    Serial.print(curHours);
    Serial.print(":");
    Serial.println(curMins);
    Serial.print("Setting Time To:- ");
    Serial.print(destHours);
    Serial.print(":");
    Serial.println(destMins);
  }
  else
  {
    bt.print("Current Time:- ");
    bt.print(curHours);
    bt.print(":");
    bt.println(curMins);
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
      sweepOneSec(50);
      delayMicroseconds(700);
    }
  }
  else if (difference < 0)
  {
    Serial.println("Travel Back...");
    for (int i = 0; i < abs(difference); i++)
    {
      sweepOneSec(30);
      //delay(10);
    }
  }
}

void sweepOneSec(int onTime)
{
  if (toggle == false)
  {
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }
  else
  {
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  }
  delay(onTime);
  toggle = !toggle;
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
}


