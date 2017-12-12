#include <dht.h>

dht DHT;

#define DHT11_PIN 5
int humidity;
int temperature;
int value;       //map function variable
char command;   //serial monitor command

//rain.c

int nRainIn = A1;                 
int nRainDigitalIn = 2;          
int nRainVal;                   
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
  pinMode(2,INPUT);            //rain.c
}

void loop()
{
  // READ DATA
 // Serial.print("DHT11, \t");
 currentTime=millis();
 
  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
                Serial.print(""); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    case DHTLIB_ERROR_CONNECT:
              Serial.print("Connect error,\t");
              break;
    case DHTLIB_ERROR_ACK_L:
              Serial.print("Ack Low error,\t");
              break;
    case DHTLIB_ERROR_ACK_H:
              Serial.print("Ack High error,\t");
              break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }

  humidity=DHT.humidity;
  temperature=DHT.temperature;
  
  //rain.c
  
  nRainVal = 1023-analogRead(nRainIn);                 
  bIsRaining = !(digitalRead(nRainDigitalIn));         
  if(bIsRaining&&(nRainVal>=700))
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
    if(command=='A')
    {
      display_data();command=0;
    }
  }
  if(currentTime-previousTime>=value*6000)  
  {
    display_data();
    previousTime = currentTime;
  }

 //display_data();
 delay(5000);
  //rain.c
}


void display_data()
{
  digitalWrite(13,HIGH);
  Serial.print("");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print("");
  Serial.print(temperature);

  Serial.print(",");
  Serial.print("");
  Serial.print(strRaining);  
  Serial.print(",");  
  Serial.print("");
  Serial.println(nRainVal);
  digitalWrite(13,LOW);
  
}

