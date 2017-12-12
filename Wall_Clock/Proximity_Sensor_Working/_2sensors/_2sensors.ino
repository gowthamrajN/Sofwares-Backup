#include <Wire.h>

// I2C address of the TMD26721 0x39(57)
#define Addr 0x39

int poleAA=5;          //watch terminals
int poleBB=4;

int hr_prox(void);
int min_prox(void);
void clock_fast(void);

void setup()
{
  // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(9600); 

  // hour hand sensor
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select enable register
  Wire.write(0x80);
  // Set power on, proximity and wait enabled
  Wire.write(0xd0);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select enable register
  Wire.write(0x82);
  // Set power on, proximity and wait enabled
  Wire.write(0xff);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);

  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select proximity time control register
  Wire.write(0x8f);
  // time = 2.73 ms
  Wire.write(0x20);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);

  pinMode(poleAA, OUTPUT);
  pinMode(poleBB, OUTPUT);
 
}

//pole manipulation bitwise
//#define ABhigh PORTD = PORTD | B00001100;
//#define Ahigh PORTD = PORTD | B00000100;
//#define Alow PORTD = PORTD & B11111011;
//#define Bhigh PORTD = PORTD | B00001000;
//#define Blow PORTD = PORTD & B11110111;
//#define ABlow PORTD = PORTD & B00000011;
//#define AlowBhigh PORTD = (PORTD & B11111011) | B00001000;
//#define BlowAhigh PORTD = (PORTD & B11110111) | B00000100;

void loop()
{
   
   int p = min_prox();
      
   digitalWrite(poleAA, HIGH);
   digitalWrite(poleBB, HIGH );
   // BlowAhigh
      delay(25);
                
     digitalWrite(poleAA, HIGH);
     digitalWrite(poleBB, LOW);
      //ABhigh
        delay(25);

     digitalWrite(poleAA, HIGH );
     digitalWrite(poleBB, HIGH);
      //AlowBhigh
        delay(25);
      
     digitalWrite(poleAA, LOW);
     digitalWrite(poleBB, HIGH);
      //ABhigh
        delay(25); 
}

  int min_prox(void)
  {
   //for minute sensor 
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
     delay(50);
     return proximity;
     
  }

  int hr_prox(void)
  {
    //sdl1 and scl1
     unsigned int data1[0];
      //unsigned int data1[0];
      // Start I2C Transmission
      Wire1.beginTransmission(Addr);
      // Select data register
      Wire1.write(0x18 | 0x80);
      // Stop I2C Transmission
      Wire1.endTransmission();
      
      // Request 2 bytes of data
      Wire1.requestFrom(Addr, 2);
      
      // Read 2 bytes of data
      // proximity lsb, proximity msb
      if(Wire1.available() == 2)
      {
        data1[0] = Wire1.read();
        data1[1] = Wire1.read();
      }
      
      // Convert the data
      int proximity1 = data1[1] * 256 + data1[0];

    return proximity1;
  }





