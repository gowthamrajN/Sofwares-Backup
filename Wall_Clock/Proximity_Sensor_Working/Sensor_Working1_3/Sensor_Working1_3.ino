#include <Wire.h>

#define TMD            0x39

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600);
  
  // Start I2C Transmission
  Wire.beginTransmission(TMD);
  // Select enable register
  Wire.write(0x00 | 0x80);
  // Set power on, proximity and wait enabled
  Wire.write(0x0D);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(500);

  // Start I2C Transmission
  Wire.beginTransmission(TMD);
  // Select proximity time control register
  Wire.write(0x02 | 0x80);
  // time = 2.73 ms
  Wire.write(0xFF);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Start I2C Transmission
  Wire.beginTransmission(TMD);
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
  Wire.beginTransmission(TMD);
  // Select data register
  Wire.write(0x18 | 0x80);
  // Stop I2C Transmission
  Wire.endTransmission();
 
   
   Wire.requestFrom(TMD, 2);
    if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }

  // Convert the data
  int proximity = data[1] * 256 + data[0];

   
  //int proximity=sensor_read(TMD);
  Serial.print("Proximity of the device : ");
  Serial.println(proximity);
  delay(1000);
  
  
}

void sensor_write(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(TMD);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}


// reading from sensor
uint8_t sensor_read(uint8_t reg)
{
  Wire.beginTransmission(TMD);
  Wire.write(reg);
  //Wire.endTransmission();
  Wire.requestFrom(TMD, 2);
  while(!Wire.available());
 if(2 == Wire.available())
  {
    return(Wire.read());
  }    
}
