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
  send2Falcon();

}

void check()
{
  FlowFrequency++; 

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
    jsn = jsn + "&analog0=0&analog1=0&analog2=0&analog3=0&analog4=0&analog5=0&digital0=";
    jsn = jsn + "&digital1=0&digital2=0&digital3=0&digital4=0&digital5=0";
    jsn=  jsn+"&serial0=";
    jsn= jsn+Litre_Str;
    jsn=jsn+"&serial1=0&serial2=0&serial3=0&latitude=+1130.6059&longitude=+7806.0677&keypad=0&customstring=SCMC";

    //TCP Client GET, send a GET request to the server and
    //save the reply.
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
void calcWaterflow()
{
  Serial.println(FlowFrequency);
  litres_Hour= (FlowFrequency*60/7.5);
  //Serial.print(litres_Hour);
  litres_Minute=(litres_Hour/60);
  Serial.print(" , ");
  //Serial.println(litres_Minute);
   litres=FlowFrequency/(60*7.5);
   Serial.print("LITRES : ");
  Serial.println(litres);
  char litre_char[9];
  dtostrf(litres,8, 2, litre_char);
   litre_char[8]='\0';;
  Litre_Str=String(litre_char);
  Serial.println(Litre_Str);
  FlowFrequency=0;
  delay(60000); 
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


