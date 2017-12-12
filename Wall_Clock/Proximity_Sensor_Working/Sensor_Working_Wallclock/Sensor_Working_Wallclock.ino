#include <Wire.h>

#define Addr 0x39
#define coil1 6
#define coil2 5
bool Condition = true;


void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);
  pinMode (coil1,OUTPUT);
  pinMode (coil2,OUTPUT);
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select enable register
  Wire.write(0x00 | 0x80);
  // Set power on, proximity and wait enabled
  Wire.write(0x0D);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(500);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select proximity time control register
  Wire.write(0x02 | 0x80);
  // time = 2.73 ms
  Wire.write(0x0F);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select proximity time register
  Wire.write(0x0F | 0x80);
  // Proximity uses Ch0 diode
  Wire.write(0x20);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop()
{
  unsigned int data[0];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x18 | 0x80);
  // Stop I2C Transmission
  Wire.endTransmission();
  
  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);
  
  // Read 2 bytes of data
  // proximity lsb, proximity msb
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  int proximity = data[1] * 256 + data[0];

  // Output data to serial monitor
  Serial.print("Proximity of the device : ");
  Serial.println(proximity);
  delay(60);
  
  
 /*if (proximity>=3)
  {
  

    digitalWrite(coil1,HIGH);
    digitalWrite(coil2,LOW);
    delay(25);
    
    digitalWrite(coil1,LOW);
    digitalWrite(coil2,LOW);

    digitalWrite(coil1,LOW);
    digitalWrite(coil2,HIGH);
    
    delay(25);
    
    digitalWrite(coil1,LOW);
    digitalWrite(coil2,LOW);
    Serial.println("IF in");
    
  
 }
 
 
 else if(proximity<=2)
  {
    digitalWrite(coil1,LOW);
    digitalWrite(coil2,LOW);
 
    
     Serial.println(" ELSE IF in");
  
  }*/
}
