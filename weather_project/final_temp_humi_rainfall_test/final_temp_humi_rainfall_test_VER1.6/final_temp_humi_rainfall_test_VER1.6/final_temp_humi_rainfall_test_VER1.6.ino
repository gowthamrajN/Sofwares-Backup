#include "DHT.h"
#include <Crc16.h>
 
#define DHTPIN 5              // pin we're connected to
#define DHTTYPE DHT22         // DHT 22  (AM2302)
# define IR 2                 // Receive the data from sensor
DHT dht(DHTPIN, DHTTYPE);
Crc16 crc; 

int value;       //map function variable
char command[20];    //serial monitor command
int h;
int t;
int dataFrame[10];





const float pi = 3.14159265;        //wind
int period = 10000;                 //wind 
int radio = 11;                     //wind 

// Variable definitions
unsigned int Sample = 0;       // Sample number
unsigned int counter = 0;      // B/W counter for sensor 
unsigned int RPM = 0;          // Revolutions per minute
int speedwind = 0;           // Wind speed (m/s)
int speedwindKm=0;           //Wind speed (km/h)
//rain.c

int nRainIn = A1;                 
int nRainDigitalIn = 7;          
int nRainVal;      
int nRainVal_8b;             
boolean bIsRaining = false;     
int strRaining;
int potentiometer;    //value of potentiometer
//rain.c

unsigned long currentTime;
unsigned long previousTime;

void setup()
{
  pinMode(13,OUTPUT);
  Serial.begin(9600);  
  dht.begin();
  pinMode(2, INPUT);            //wind_input
  pinMode(7,INPUT);  //rain.c

 }

void loop()
{
//------DHT22------//
   
   
   currentTime=millis();

  h = dht.readHumidity();

  t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    Serial.println("000");
    return;
 }
//====rain.c======//
  
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
  potentiometer=analogRead(A3);
  value=map(potentiometer,0,1023,1,60);
  if(Serial.available()>0)
  {
    char command=Serial.read();
    if(command=='ABC')
    {
      display_data();command=0;
    }
  }
  if(currentTime-previousTime>=value*10000)  
  {
    display_data();
    previousTime = currentTime;
  }
    
  //-------windSensor------//
  
  windvelocity();
  RPMcalc();
  WindSpeed();
//-------windSensor------//
  
//---rain.c---//



}    
 

// Measure wind speed
  void windvelocity(){
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

void RPMcalc()
{
 RPM=counter*6; // Calculate revolutions per minute (RPM)
 
}

void WindSpeed()
{
 speedwind  = ((2 * pi * radio * RPM)/60) / 1000;   // Calculate wind speed on m/s

 speedwindKm = speedwind*3.6;

}

void addcount(){
 counter++;
}


void display_data()
  {
   
  digitalWrite(13,HIGH);
  Serial.print("~");  
  Serial.print("01");
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
   Serial.print("/");
    Serial.print("  ");
  
  displayData();
  crcProcess();
  digitalWrite(13,LOW);
  
}
void   displayData()
{ int i;
   dataFrame[0]='~';
  dataFrame[1]=01;
  dataFrame[2]=h;
   dataFrame[3]=t;
   dataFrame[4]=strRaining;
   dataFrame[5]=nRainVal_8b;
   dataFrame[6]= counter ;
  dataFrame[7]= RPM ;
   dataFrame[8]=speedwind ;
     dataFrame[9]=speedwindKm ;
   dataFrame[10]='/' ;
for(i=0;i<11;i++)
  {  

   Serial.print(dataFrame[i]);
     
  }
  Serial.println();
}
void crcProcess()
{
  crc.clearCrc();
  for(byte i=0;i<11;i++)
  {  

   crc.updateCrc(dataFrame[i]);
     
  }

   unsigned short value = crc.getCrc();
  Serial.print("crc = 0x");
  Serial.println(value, HEX);
  

  
  //Reference crc
 
  value = calcrc((char*)dataFrame, 11);
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


