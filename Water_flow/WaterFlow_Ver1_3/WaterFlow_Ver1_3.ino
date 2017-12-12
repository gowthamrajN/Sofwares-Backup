#include "SIM900.h"          //Include library for SIM900 GSM modem
#include "inetGSM.h"        //Include library for GPRS
#include "sms.h"            //include library for SMS
#include "call.h"
#include <SoftwareSerial.h>
#include <DS1307.h>
#include<Wire.h>
#include <OneWire.h>




unsigned long FlowFrequency=0;
unsigned litres_Hour;
unsigned litres_Minute;
float litres;
String Litre_Str;
unsigned int litre_int;
String lit_total;
float Sum=0;

String Date;
String T_ime;
String DAT_M;
String MONTH;
String YEAR;
String Correct_DATE;

DS1307 rtc(SDA, SCL);
InetGSM inet;              //Instatnce of GPRS
SMSGSM sms;                //Instance of SMS
CallGSM notify;

boolean started = false; 

int devID = 1;
void setup() {
  
  Serial.begin(9600);
  Serial.println("Setup_begin");
  pinMode(2,INPUT);
  attachInterrupt(0, check, FALLING);
  rtc.begin();
  rtc.halt(false);
  Serial.println("Setup_End");

}

void loop() {
  startmodem();
  calcWaterflow(); 
  RTC();
  send2Falcon();
  
}
void startmodem(void)
{
  if (gsm.begin(2400))                      //Begin GSM modem at 2400 bits per second
  {
    Serial.println("READY");                //If modem responds, print ready and update 'started' variable.
    started = true;
  }
  else
  {
    Serial.println("IDLE");                //If modem does not respond, print "IDLE"
  }
}

void check()
{
  FlowFrequency++; 

}


void calcWaterflow()
{
    for(int i=1;i<=6;i++)
  {
  delay(60000); 
  }
  Serial.println("FlowFrequency :");
  Serial.println(FlowFrequency);
  litres_Hour= (FlowFrequency*60/7.5);
  //Serial.print(litres_Hour);
  litres_Minute=(litres_Hour/60);
  Serial.print(" , ");
  //Serial.println(litres_Minute);
   litres=FlowFrequency/(60*7.5);
   if(litres>0)
  {
   litres= litres+(litres/7); 
  
  }
  Serial.print("LITRES : ");
  Serial.println(litres);
  litre_int=litres;
 unsigned  int litre_dec=((litres*100)-(litre_int*100));
  Litre_Str=String(litre_int)+"."+String(litre_dec);
  Serial.println("Litre_Str:");
  Serial.println(Litre_Str);
  Sum=Sum+litres;
 unsigned int Sum_int=Sum;
 unsigned int Sum_dec=((Sum*100)-(Sum_int*100));
 lit_total=String(Sum_int)+"."+String(Sum_dec);
  Serial.println("Litre_Total:");
  Serial.println(lit_total);
 
  FlowFrequency=0;

}
void RTC()
{ Date =rtc.getDateStr();
  DAT_M= Date.substring(0, 2);
  MONTH= Date.substring(3, 5);
  YEAR = Date.substring(6, 10);
  Correct_DATE=DAT_M +"/"+MONTH +"/"+YEAR;

  Serial.println(Correct_DATE);
  T_ime =rtc.getTimeStr();
  Serial.println(T_ime);
  
}



void send2Falcon()
{
  int numdata = 0;
  char msg[10];

  if (started)
  {
    //GPRS attach, put in order APN, username and password.
    //If no needed auth let them blank.
    if (inet.attachGPRS("internet.wind", "", ""))
      Serial.println("ATT");
    else
      Serial.println("ERR");
    delay(1000);

    //Read IP address.
    gsm.SimpleWriteln("AT+CIFSR");
    delay(5000);
    //Read until serial buffer is empty.
    gsm.WhileSimpleRead();

    //Prepare the REST API string by adding PPS in analog0 value and valve status in digital0 value
    String jsn = "/json/AddTrackDetails.aspx?deviceid=";
    jsn = jsn + String(devID);
    jsn = jsn + "&datetime=";
    jsn = jsn + Correct_DATE;
    jsn = jsn + "%20";
    jsn = jsn + T_ime;
    jsn = jsn + "&analog0=0&analog1=0&analog2=0&analog3=0&analog4=0&analog5=0&digital0=0";
    jsn = jsn + "&digital1=0&digital2=0&digital3=0&digital4=0&digital5=0";
    jsn = jsn +"&serial0=";
    jsn = jsn + Litre_Str;
    jsn = jsn +"&serial1=";
    jsn = jsn + lit_total;
    jsn = jsn +"&serial2=0&serial3=0&latitude=+1130.6059&longitude=+7806.0677&keypad=0&customstring=SCMC";

   
    numdata = inet.httpGET("track.salieabs.com", 80, &jsn[0], msg, 10);

    //Print the results.
    Serial.print("JSON: ");
    Serial.println(jsn);
    Serial.println("size:");
    Serial.println(numdata);
    Serial.println("Data:");
    Serial.println(msg);
  }
}

