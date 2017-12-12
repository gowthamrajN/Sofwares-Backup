#include <Wire.h>

#define TMD            0x39

#define R_ENABLE       0x00
#define R_PTIME        0x02
#define R_WTIME        0x03
#define R_PILTL        0x08
#define R_PILTH        0x09
#define R_PIHTL        0x0A
#define R_PIHTH        0x0B
#define R_PERS         0x0C
#define R_CONFIG       0x0D
#define R_PPULSE       0x0E
#define R_CONTROL      0x0F
#define R_REVISION     0x11
#define R_ID           0x12
#define R_STATUS       0x13
#define R_PDATAL       0x18
#define R_PDATAH       0x19
#define R_OFFSET       0x1E
#define PULSECOUNT      1



void setup()
{
  Serial.begin(9600);
  Wire.begin();
  configure_sensor();
  //attachInterrupt(6, int6, LOW); // Int6 = Digital Pin 7, active low
  delay(3000);
}

void loop()
{
  unsigned int data[2];
  Wire.beginTransmission(TMD);   
  Wire.requestFrom(TMD, 2);
 /* if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();

    // Convert the data
    int proximity = data[1] * 256 + data[0];
    //int proximity=sensor_read(TMD);
    Serial.print("Proximity of the device : ");
    Serial.println(proximity);
  }
  Wire.endTransmission();*/

  data[0] = sensor_read(0x18); 
  data[1] = sensor_read(0x19); 

  // Convert the data
  int proximity = data[1] * 256 + data[0];
  //int proximity=sensor_read(TMD);
  Serial.print("Proximity of the device : ");
  Serial.println(proximity);
 Wire.endTransmission();
  delay(1000);

}


/*---------------------------------------------------------------------------------*/

// configure sensor
void configure_sensor()
{
  // set Proximity Time Control Register to 2.73ms
  sensor_write(R_PTIME, 0xFF);

  // set Wait Time Register to 200ms
  sensor_write(R_WTIME, 0xB6);

  // set bit for WLONG in Configuration Register. Cycle: 200ms * 12 = 2,4s
  sensor_write(R_CONFIG, 0x02);

  // set Proximity Pulse Count
  sensor_write(R_PPULSE, 0x01);

  // set Control Register to I(LED)=100mA, CH1
  sensor_write(R_CONTROL, 0xE0);

  // interrupt generated every cycle
  sensor_write(R_PERS, 0x00);

  //set Enable Register to Wait Enable, Proximity Enable, Power ON, Interrup enable
  sensor_write(R_ENABLE, 0x0D);
}

// reading from sensor
uint8_t sensor_read(uint8_t reg)
{
  Wire.beginTransmission(TMD);
  Wire.write(reg);
  //Wire.endTransmission();
  //Wire.requestFrom(TMD, 2);
  while(!Wire.available());
 // if(2 == Wire.available())
  {
    return(Wire.read());
  }   
}


void sensor_write(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(TMD);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}


