#include <SoftwareSerial.h>
char inchar; //Will hold the incoming character from the serial port.
//SoftwareSerial Serial1(2,3); //Create a 'fake' serial port. Pin 2 is the Rx pin, pin 3 is the Tx pin.
String readString;
 int led1 = 13;
 int flag=0;

 void setup()
 {
     Serial.begin(9600);
     Serial.println("Setup_begin");
     // Prepare the digital output pins
     pinMode(53, OUTPUT);
     digitalWrite(53, LOW);

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
    while(Serial1.available() > 0) {inchar = Serial1.read(); readString+=inchar ;delay(10);}
    Serial.print(readString);  /// Declare a string    " String readString; "
    for (int i=0; i<200; i++)
    {  
        if(readString.substring(i,i+2)=="ON")
        {  flag=1;
       
        // break;
        } 
      else if(readString.substring(i,i+3)=="OFF")
        { 
        flag=2;
         //break;
        } 
    } 
    
    
    if (flag==1)
    {
       digitalWrite(53,HIGH);
    }
    else if(flag==2)
    {
       digitalWrite(53,LOW);
    }
 }
