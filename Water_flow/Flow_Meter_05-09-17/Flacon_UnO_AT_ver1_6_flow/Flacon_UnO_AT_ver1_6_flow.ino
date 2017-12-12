//#include <SoftwareSerial.h>

//SoftwareSerial Serial1(10, 11); // RX, TX

unsigned long count = 0;
String devID = "2";
String jsn;
int FlowFrequency;
unsigned long litres_long,Sum=0,units;
String Litre_Str;
String lit_total;
String units_str;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(2400);
  Serial.println("Setup_End");
}

void loop() 
{
 flowCal();
falconClient(jsn);
 
}

void parseCmd(String command)
{
  //Serial.print("Command: ");
  //Serial.println(command);
  Serial1.println(command);
  delay(100);
  //Serial.print("Response: ");
  while(Serial1.available()>0)
  {
    Serial.print(char(Serial1.read()));
  }
}

   void flowCal()
{ 
  Serial.println("FLOW CAl Begin");
   for(int i=1;i<=2;i++)
  {
    delay(100); 
  }
   FlowFrequency=200;
    litres_long=(FlowFrequency/2);
    Litre_Str=String(litres_long);
    Sum=Sum+litres_long;
    Litre_Str=String(litres_long);
    lit_total=String(Sum);
    units=(litres_long/1000);
    units_str=String(units);
    FlowFrequency=0;
    jsn = "AT+HTTPPARA=\"URL\",\"http://track.salieabs.com/json/AddTrackDetails.aspx?deviceid=";
    jsn = jsn + devID + "&analog0=" + Litre_Str+ "&analog1="+ lit_total +"&analog2="+ units_str +"&analog3=0&analog4=0&analog5=0&digital0=0&digital1=0&digital2=0&digital3=0&digital4=0&digital5=0&serial0=0&serial1=0&serial2=0&serial3=0&latitude=+1130.6059&longitude=+7806.0677&keypad=0&customstring=SCMC\"";
   Serial.println("FLOW CAl END");
} 

void falconClient(String json)
{
  
  parseCmd("AT"); //Check AT
  parseCmd("ATE1");
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
}
