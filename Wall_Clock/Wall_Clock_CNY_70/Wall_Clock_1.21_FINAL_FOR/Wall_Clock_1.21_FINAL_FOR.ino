#include <SoftwareSerial.h>

#define hw false    //Serial Monitor condition
#define sw true    //Bluetooth Monitor

bool stoggle = false;       //condition for sec
bool toggle = false;
bool mtoggle = false;
bool Sectoggle = false;
int scounter = 0;
SoftwareSerial bt(10, 11);

#define base 200
#define coil1 6
#define coil2 5
#define coil3 3
#define coil4 2

int Minute_Hand = 0;            //one defines Minute_Hand
int Hour_Hand = 0;            //two defines Hour_Hand
bool Condition = true;       //Two Hands Working Condition
bool Sec_Condition = true;


unsigned long CurrentTime = 0;  //Rotation taken for 12 Set begin
unsigned long ExtraTime = 0;   //Rotation taken for 12 Set End

unsigned long RunTime = 0;    //Rotation taken for Exact time begin
unsigned long EndTime = 0;    //Rotation taken for Exact time End

  int md=0;
  int hd=0;
  int sd=0;


void setup() {

  Serial.begin(9600);
  bt.begin(9600);
  pinMode (coil1, OUTPUT);        //COIL 1 , 2 MEANT FOR SECONDS HAND
  pinMode (coil2, OUTPUT);

  pinMode (coil3, OUTPUT);
  pinMode (coil4, OUTPUT);    //COIL 3 , 4  MEANT FOR MINUTES HAND


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
  int difference;
  int difference_Seconds;
  if (source == hw)
    inString = Serial.readString();
  else if (source == sw)
    inString = bt.readString();

  String dh = inString.substring(0, 2);
  String dm = inString.substring(2, 4);
  String ds = inString.substring(4, 6);

  int destHours = dh.toInt();
  int destMins = dm.toInt();
  int destSec = ds.toInt();

 
if((destHours==12)&&(destMins<30))
{
  difference=((destHours-12)*360)+(destMins*6)+4140;
}
else if((destHours==12)&&(destMins>=30))
{
  difference=((destHours-12)*360)+((destMins-30)*6);
}

else 
{
 
  md=destMins-30;
  hd=destHours-0;
  difference=(hd*360)+(md*6);
  
}

  sd=destSec-2;
  Serial.print("sd");
  Serial.println(sd);
  difference_Seconds=sd;
  
  Serial.print("difference_Seconds");
  Serial.println(difference_Seconds);
/*  
  if(difference < 0)
  {
    difference = 4320 + difference;
  }*/
  
 if(difference_Seconds<0)
  {

  difference_Seconds = 60+ difference_Seconds;      // SECOND HAND PULSE DIFFERENCE
  }
  Serial.println("Zero Setting Started");
  bt.println("Connected with Clock");

  CurrentTime = millis();                        // to call mills to calculate start time

  /*------------This Condition makes HOUR AND MINUTE HAND 12 SET-----------------*/
  while (Condition)
  {
    Minute_Hand = analogRead(A0);
    Hour_Hand = analogRead(A1);

    if ((Minute_Hand >= 200 && Hour_Hand <= 200) && (Hour_Hand <= 130))
    {
      Condition = false;
      delay(500);
    }

    Serial.print(Minute_Hand);
    Serial.print(", ");
    Serial.println(Hour_Hand);

    digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
    delay(30);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(5);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, HIGH);
    delay(30);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(5);

  }


  Minute_Hour_Extension();   // THE FUNCTION TELLS MINUTE AND HOUR HAND TO 1:30

  /*--------------THIS CONDITION RUNS FOR SECOND HAND TO 12 SET-------*/
  while (Sec_Condition == true)
  {
    int Second_Hand = analogRead(A1);
    Serial.println(Second_Hand);
    if (Second_Hand >= 150)
    {
      Sec_Condition = false;
    }

      sweepSeconds(80);
      delay(200);
  }

      Sec_Condition = true;
      delay(300);
      Condition = true;

  /*-------------THIS CONDTION RUNS HOUR ,MINUTE AND SECOND HAND TO 12 --------*/
//  while (Condition)
//  {
//    Minute_Hand = analogRead(A0);
//    Hour_Hand = analogRead(A1);
//
//    if ((Minute_Hand >= 200 && Hour_Hand <= 200) && (Hour_Hand <= 105))
//    {
//      Condition = false;
//      //delay(250);
//    }
//
//    Serial.print(Minute_Hand);
//    Serial.print(", ");
//    Serial.println(Hour_Hand);
//
//    digitalWrite(coil3, HIGH);
//    digitalWrite(coil4, LOW);
//    delay(30);
//    digitalWrite(coil3, LOW);
//    digitalWrite(coil4, LOW);
//    delay(5);
//    digitalWrite(coil3, LOW);
//    digitalWrite(coil4, HIGH);
//    delay(30);
//    digitalWrite(coil3, LOW);
//    digitalWrite(coil4, LOW);
//    delay(5);
//
//
//  }

   ExtraTime = millis();                            //ROTATION COMPLETION OF 12 SET MINUTE ,HOUR AND SECOND HAND
   ExtraTime = ExtraTime - CurrentTime;
   ExtraTime = ExtraTime / 1000;                  //CHANGING VALUE TO SECONDS
//  Serial.println(ExtraTime);
  float ExtraTime_min = (float)ExtraTime / 60;   //CHANGING VALUE TO MINUTES
  //Serial.println(ExtraTime_min);
  ExtraTime_min = ExtraTime_min * 6;            //CONVERTING MINUTES TO PULSES
  Serial.println(ExtraTime_min);

  difference = difference + ExtraTime_min;      //THE VALUE WILL HAVE ADDITIONAL PULSE TAKEN FOR 12 12 SET

  // delay(500);
  Condition = true;

  if (source == hw)
  {

    Serial.print("Setting Time To:- ");
    Serial.print(destHours);
    Serial.print(":");
    Serial.print(destMins);
    Serial.print(":");
    Serial.println(destSec);
  }
  else
  {

    bt.print("Setting Time To:- ");
    bt.print(destHours);
    bt.print(":");
    bt.print(destMins);
    Serial.print(":");
    bt.print(destSec);
  }


  if (difference > 0)
  {
    Serial.println("Travel Ahead...");
    RunTime = millis();
    for (int i = 0; i < difference; i++)
    {
      sweepOneSec(30);
      delay(5);
    }
    EndTime = millis();
    EndTime = EndTime - RunTime;
    EndTime = EndTime / 1000;
    Serial.println(EndTime);
    float EndTime_min = (float)EndTime / 10;
    Serial.println(EndTime_min);

    for (int i = 0; i < EndTime_min; i++)
    {
      sweepOneSec(30);
      delay(5);
    }

    for (int i = 0; i < difference_Seconds; i++)
    {
      sweepSeconds(80);
      delay(200);
    }

  }


  /* else if (difference < 0)
    {
     Serial.println("Travel Back...");
     for (int i = 0; i < abs(difference); i++)
     {
       sweepOneSec(25);
       // delay(10);

     }
    }*/
}

/*----------FUNCTION MAKES MINUTE HAND TO DRIVE------------*/
void sweepOneSec(int onTime)
{
  if (toggle == false)
  {
    digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
  }
  else
  {
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, HIGH);
  }
  delay(onTime);
  toggle = !toggle;
  digitalWrite(coil3, LOW);
  digitalWrite(coil4, LOW);
}



/*--------------------THE FUNCTION MEANT FOR EVERY ONE SECOND TICKING----------------*/
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
      scounter++;
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
    scounter++;
  }

  if (scounter >= 10)
  {


    if (mtoggle == false)
    {
      digitalWrite(coil3, HIGH);
      digitalWrite(coil4, LOW);
      delay(50);
      digitalWrite(coil3, LOW);
      digitalWrite(coil4, LOW);
      delay(40);
      mtoggle = true;
      scounter = 0;
    }
    else
    {
      digitalWrite(coil3, LOW);
      digitalWrite(coil4, HIGH);
      delay(50);
      digitalWrite(coil3, LOW);
      digitalWrite(coil4, LOW);
      delay(40);

      mtoggle = false;
      scounter = 0;
    }

  }
}
/*-----------------FUNCTION TO RUN MINUTE AND HOUR HAND TO 1:30------------
   ONE HOUR 360 PULSES ,WE SETTING TIME TO 12:30 .SO 180 PULSE ,HERE WE GIVE TWO PULSE SO DIVIDE BY 2 ,90 PULSE,to get accurate reduce 3 pulse
*/
void  Minute_Hour_Extension()
{
  for (int i = 0; i <= 88; i++)
  {
    digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
    delay(30);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(5);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, HIGH);
    delay(30);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(5);
  }
}
/*-----------THE FUNCTION MEANT FOR SECOND HAND DRIVING TO 12 SET--------*/
void sweepSeconds(int onTime)
{
  if (Sectoggle == false)
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
  Sectoggle = !Sectoggle;
  digitalWrite(coil1, LOW);
  digitalWrite(coil2, LOW);
}

