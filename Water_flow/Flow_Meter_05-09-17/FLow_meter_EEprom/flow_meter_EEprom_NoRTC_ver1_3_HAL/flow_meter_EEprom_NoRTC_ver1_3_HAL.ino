
#include <LiquidCrystal.h>
#include "SIM900.h"          //Include library for SIM900 GSM modem
#include "inetGSM.h"        //Include library for GPRS
#include "sms.h"            //include library for SMS
#include "call.h"
#include <SoftwareSerial.h>
#include<Wire.h>
#include <OneWire.h>
#include <EEPROM.h>
#include <String.h>
char addr[10]={'0','1','2','3','4','5','6','7','8','9'};


char Size;
char value[30];

char result_Bit_13,result_Bit_12,result_Bit_11,result_Bit_10,result_Bit_9,result_Bit_8,result_Bit_7,result_Bit_6,result_Bit_5,result_Bit_4,result_Bit_3,result_Bit_2,result_Bit_1,result_Bit_0;

String result;
unsigned long lit_EEPROM;

double litres;        //Serial 0 value in website
String Litre_Str;          //Litre string
unsigned long litres_long=0;
String lit_total;           //Sum value String
//double Sum;                  //Sum of litres
unsigned long Sum = 0;
char Litre_chr[40];
unsigned long units;
String units_str;

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
unsigned long  sensorValue = 0;        // value read from the pot

InetGSM inet;              //Instatnce of GPRS
SMSGSM sms;                //Instance of SMS
CallGSM notify;

boolean started = false; 

int devID = 4;

LiquidCrystal lcd(28, 32, 42, 44, 46, 48);
unsigned long FlowFrequency=0;


int AdapterValue=0;
int BatteryValue=0;
String Power;


void setup() 
{
 
  Serial.begin(9600);
  Serial.println("Setup_begin");
  pinMode(8, INPUT);             //Adapter pin reading
  pinMode(9, INPUT);            //Battery pin reading  
  pinMode(22,OUTPUT);   //VSS
  digitalWrite(22,LOW);
  pinMode(24,OUTPUT);   //Vcc
  digitalWrite(24,HIGH);
  pinMode(26,OUTPUT);   //Vee
  digitalWrite(26,LOW);
  pinMode(30,OUTPUT);   //R/W
  digitalWrite(30,LOW); 
  pinMode(34,OUTPUT);   //D0
  digitalWrite(34,LOW); 
  pinMode(36,OUTPUT);   //D1
  digitalWrite(36,LOW); 
  pinMode(38,OUTPUT);   //D2
  digitalWrite(38,LOW);
  pinMode(40,OUTPUT);   //D3
  digitalWrite(40,LOW);
  pinMode(50,OUTPUT);   //LED+
  digitalWrite(50,HIGH);
  pinMode(52,OUTPUT);   //LED-
  digitalWrite(52,LOW);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("SALIEABS");
  lcd.setCursor(0, 1); 
  lcd.print("  SMART  METER  ");
  startmodem();
  Eepromread();
  pinMode(3,INPUT_PULLUP);
  attachInterrupt(1, check,CHANGE);
  Serial.println("Setup_End");
  //delay(10000);
}

void check()
{
  FlowFrequency++; 
}

void loop() 
{
  flowCal();
  EepromWrite();
  //PowerReading();
  print2Serial();
  showLCD(Sum,units);
  CurrentTime=millis();
  if(CurrentTime-PreviousTime>=60000)             //43200000 for half day  //86400000 for one day 
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
   //Serial.println(litres_long);
   result=String(litres_long);
   //Serial.println(result);
  
  
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

void flowCal()
{
  Serial.println("FLOW CAl Begin");
   for(int i=1;i<=2;i++)
  {
    delay(10000); 
  }
    litres_long=(FlowFrequency/2);
    Litre_Str=String(litres_long);
    Sum=Sum+litres_long;
    Litre_Str=String(litres_long);
    lit_total=String(Sum);
    units=(litres_long/1000);
    units_str=String(units);
    FlowFrequency=0;
   Serial.println("FLOW CAl END");
}  



/*void flowCal()
{
  sensorValue = analogRead(A0);
  if (sensorValue <=900)
  {
    flag++;
      if(flag==1)
        {
          litres_long=litres_long+1;
          Litre_Str=String(litres_long);
          units=(litres_long/1000);
          units_str=String(units);
        
        }
    
  }
 else
 {
   flag=0;
   Litre_Str=String(litres_long);
   units_str=String(units);
 }
}
*/
void EepromWrite()
{
  
   lit_total.toCharArray(value,11);
   //Serial.println(Litre_Str);
   //Serial.println(value);
 
 
   for (int i=0; i<11; i++)
  {
    
    EEPROM.write(addr[i], value[i]);
  }
  
}



void showLCD(unsigned long thisSample,unsigned long cumulative)
{
  lcd.clear();
  lcd.setCursor(14, 0);
  lcd.print("lt");
  lcd.setCursor(0, 0);
  lcd.print(thisSample);
  lcd.setCursor(11, 1);
  lcd.print("Units");
  lcd.setCursor(0, 1);
  lcd.print(cumulative);
}


/*void PowerReading()
{
  
  AdapterValue = digitalRead(8);
  //BatteryValue = digitalRead(9);
  Power=String(AdapterValue);
  
}*/


void print2Serial()
{
  Serial.print(Sum);
  Serial.print(",");
  Serial.println(units);
  //Serial.println(lit_total);
}

void send2Falcon()
{ 
  int numdata = 0;
  char msg[10];

  if (started)
  {
    //GPRS attach, put in order APN, username and password.
    //If no needed auth let them blank.
    if (inet.attachGPRS("airtelgprs.com", "", ""))
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
    jsn = jsn + "&analog0=";
    jsn = jsn + lit_total;
    jsn = jsn + "&analog1=";
    jsn = jsn + units_str;
    jsn = jsn + "&analog2=0";
    //jsn = jsn + units_str;
    jsn = jsn+"&analog3=0&analog4=0&analog5=0&digital0=0";
    jsn = jsn + "&digital1=0&digital2=0&digital3=0&digital4=0&digital5=0";
    jsn = jsn +"&serial0=0";
    // jsn = jsn + Litre_Str;
    jsn = jsn +"&serial1=0";
    jsn = jsn +"&serial2=0&serial3=0&latitude=+11.6550853&longitude=+78.1531832&keypad=0&customstring=SCMC";

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



