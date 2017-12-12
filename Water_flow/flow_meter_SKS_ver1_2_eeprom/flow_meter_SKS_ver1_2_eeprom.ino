
/*
The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 6
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 4
 * LCD D7 pin to digital pin 3
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3) 
 */

// include the library code:
#include <LiquidCrystal.h>
#include "SIM900.h"          //Include library for SIM900 GSM modem
#include "inetGSM.h"        //Include library for GPRS
#include "sms.h"            //include library for SMS
#include "call.h"
#include <SoftwareSerial.h>
//#include <DS1307.h>
#include<Wire.h>
#include <OneWire.h>
#include <EEPROM.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 6, 5, 4, 3);

unsigned long FlowFrequency=0;
double litres;        //Serial 0 value in website
String Litre_Str;          //Litre string
unsigned long litres_long;
String lit_total;           //Sum value String
unsigned long Sum = 0;
char Litre_chr[40];

int flag=1;              //One time execution flag for manipulation  
int condition=0;           
String Date;
String T_ime;
String DAT_M;
String MONTH;
String YEAR;
String Correct_DATE;
//char Number[]="+918682846199";
unsigned long PreviousTime=0;
unsigned long CurrentTime=0;

//DS1307 rtc(SDA, SCL);
InetGSM inet;              //Instatnce of GPRS
SMSGSM sms;                //Instance of SMS
CallGSM notify;

boolean started = false; 

int devID = 12;
char addr[10]={'0','1','2','3','4','5','6','7','8','9'};

char Size;
char value[30];

char result_Bit_13,result_Bit_12,result_Bit_11,result_Bit_10,result_Bit_9,result_Bit_8,result_Bit_7,result_Bit_6,result_Bit_5,result_Bit_4,result_Bit_3,result_Bit_2,result_Bit_1,result_Bit_0;

void setup() 
{
    Serial.begin(9600);
    Serial.println("Setup_begin");
  // set up the LCD's number of columns and rows: 
   startmodem();
  lcd.begin(16, 2);
  // Clesar & Print a message to the LCD.
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("SALIEABS");
  lcd.setCursor(0, 1); 
  lcd.print("  Smart  Meter  ");
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(0, check, FALLING);
  pinMode(7,INPUT_PULLUP);
 // rtc.begin();
 // rtc.halt(false);
   Eepromread();
  Serial.println("Setup_End");
  //delay(10000);
}

void loop() 
{
  flowCal();
 // RTC();
  EepromWrite();
  showLCD(litres_long,Sum);
  print2Serial();
  CurrentTime=millis();
  if(CurrentTime-PreviousTime>=21600000)
  {
    startmodem();
    send2Falcon();
    PreviousTime=millis();
  }
}

void Eepromread()
{
    result_Bit_9 = EEPROM.read(addr[0]);
    result_Bit_8 = EEPROM.read(addr[1]);
    result_Bit_7 = EEPROM.read(addr[2]);
    result_Bit_6 = EEPROM.read(addr[3]);
    result_Bit_5 = EEPROM.read(addr[4]);
    result_Bit_4 = EEPROM.read(addr[5]);
    result_Bit_3 = EEPROM.read(addr[6]);
    result_Bit_2 = EEPROM.read(addr[7]);
    result_Bit_1 = EEPROM.read(addr[8]);
    result_Bit_0 = EEPROM.read(addr[9]); 
   char temp[30]={result_Bit_9,result_Bit_8,result_Bit_7,result_Bit_6,result_Bit_5,result_Bit_4,result_Bit_3,result_Bit_2,result_Bit_1,result_Bit_0};
  // Serial.println(temp);
   Sum =atol(temp);
   //Serial.println(Sum);
 
  
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

void flowCal()
{
  for(int i=1;i<=2;i++)
  {
    delay(70000); 
  }
  litres=FlowFrequency/(60*7.5);
  litres = litres/10;
  litres=round(litres);
  litres_long=litres;
  if(litres_long==1)
  {
    litres_long=2;
  }
  litres_long=litres_long*10;
  Sum=Sum+litres_long;
  Litre_Str=String(litres_long);
  lit_total=String(Sum);

  showLCD(litres_long,Sum);
  FlowFrequency = 0;
}

void EepromWrite()
{
  
   lit_total.toCharArray(value,11);
   //Serial.println(Litre_Str);
   //Serial.println(value);
 
 
   for (int i=0; i<11; i++)
  {
    
    EEPROM.write(addr[i], value[i]);
  }
  delay(1000);
}

void showLCD(unsigned long thisSample, unsigned long cumulative)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("This Slot:");
  lcd.print(thisSample);
  lcd.setCursor(0, 1);
  lcd.print("Meter:");
  lcd.print(cumulative);
}

void print2Serial()
{
 /* Serial.println(Correct_DATE);
  Serial.print('\t');
  Serial.print(T_ime);
  Serial.print('\t');*/
  Serial.print(FlowFrequency);
  Serial.print('\t');
  Serial.print(litres_long);
  Serial.print('\t');
  Serial.println(lit_total);
}



/*void RTC()
{ 
  Date =rtc.getDateStr();
  DAT_M= Date.substring(0, 2);
  MONTH= Date.substring(3, 5);
  YEAR = Date.substring(6, 10);
  Correct_DATE=DAT_M +"/"+MONTH +"/"+YEAR;

  //Serial.println(Correct_DATE);
  T_ime =rtc.getTimeStr();
  //Serial.println(T_ime);
}
*/


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
    /*jsn = jsn + "&datetime=";
    jsn = jsn + Correct_DATE;
    jsn = jsn + "%20";
    jsn = jsn + T_ime;*/
    jsn = jsn + "&analog0=";
    jsn = jsn + Litre_Str;
    jsn = jsn + "&analog1=";
    jsn = jsn + lit_total;
    jsn = jsn  +"&analog2=0&analog3=0&analog4=0&analog5=0&digital0=0";
    jsn = jsn + "&digital1=0&digital2=0&digital3=0&digital4=0&digital5=0";
    jsn = jsn +"&serial0=0";
    // jsn = jsn + Litre_Str;
    jsn = jsn +"&serial1=0";
    jsn = jsn +"&serial2=0&serial3=0&latitude=+11.6702&longitude=+78.1435&keypad=0&customstring=SCMC";

    numdata = inet.httpGET("track.salieabs.com", 80, &jsn[0], msg, 10);

    //Print the results.
    Serial.print("JSON: ");
    Serial.println(jsn);
    Serial.println("size:");
    Serial.println(numdata);
    Serial.println("Data:");
    //Serial.println(msg);
  }
}



