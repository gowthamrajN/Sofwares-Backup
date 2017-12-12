/*BLE CODE
 * 
 */
 
#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include<Wire.h>
#define Addr 0x39


char buf1 [100];
byte q;
volatile byte pos;
volatile boolean process_it;
#define Addr 0x39

unsigned char z[6]={0};
unsigned char buf[16] = {0};
unsigned char len = 0;
int j, i=0;
unsigned long ti=0;
unsigned long t[3]={};

boolean f=0;

int poleAA=4;          //watch terminals
int poleBB=5;

int proximity (void);

void setup()
{  
 Serial.begin(57600);
 
  ble_begin();
  
  pinMode(poleAA, OUTPUT);
  pinMode(poleBB, OUTPUT);

   ble_set_name("BLE clock");

    // Initialise I2C communication as MASTER
  Wire.begin();
  // Initialise Serial Communication, set baud rate = 9600
  Serial.begin(57600);
  
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
  Wire.write(0xFF);
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

//pole manipulation bitwise
#define ABhigh PORTD = PORTD | B00001100;
#define Ahigh PORTD = PORTD | B00000100;
#define Alow PORTD = PORTD & B11111011;
#define Bhigh PORTD = PORTD | B00001000;
#define Blow PORTD = PORTD & B11110111;
#define ABlow PORTD = PORTD & B00000011;
#define AlowBhigh PORTD = (PORTD & B11111011) | B00001000;
#define BlowAhigh PORTD = (PORTD & B11110111) | B00000100;
 

// main loop - wait for flag set in interrupt routine

void loop(void)
{ 
   if ( ble_connected() )
   {
  if(f==0) 
  {
    ble_write('E');
    ble_write('n');
    ble_write('t');
    ble_write('e');
    ble_write('r');
    ble_write(' ');
    ble_write('t');
    ble_write('i');
    ble_write('m');
    ble_write('e');
    ble_write(' ');
    ble_write('i');
    ble_write('n');
    ble_write(' ');
    ble_write('h');
    ble_write('h');
    ble_write('m');
    ble_write('m');
    ble_write('s');
    ble_write('s');
    f=1;
  }  
 }

 ble_do_events();
   
  if ( ble_available() )
  {
    i=0;
    while ( ble_available() )
      {   
      z[i]=ble_read();
      z[i]=z[i]-48;
      i++;
      }

    for(j=0;j<i;j++)
    {
       Serial.print(z[j]);
    }
   Serial.println();
    t[0]=(z[0]*10)+(z[1]*1);
    t[1]=(z[2]*10)+(z[3]*1);
    t[2]=(z[4]*10)+(z[5]*1);

    if(t[0]==12)
    {
      t[0]=0;
    }
    ti =((t[0]*1800)+ (t[1]*30) + (t[2]/2));

    Serial.println(ti);
sensor:  
    int p1=proximity();
    
     //for setting home position 
    //20hz Movement
        BlowAhigh
          delay(6);
        ABhigh
          delay(44);
 
        AlowBhigh
          delay(6);
        ABhigh
          delay(44);
    if(p1!=1) goto sensor;
     
delay(2000);
     for(int i=0;i<ti;i++) // 
     {
      Serial.println("for setting given time");
        //20hz Movement 
        BlowAhigh
          delay(6);
        ABhigh
          delay(44);
 
        AlowBhigh
          delay(6);
        ABhigh
          delay(44);
     }
    delay(500);
    
     normal:
                BlowAhigh
                  delay(5);
                ABhigh
                  delay(995);
 
               AlowBhigh
                  delay(5);
               ABhigh
                  delay(995);
  goto normal;
}
  
  if ( Serial.available() )
  {
    delay(5);
    
    while ( Serial.available() )
        ble_write( Serial.read() );
  }
  
  ble_do_events(); 
}

int proximity (void)
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
  int p = data[1] * 256 + data[0];

  // Output data to serial monitor
  Serial.print("Proximity of the device : ");
  Serial.println(p);
  //delay(1000);
  return p;
}

