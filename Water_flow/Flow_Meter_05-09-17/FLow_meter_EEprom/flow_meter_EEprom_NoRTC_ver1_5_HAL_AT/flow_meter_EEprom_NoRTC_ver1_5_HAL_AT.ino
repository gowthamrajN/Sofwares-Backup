
#include <LiquidCrystal.h>
/*#include "SIM900.h"          //Include library for SIM900 GSM modem
#include "inetGSM.h"        //Include library for GPRS
#include "sms.h"            //include library for SMS
#include "call.h"*/
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
unsigned long Sum =15423;
char Litre_chr[40];
unsigned long units;
String units_str;

int flag=1;              //One time execution flag for manipulation  
int condition=0;           

//char Number[]="+918682846199";
unsigned long PreviousTime=0;
unsigned long CurrentTime=0;
unsigned long  sensorValue = 0;        // value read from the pot



boolean started = false; 

int devID = 4;
String jsn;
String eeprom_str;
LiquidCrystal lcd(28, 32, 42, 44, 46, 48);
unsigned long FlowFrequency=0;


int AdapterValue=0;
int BatteryValue=0;
String Power;
int flag;


void setup() 
{
 
  Serial.begin(9600);
  Serial1.begin(2400);
  delay(15000);
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
  Eepromread();
  pinMode(3,INPUT_PULLUP);
  attachInterrupt(1, check,FALLING);
  flag=true;
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
  if(CurrentTime-PreviousTime>=21600000)             //43200000 for half day  //86400000 for one day //21600000 for 6hrs  //3600000 for 1hr  //60000 for 1 min
  {
    falconClient(jsn);
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
   lit_EEPROM =atol(temp);
   //Sum=lit_EEPROM;
   
}



void flowCal()
{ 
     //delay(10000);
    Serial.println("FLOW CAl Begin");
    for(int i=1;i<=2;i++)
    {
      delay(10000);
    }

    litres_long=FlowFrequency;
    Litre_Str=String(litres_long);
    Sum=Sum+litres_long;  
    lit_EEPROM=Sum;
    eeprom_str=String(lit_EEPROM);
    Litre_Str=String(litres_long);
    lit_total=String(Sum);
    units=(Sum/1000);
    units_str=String(units);
    FlowFrequency=0;
    jsn = "AT+HTTPPARA=\"URL\",\"http://track.salieabs.com/json/AddTrackDetails.aspx?deviceid=";
    jsn = jsn + devID + "&analog0=" + lit_total + "&analog1="+ units_str +"&analog2=0&analog3=0&analog4=0&analog5=0&digital0=0&digital1=0&digital2=0&digital3=0&digital4=0&digital5=0&serial0=A&serial1=0&serial2=0&serial3=0&latitude=+1130.6059&longitude=+7806.0677&keypad=0&customstring=SCMC%20falcon%20message1\"";
    Serial.println("FLOW CAl END");
}  


void EepromWrite()
{
  eeprom_str.toCharArray(value,11);
   //Serial.println(Litre_Str);
   //Serial.println(value);
 for (int i=0; i<11; i++)
  {
    
    EEPROM.write(addr[i], value[i]);
    delay(1000);
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
void parseCmd(String command)
{
  //Serial.print("Command: ");
  //Serial.println(command);
  Serial1.println(command);
  delay(200);
  //Serial.print("Response: ");
  while(Serial1.available()>0)
  {
    Serial.print(char(Serial1.read()));
  }
}

void falconClient(String json)
{
  parseCmd("AT"); //Check AT
  parseCmd("ATE0");
  parseCmd("AT+SAPBR=3,1,\"ConType\",\"GPRS\"");  //Init GPRS
  parseCmd("AT+SAPBR=3,1,\"APN\",\"www\"");   //Init APN
  parseCmd("AT+SAPBR =1,1");    //ENable GPRS & Get IP Address
  parseCmd("AT+SAPBR=2,1");     //Check IP address
  //parseCmd("ATD9791848808;"); //dial a call
  parseCmd("AT+HTTPINIT");
  parseCmd("AT+HTTPPARA=\"CID\",1");
  parseCmd(json);
  //parseCmd("AT+HTTPPARA=\"URL\",\"http://track.salieabs.com/json/AddTrackDetails.aspx?deviceid=2&datetime=17/11/1990%2011:22:20&analog0=109&analog1=5&analog2=1&analog3=2&analog4=3&analog5=4&digital0=0&digital1=1&digital2=0&digital3=1&digital4=0&digital5=1&serial0=A&serial1=1&serial2=a&serial3=-&latitude=+1130.6059&longitude=+7806.0677&keypad=0&customstring=salieabs%20falcon%20message1\"");
  parseCmd("AT+HTTPACTION=0");
  parseCmd("AT+HTTPREAD");
  
  Serial.println("End of JSON Call\n\n");
  delay(5000);
}



