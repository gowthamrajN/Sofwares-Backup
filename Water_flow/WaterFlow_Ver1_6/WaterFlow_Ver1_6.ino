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
double litres=1441.63;
String Litre_Str;
unsigned int litre_int;
String lit_total;
double Sum=0;
char Litre_chr[40];

String Date;
String T_ime;
String DAT_M;
String MONTH;
String YEAR;
String Correct_DATE;
char Number[]="+917904925631";

DS1307 rtc(SDA, SCL);
InetGSM inet;              //Instatnce of GPRS
SMSGSM sms;                //Instance of SMS
CallGSM notify;

boolean started = false; 
int smsstatus = 0;      
char text[180];               
String message;
char Mesg[160];


char *p;
char *q;
char message_SMS[40];

int devID = 1;
void setup() {
  
  Serial.begin(9600);
  Serial.println("Setup_begin");
  pinMode(2,INPUT);
  attachInterrupt(0, check, FALLING);
  pinMode(7,INPUT_PULLUP);
  rtc.begin();
  rtc.halt(false);
    if(started ==  true)                     //If GSM modem started
  {
    for (int i = 1; i <= 10; i++)          //Delete 10 SMS from INBOX, considering it as unwanted
    {
      sms.DeleteSMS(i);
    }
  }
  Serial.println("Setup_End");

}

void loop() {
  startmodem();
  readSmsCommand();
  delay(10000);
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
    if (smsstatus==1)
   { 
  for(int i=1;i<=5;i++)
  {
  delay(60000); 
  }
   }
    else  if (smsstatus==2)
   { 
  for(int i=1;i<=10;i++)
  {
  delay(60000); 
  }
   }
   else if (smsstatus==3)
   { 
  for(int i=1;i<=20;i++)
  {
  delay(60000); 
  }
   }
   else   if (smsstatus==4)
   { 
  for(int i=1;i<=30;i++)
  {
  delay(60000); 
  }
   }
   else{
 for(int i=1;i<=2;i++)
  {
  delay(60000); 
  }
     
   }
   
  Serial.println("FlowFrequency :");
  Serial.println(FlowFrequency);
  litres_Hour= (FlowFrequency*60/7.5);
  //Serial.print(litres_Hour);
  litres_Minute=(litres_Hour/60);
  Serial.print(" , ");
  //Serial.println(litres_Minute);
  if(litres>0)
  {
  litres=FlowFrequency/(60*7.5);
  litres=litres+(litres*0.27);
  }
  else 
  {litres=FlowFrequency/(60*7.5);
  }
                                                  //USE UR CALIBIRATION
  floatToString(Litre_chr,litres,2,1);
  FlowFrequency=0;
  Litre_Str=String(Litre_chr);
  Serial.print("LITRES : ");
  Serial.println(litres);
 
  Serial.println("Litre_Str:");
  Serial.println(Litre_Str);
  Sum=Sum+litres;
  floatToString(Litre_chr,Sum,2,1);
 
  lit_total=String(Litre_chr);
  Serial.println("Litre_Total:");
  Serial.println(lit_total);
 
  /*if(digitalRead(7)==0)
   { 
       String message="SCMC WATER FLOW CALCULATION,LITRES IS :"+ Litre_Str +", TOTAL IS : "+ lit_total;
       char Mesg[160];
       message.toCharArray(Mesg,100);
       sms.SendSMS(Number,Mesg); 
   } */


}
char * floatToString(char * outstr, double val, byte precision, byte widthp){
 char temp[40]; //increase this if you need more digits than 15
 byte i;

 temp[0]='\0';
 outstr[0]='\0';

 if(val < 0.0){
   strcpy(outstr,"-\0");  //print "-" sign
   val *= -1;
 }

 if( precision == 0) {
   strcat(outstr, ltoa(round(val),temp,10));  //prints the int part
 }
 else {
   unsigned long frac, mult = 1;
   byte padding = precision-1;
   
   while (precision--)
     mult *= 10;

   val += 0.5/(float)mult;      // compute rounding factor
   
   strcat(outstr, ltoa(floor(val),temp,10));  //prints the integer part without rounding
   strcat(outstr, ".\0"); // print the decimal point

   frac = (val - floor(val)) * mult;

   unsigned long frac1 = frac;

   while(frac1 /= 10) 
     padding--;

   while(padding--) 
     strcat(outstr,"0\0");    // print padding zeros

   strcat(outstr,ltoa(frac,temp,10));  // print fraction part
 }

 // generate width space padding 
 if ((widthp != 0)&&(widthp >= strlen(outstr))){
   byte J=0;
   J = widthp - strlen(outstr);

   for (i=0; i< J; i++) {
     temp[i] = ' ';
   }

   temp[i++] = '\0';
   strcat(temp,outstr);
   strcpy(outstr,temp);
 }

 return outstr;
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




void readSmsCommand()
{
   Serial.println("Read Sms Command begin");
  
  char pos;
 
  char phoneNumber[14];      
  pos=sms.IsSMSPresent(SMS_UNREAD);
  if((int)pos)
  {
    Serial.print("Unread SMS Position: ");
    Serial.println((int)pos);
  }
  else
  {
    Serial.println("No New SMS");
  }
  
  if((int)pos>0&&(int)pos<=20)
  {
      text[0]='\0';
      sms.GetSMS((int)pos, phoneNumber, text, 180);   //Get the SMS string
      String RecdPhone = String(phoneNumber);
      String FromPhone = String(Number);
      Serial.print("Fixed Number: ");
      Serial.println(Number);
      Serial.print("Received Number: ");
      Serial.println(RecdPhone);
      Serial.print("Text Received: ");
      Serial.println(text);
      q=strstr(text,"updateNumber");
       if(q)
      {
        for(int i=0;i<14;i++)
        {
          Number[i] = phoneNumber[i];
        }
        Serial.print("New Fixed Number: ");
        Serial.println(Number);
        sms.SendSMS(Number,"Number Updated Successfully");
      }
      
      
    if(FromPhone.equals(RecdPhone))
    {
      p=strstr(text,"5Min");
      if(p)
      {
       Serial.println("delay for 5 minutes");
       smsstatus = 1;
        sms.SendSMS(Number,"delay for 5 minutes");
      }
       p=strstr(text,"10Min");
      if(p)
       {
       smsstatus = 2;
       Serial.println("delay for 10 minutes");
        sms.SendSMS(Number,"delay for 10 minutes");
       }
       p=strstr(text,"20Min");
      if(p)
       {
       smsstatus = 3;
       Serial.println("delay for 20 minutes");
        sms.SendSMS(Number,"delay for 20 minutes");
       }
       p=strstr(text,"30Min");
      if(p)
       {
       smsstatus = 4;
       Serial.println("delay for 30 minutes");
        sms.SendSMS(Number,"delay for 30 minutes");
       }
       
      
     sms.DeleteSMS((int)pos);
    }
    else
    {
      sms.DeleteSMS((int)pos);
      Serial.println("Wrong Number, SMS Discarded");
    }
  }
   Serial.println("Read Sms Command End");
}
