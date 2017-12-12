#include <dht.h>

dht DHT;

#define DHT11_PIN 5


//rain.c

int nRainIn = A1;                 
int nRainDigitalIn = 2;          
int nRainVal;                   
boolean bIsRaining = false;     
String strRaining;

//rain.c


void setup()
{
  Serial.begin(9600);
   pinMode(2,INPUT);            //rain.c
  //Serial.println("DHT TEST PROGRAM ");
  //Serial.print("LIBRARY VERSION: ");
  //Serial.println(DHT_LIB_VERSION);
  Serial.println();
  //Serial.println("Type,\tstatus,\tHumidity (%),\tTemperature (C)");
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
  // DISPLAY DATA
  Serial.print("Humidity(%) ");
  Serial.print(DHT.humidity);
  Serial.print(",");
  Serial.print("Temperature (C)");
  Serial.print(DHT.temperature, 1);

  //delay(2000);
  
   //rain.c
  
    nRainVal = analogRead(nRainIn);                 
  bIsRaining = !(digitalRead(nRainDigitalIn));         
  if(bIsRaining){  
    
    strRaining = "YES";                                      
  }
  else{                                                        
    strRaining = "NO";
  }
  
  Serial.print(",Raining?: ");
  Serial.print(strRaining);  
  Serial.print(",");  
  Serial.print(" Moisture Level: ");
  Serial.println(nRainVal);
  
  delay(2000);

  //rain.c
}
