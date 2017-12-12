#include "DHT.h"
#include <Crc16.h>
 
#define DHTPIN 5                            // pin we're connected to
#define DHTTYPE DHT22                      // DHT 22  (AM2302)
# define IR 2                             // Receive the data from sensor
DHT dht(DHTPIN, DHTTYPE);
Crc16 crc; 

int dev_id=31;
unsigned long value;                 //map function variable
char command[20];                     //serial monitor command
int h;                               //value of humidity
int t;                              //value of temperature  
int dataFrame[10];
const float pi = 3.14159265;      //wind
int period = 10000;              //windSensor delay 
int radio = 11;                 //windSensor wingRadius 

// Variable definitions
unsigned int Sample = 0;       // Sample number
unsigned int counter = 0;     // B/W counter for sensor 
unsigned int RPM = 0;        // Revolutions per minute
int speedwind = 0;          // Wind speed (m/s)
int speedwindKm=0;         //Wind speed (km/h)
//rain.c

int nRainIn = A1;                  //RainSensor AnalogInput pin          
int nRainDigitalIn = 7;           //RainSensor DigitalOutput pin
int nRainVal;                    //values of Raindrops in 10Bit 
int nRainVal_8b;                //values of Raindrops in 10Bit 
boolean bIsRaining = false;    //Making bIsRaining as Zero
int strRaining;               //Value shows rain or not (i.e)0-->no Rain 1--->Rain
unsigned int potentiometer;            //value of potentiometer
//rain.c

unsigned long now = 0;
unsigned long then = 0;
boolean flag = true;

void setup()
{
  pinMode(13,OUTPUT);
  Serial.begin(9600);  
  dht.begin(); 
  pinMode(2, INPUT);          //wind_input pin
  pinMode(7,INPUT);           //rain_digital read pin
  delay(3000);
 }

void loop()
{
/*--------------------------DHT22-------------------------------------------------------------------
  Sensor detect values ,Send it to h(Humditity),t(Temperature).If Sensor Fails it gives output "000"
  
-------------------------------------------------------------------------------------------------*/
                                                                                                 
if(flag == true)
{
  flag=false;
  then=millis();
}


  potentiometer=analogRead(A3);
  value=map(potentiometer,0,1023,1,60);
  
  h = dht.readHumidity();

  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("000");
    return;
 }
/*--------------------------------------------RainSensor-----------------------------------
 Sensor reads Value from AnalogPin(A1) gives to nRainVal ,nRainVal_8b gives 8bit_data,Here 
 174 is thershold value above this strRaining is 1 ,below this strRaining is 0,When you 
 want to push data Immeditely give command ABC in Serial Monitor.
-----------------------------------------------------------------------------------------*/ 
  
  nRainVal    =analogRead(nRainIn); 
  nRainVal_8b =255-(map(nRainVal,0,1023,0,255));
  bIsRaining  = !(digitalRead(nRainDigitalIn));         
  if(bIsRaining&&nRainVal_8b>=174)
  {  
    strRaining =1;                                      
  }
  else
  {                                                        
    strRaining =0;
  }

   /*--------------------------------------windSensor-------------------------------*/
  

  
  if(Serial.available()>0)
  {
    char command=Serial.read();
    if(command=='A')
    {
     windvelocity();
     RPMcalc();
     WindSpeed();
     display_data();command=0;
    }
  }
  now=millis();
//  Serial.print(then);
//  Serial.print('\t');
//  Serial.print(now);
//  Serial.print('\t');
//  Serial.print(now-then);
//  Serial.print('\t');
//  Serial.println(value*10000);
  
  if(now-then >= (value*10000)-10000)  
  {
     windvelocity();
     RPMcalc();
     WindSpeed();
    display_data();
    
    flag=true;
  }
 
 }  
  /*---------------Windvelocity()--------------------------------------------
  This function gives attach interrupt and calls addcount()to measure wind for 10sec.
  ------------------------------------------------------------------------*/
  void windvelocity()
  {
    speedwind = 0;
    counter = 0;  
  
   attachInterrupt(0, addcount, CHANGE);
   //unsigned long millis();                     
   long startTime = millis();
   while(millis() < startTime + period) 
   {
   }
   digitalWrite(13, LOW);
   detachInterrupt(1);
}
/*---------------------RPMcalc()---------------------------
 To calculate Rotation per Minute using counter value
 ------------------------------------------------------*/
void RPMcalc()
{
 RPM=counter*6; // Calculate revolutions per minute (RPM)
 
}
/*------------------WindSpeed()----------------------------
 Calculate WindSpeed in m/s and Km/h
 ----------------------------------------------------------*/
void WindSpeed()
{
 speedwind  = ((2 * pi * radio * RPM)/60) / 1000;        // Calculate wind speed on m/s

 speedwindKm = speedwind*3.6;                           //Calculate wind speed on Km/h

}
/*-----------------Interrupt addcount()----------------
 Interrupt occurs and counter value increments.
 ----------------------------------------------------*/
void addcount(){
 counter++;
}
  

/*------------------------display_data()--------------------------------------------------------------
 this function gives output of Humidity,Temperature,Rain,RainValue,Counter,RPM,SpeedWind in m/s,SpeedWind in Km/h
                       and calls displayData(),crcProcess()  
  
------------------------------------------------------------------------------------------------------- */

void display_data()
  {
  
  digitalWrite(13,HIGH);
  Serial.print("~");  
  Serial.print(dev_id);
  Serial.print(",");
  Serial.print(value);
  Serial.print(",");
  Serial.print(h);
  Serial.print(",");
  Serial.print("");
  Serial.print(t);
  Serial.print(",");
  Serial.print("");
  Serial.print(strRaining);  
  Serial.print(",");  
  Serial.print("");
  Serial.print(nRainVal_8b);
  Serial.print(","); 
  Serial.print("");
  Serial.print(counter);
  Serial.print(",");
  Serial.print("");
  Serial.print(RPM);
  Serial.print(",");
  Serial.print("");
  Serial.print(speedwind);
  Serial.print(",");
  Serial.print("");
  Serial.print(speedwindKm);
  Serial.print(",");
  Serial.print("");
  displayData();         //summing of all values into array
  crcProcess();          //crc process

}
/*--------------------------DisplayData()--------------------
 this function Grouping of value into An array for CRC process
 -------------------------------------------------------------*/
void   displayData()
{ 
  int i;

  dataFrame[0]=dev_id;
  dataFrame[1]=value;
  dataFrame[2]=h;
  dataFrame[3]=t;
  dataFrame[4]=strRaining;
  dataFrame[5]=nRainVal_8b;
  dataFrame[6]= counter ;
  dataFrame[7]= RPM ;
  dataFrame[8]=speedwind ;
  dataFrame[9]=speedwindKm ;
}
/*-------------------crcProcess()-----------------------
 For error free Transistion ,Detect error and processing
 ------------------------------------------------------*/
void crcProcess()
{
  crc.clearCrc();
  for(byte i=0;i<10;i++)
  {  

   crc.updateCrc(dataFrame[i]);
     
  }

  unsigned short value = crc.getCrc();
  value = calcrc((char*)dataFrame, 10);
  Serial.print(value, HEX);
  Serial.println("/");
  digitalWrite(13,LOW);
  
}

int calcrc(char *ptr, int count)
{
    int  crc;
    char i;
    crc = 0;
    while (--count >= 0)
    {
        crc = crc ^ (int) *ptr++ << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while(--i);
    }
    return (crc);
}


