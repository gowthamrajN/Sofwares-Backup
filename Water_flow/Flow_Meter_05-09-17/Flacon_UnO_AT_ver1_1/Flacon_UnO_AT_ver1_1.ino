#include <SoftwareSerial.h>

SoftwareSerial gsm(10, 11); // RX, TX

unsigned long count = 0;
String devID = "2";

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  gsm.begin(2400);
  Serial.println("Setup_End");
}

void loop() 
{
  // put your main code here, to run repeatedly:
 // count++;
  //String val = String(count);
  String jsn = "AT+HTTPPARA=\"URL\",\"http://track.salieabs.com/json/AddTrackDetails.aspx?deviceid=";
  jsn = jsn + devID + "&analog0=600&analog1=5&analog2=1&analog3=2&analog4=3&analog5=4&digital0=0&digital1=1&digital2=0&digital3=1&digital4=0&digital5=1&serial0=A&serial1=1&serial2=a&serial3=-&latitude=+1130.6059&longitude=+7806.0677&keypad=0&customstring=salieabs%20falcon%20message1\"";
  Serial.print("Value= ");
  //Serial.println(val);
  
  parseCmd("AT"); //Check AT
  parseCmd("ATE0");
  parseCmd("AT+SAPBR=3,1,\"ConType\",\"GPRS\"");  //Init GPRS
  parseCmd("AT+SAPBR=3,1,\"APN\",\"www\"");   //Init APN
  parseCmd("AT+SAPBR =1,1");    //ENable GPRS & Get IP Address
  parseCmd("AT+SAPBR=2,1");     //Check IP address
  //parseCmd("ATD9791848808;"); //dial a call
  parseCmd("AT+HTTPINIT");
  parseCmd("AT+HTTPPARA=\"CID\",1");
  parseCmd(jsn);
  //parseCmd("AT+HTTPPARA=\"URL\",\"http://track.salieabs.com/json/AddTrackDetails.aspx?deviceid=2&datetime=17/11/1990%2011:22:20&analog0=109&analog1=5&analog2=1&analog3=2&analog4=3&analog5=4&digital0=0&digital1=1&digital2=0&digital3=1&digital4=0&digital5=1&serial0=A&serial1=1&serial2=a&serial3=-&latitude=+1130.6059&longitude=+7806.0677&keypad=0&customstring=salieabs%20falcon%20message1\"");
  parseCmd("AT+HTTPACTION=0");
  parseCmd("AT+HTTPREAD");
  Serial.println("End of Loop\n\n");
  delay(30000);
}

void parseCmd(String command)
{
  //Serial.print("Command: ");
  //Serial.println(command);
  gsm.println(command);
  delay(100);
  //Serial.print("Response: ");
  while(gsm.available()>0)
  {
    Serial.print(char(gsm.read()));
  }
}
