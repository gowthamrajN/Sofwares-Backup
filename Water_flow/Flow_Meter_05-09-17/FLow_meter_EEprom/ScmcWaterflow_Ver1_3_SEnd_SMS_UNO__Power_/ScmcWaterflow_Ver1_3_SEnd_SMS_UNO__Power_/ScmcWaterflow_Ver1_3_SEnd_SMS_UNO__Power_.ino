


/*

 Project Name: Salem Corporation Smart Water Meter
 Filename:  ScmcWaterflow.ino
 Functions: setup(),loop(), Eepromread(),check(),flowCal(), EepromWrite(), print2Serial(),showLCD(Unsigned long,Unsigned long),falconClient(String)
 Global Variables:  Litre_Str, litres_long, lit_total, Sum, units, units_str,previousTime,currentTime,devID,jsn,FlowFrequency;
 
 */


#include <LiquidCrystal.h>   //Include library for LCD
#include <SoftwareSerial.h>  //Include library Software Serial for Uno
#include <EEPROM.h>          // Include library for EEPROM 
#include <String.h>          //Include library for String
SoftwareSerial gsm(3, 4); // RX, TX

char addr[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}; //To Store 10 Bit Address for 10 Value Storage
char value[30];        // To change String to Char for eeprom Write
char result_Bit_13, result_Bit_12, result_Bit_11, result_Bit_10, result_Bit_9, result_Bit_8, result_Bit_7, result_Bit_6, result_Bit_5, result_Bit_4, result_Bit_3, result_Bit_2, result_Bit_1, result_Bit_0;
// To Store Each Address from 0 to 9

String Litre_Str;            //Litre string  to Website
unsigned long litres_long = 0; //To Store Pulse from Interrupt
String lit_total;           //Sum value String
unsigned long Sum ;         // To Add Value Frequently after Interrupt
unsigned long units;       //To Store value (total_litres/1000)
String units_str;          //for String Conversion
unsigned long previousTime = 0; // Millis Calculation
unsigned long currentTime = 0;
unsigned long tmr_20s = 0, tmr_ping = 0; // Millis Calculation
//unsigned long devID = 1000000003;     //CC - Two Digit City Code,ZZ - Two Digit Zone Code,WW - Two Digit Ward Code,DDDD - 4 digit device ID
unsigned long devID = 1000000033;
String jsn;       // To Store Url for data Push
unsigned long FlowFrequency = 0; // Store Pulse Count from UniPolar Sensor
char increment=0;
char decrement=0;
char addinglong=0;
int Subractlong=0;
unsigned long modify=0;
String json;
String Warning;
int Voltage;
LiquidCrystal lcd(5, 6, 7, 8, 9, 10);    //digital pins for LCD Function

/*
  Function Name:  ISR
 Input: NIL
 Output: NIL
 Logic: Interrupt Service Routine that will get executed whenever flow meter gives a Falling edge
 */
void interrupt()
{
  FlowFrequency++;
}


void setup()
{
  //delay(300000);   //delay 5 Min
   Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);         //Interrupt for Waterflow
  pinMode(13, INPUT_PULLUP);          //Reset all values
  pinMode(12,INPUT_PULLUP);          // Subracting 10
  pinMode(11,INPUT_PULLUP);          //Adding 10
  delay(3000);
  int State= digitalRead(13);
  Serial.println(State);
  if(State==0)
  {
    Reset();
    Serial.println("Reseted Sucessfully");
  }

 
  gsm.begin(9600);
  delay(20000);
  Serial.println("Setup_begin");
  /*pinMode(22, OUTPUT);  //VSS
  digitalWrite(22, LOW);
  pinMode(24, OUTPUT);  //Vcc
  digitalWrite(24, HIGH);
  //pinMode(26,OUTPUT);   //Vee
  //digitalWrite(26,LOW);
  pinMode(30, OUTPUT);  //R/W
  digitalWrite(30, LOW);
  pinMode(34, OUTPUT);  //D0
  digitalWrite(34, LOW);
  pinMode(36, OUTPUT);  //D1
  digitalWrite(36, LOW);
  pinMode(38, OUTPUT);  //D2
  digitalWrite(38, LOW);
  pinMode(40, OUTPUT);  //D3
  digitalWrite(40, LOW);
  pinMode(50, OUTPUT);  //LED+
  digitalWrite(50, HIGH);
  pinMode(52, OUTPUT);  //LED-
  digitalWrite(52, LOW);*/
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("SALIEABS");
  lcd.setCursor(0, 1);
  lcd.print("  SMART  METER  ");
  Eepromread();
  attachInterrupt(0, interrupt, FALLING);
  Send_Sms();
  delay(1000);
  Serial.println("Setup_End");

}
void Send_Sms()
{
  parseCmd("AT"); //Check AT
  parseCmd("AT+CMGD=1");    // Delete the sms
  parseCmd("AT+CMGF=1");    // Text Mode
  parseCmd("AT+CMGS=\"1925\"");
  delay(300);
  parseCmd("START");
  delay(500);
  gsm.write((char)26);
}

void loop()
{  
  Adding();
  subracting();
  modify=(addinglong+(Subractlong));
  addinglong=0;
  Subractlong=0;
  if(tmr_20s == 0 || currentTime - tmr_20s >= 10000)
  { 
    powerCheck();
    flowCal();
    EepromWrite();
    print2Serial();
    showLCD(Sum, units);
    tmr_20s = millis();
  }
  if(tmr_ping == 0 || currentTime - tmr_ping >= 12000)        //43200000 for half day  //86400000 for one day //21600000 for 6hrs  //3600000 for 1hr  //10800000 for 3 hors//7200000 for 2hrs//60000 for 1 min //600000 for 10mins
  {
    gsm.begin(9600);
    delay(15000);
    falconClient(jsn);
    waterSalieabs(json);
    tmr_ping = millis();
  }
  currentTime = millis();
}

void Adding()
{
  increment=digitalRead(11);
  Serial.println(increment);
  if(increment==0)
  {
    addinglong=10;
    Serial.println(addinglong);
  }
}

void subracting()
{
  decrement=digitalRead(12);

  if(decrement==0)
  {
    Subractlong=-10;
  }
}

/*
  Function Name:  Eepromread
 Input: NIL
 Output: NIL
 Logic: Read Address value from 0 to 9
 */
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
  char temp[30] = {result_Bit_9, result_Bit_8, result_Bit_7, result_Bit_6, result_Bit_5, result_Bit_4, result_Bit_3, result_Bit_2, result_Bit_1, result_Bit_0};
  Sum = atol(temp);
}

void Reset()
{
  for (int i = 0; i < 512; i++)
  {
    EEPROM.write(i,0);
  } 
  delay(1000);
}


/*
 Function Name:  flowCal
 Input: NIL
 Output: NIL
 Logic:The Function Calculates every 20 Seconds  Sum of Litres (Sum) and Units(1000/perUnit).
 */
void flowCal()
{
  Serial.println("FLOW CAl Begin");
  litres_long = FlowFrequency;
  Sum = Sum + litres_long+modify;
  modify=0;
  Litre_Str = String(litres_long);
  lit_total = String(Sum);
  units = (Sum / 1000);
  units_str = String(units);
  FlowFrequency = 0;
  increment =0;

  //Uncomment the following two lines to send to water.salieabs.com
    json = "AT+HTTPPARA=\"URL\",\"http://water.salieabs.com/Api/getDeviceData.aspx?deviceid=";
    json = json + devID + "&slotreading=" + units_str + "&meterreading=" + lit_total + "&warningcode="+ Warning +"" ;

  //Uncomment the following two lines to send to track.salieabs.com
  jsn = "AT+HTTPPARA=\"URL\",\"http://track.salieabs.com/json/AddTrackDetails.aspx?deviceid=130";
  jsn = jsn + "&analog0=" + lit_total + "&analog1="+ units_str +"&analog2=0&analog3=0&analog4=0&analog5=0&digital0=0&digital1=0&digital2=0&digital3=0&digital4=0&digital5=0&serial0=0&gsm=0&serial2=0&serial3=0&latitude=+1130.6059&longitude=+7806.0677&keypad=0&customstring=SCMC\"";

  Serial.println("FLOW CAl END");
}

/*

 Function Name:  EepromWrite
 Input: NIL
 Output: NIL
 Logic:The Function Writes Total Number of Litres Consumed in EEPROM from Address 0 to 10
 */
void EepromWrite()
{
  lit_total.toCharArray(value, 11);
  for (int i = 0; i < 11; i++)
  {
    EEPROM.write(addr[i], value[i]);
  }
  delay(1000);
}

/*
  Function Name:  showLCD
 Logic:The Function Displays Total Number of Litres Consumed in Row 0 and Units in Row 1
 */
void showLCD(unsigned long thisSample, unsigned long cumulative)
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
void powerCheck()
{
  Voltage=(analogRead(A1));
  Serial.println(Voltage);
  if (Voltage>680)
  {
    Warning="101";
    Serial.println("Only Adapter + full Battery");
  }
  else if (700<Voltage)
  {
    Warning="102";
    Serial.println("Adapter + Battery(Charging)");
  }
  /*  else if (Voltage<653 && Voltage>50)
  {
    Warning="103";
   // Serial.println("Only Battery(Empty) and Adapter");
  }*/
  else
  {
   Warning="00";
   Serial.println("Condition Fails"); 
  } 
}

/*
  Function Name:  print2Serial
 Logic:The Function Displays Total Number of Litres Consumed and Units in Serial Monitor
 */
void print2Serial()
{
  Serial.print(Sum);
  Serial.print(",");
  Serial.println(units);
  Serial.print(",");
  Serial.println(Warning);
 
}

/*
  Function Name:  parseCmd
 Logic:The Function Communicates With GSM for Pushing Data
 */
void parseCmd(String command)
{
  //Serial.print("Command: ");
  //Serial.println(command);
  gsm.println(command);
  delay(200);
  //Serial.print("Response: ");
  while (gsm.available() > 0)
  {
    Serial.print(char(gsm.read()));
  }
}

/*

 Function Name:  falconClient
 Input: NIL
 Output: NIL
 Logic:The Function fetches data to the Server
 */
void falconClient(String jsn)
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
  parseCmd(jsn);
  parseCmd("AT+HTTPACTION=0");
  parseCmd("AT+HTTPREAD");
  Serial.println("End of JSON Call\n\n");
  delay(5000);
}

void waterSalieabs(String json)
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
  parseCmd("AT+HTTPACTION=0");
  parseCmd("AT+HTTPREAD");
  Serial.println("End of JSON Call\n\n");
  delay(5000);
}




