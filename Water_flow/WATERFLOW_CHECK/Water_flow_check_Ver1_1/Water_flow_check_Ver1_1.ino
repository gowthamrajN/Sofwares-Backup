//#include "SIM900.h"          //Include library for SIM900 GSM modem
//#include "inetGSM.h"        //Include library for GPRS
//#include "sms.h"            //include library for SMS
//#include "call.h"
//#include <SoftwareSerial.h>
//#include <DS1307.h>
//#include<Wire.h>
//#include <OneWire.h>




unsigned long FlowFrequency=0;

unsigned long  litres_int;
double litres;
String Litre_Str;
//unsigned long litres_int;
String lit_total;
unsigned long  Sum;
char Litre_chr[40];

String Date;
String T_ime;
String DAT_M;
String MONTH;
String YEAR;
String Correct_DATE;


boolean started = false; 

int devID = 1;
void setup() {
  
  Serial.begin(9600);
  Serial.println("Setup_begin");
  pinMode(2,INPUT);
  attachInterrupt(0, check, FALLING);

  Serial.println("Setup_End");

}

void loop() {
  //startmodem();
  calcWaterflow(); 
   delay(1000);
  
}

void check()
{
  FlowFrequency++; 

}

int secscounter=0;
void calcWaterflow()
{
  //secscounter=secscounter+1;
 /*   for(int i=1;i<=5;i++)
  {
  delay(60000); 
  }*/
  Serial.println("FlowFrequency :");
  Serial.println(FlowFrequency);
//litres=FlowFrequency/(7.5*60);
  litres=8.9;
     litres = litres/10;
    litres=ceil(litres);
     litres_int=litres;
     if(litres_int==1)
     {
       litres_int=2;
     }
  litres_int=litres_int*10;
  
    //Litre_Str=String(litres_int);
  Serial.print("LITRES : ");
  Serial.println(litres);
   Serial.print("LITRES int: ");
  Serial.println(litres_int);
 
 /* Serial.println("Litre_Str:");
  Serial.println(Litre_Str);*/
    Sum=Sum+litres_int;
  //floatToString(Litre_chr,Sum,2,1);
     FlowFrequency=0;

  lit_total=String(Sum);
  Serial.println("Litre_Total:");
  Serial.println(lit_total);
  //litres_int=0;
   // flag=0;
   Serial.println("**************5 Minute************************");
  






   
}







