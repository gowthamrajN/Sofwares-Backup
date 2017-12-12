#include <SoftwareSerial.h>
char inchar; //Will hold the incoming character from the serial port.
//SoftwareSerial Serial1(2,3); //Create a 'fake' serial port. Pin 2 is the Rx pin, pin 3 is the Tx pin.
char readString;
 int led1 = 13;

 void setup()
 {
     Serial.begin(9600);
     Serial.println("Setup_begin");
     // Prepare the digital output pins
     pinMode(53, OUTPUT);
     digitalWrite(53, HIGH);

     //Initialize GSM module serial port for communication.
     Serial1.begin(2400);
     delay(10000); // Give time for GSM module to register on network, etc.
     Serial1.println("AT+CMGF=1"); // Set SMS mode to text
     delay(200);
     Serial1.println("AT+CNMI=2,2,0,0,0"); // Set module to send SMS data to serial out upon receipt 
     delay(200);
     Serial.println("Setup_end");
 }

 void loop() 
 {
    if(Serial1.available() >0)
     {
        delay(10);
        char ch = Serial1.read();
        Serial.print(ch);
         //inchar=cell.read(); 
        if (ch=='0')
             {
                 digitalWrite(53, LOW);
             } 
             else if (ch=='1')
             {
                 digitalWrite(53, HIGH);
             }
             /*delay(10);
             delay(10);*/
         }
         //Serial1.println("AT+CMGD=1,4"); // Delete all SMS
     
 }
