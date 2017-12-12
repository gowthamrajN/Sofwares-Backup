#include "DHT.h"
 
#define DHTPIN 5         // pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

int value;       //map function variable
char command[20];    //serial monitor command
int h;
int t;
//rain.c

int nRainIn = A1;                 
int nRainDigitalIn = 2;          
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
  pinMode(2,INPUT);            //rain.c
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
  //nRainVal_8b = 255-nRainVal_8b;              
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


//delay(2000);
  //---rain.c---//
}


void display_data()
  {

  digitalWrite(13,HIGH);
  Serial.print("");
  Serial.print(h);
  Serial.print(",");
  Serial.print("");
  Serial.print(t);

  Serial.print(",");
  Serial.print("");
  Serial.print(strRaining);  
  Serial.print(",");  
  Serial.print("");
  Serial.println(nRainVal_8b);
  digitalWrite(13,LOW);
  
}

