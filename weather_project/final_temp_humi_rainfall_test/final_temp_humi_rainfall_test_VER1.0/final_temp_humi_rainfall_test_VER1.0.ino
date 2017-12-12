#include <dht.h>

dht DHT;

#define DHT11_PIN 5
int humidity;
int temperature;


//rain.c

int nRainIn = A1;                 
int nRainDigitalIn = 2;          
int nRainVal;                   
boolean bIsRaining = false;     
int strRaining;


//rain.c


void setup()
{
        Serial.begin(9600);
         pinMode(2,INPUT);            //rain.c
 
}

void loop()
{
  // READ DATA
 // Serial.print("DHT11, \t");
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
  // DISPLAY DATA
        Serial.print("");
        Serial.print(humidity);
        Serial.print(",");
        Serial.print("");
        Serial.print(temperature);

 
  
  //rain.c
  
    nRainVal = analogRead(nRainIn);                 
  bIsRaining = !(digitalRead(nRainDigitalIn));         
  if(bIsRaining)
  {  
    
    strRaining =1;                                      
  }
  else{                                                        
    strRaining =0;
  }
 

  
     Serial.print(",");
     Serial.print("");
     Serial.print(strRaining);  
     Serial.print(",");  
     Serial.print("");
     Serial.println(nRainVal);

  
  
  delay(1000);

  //rain.c
}
