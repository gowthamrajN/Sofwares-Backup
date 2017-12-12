


String jsn;
char inchar; //Will hold the incoming character from the serial port.
//SoftwareSerial Serial1(2,3); //Create a 'fake' serial port. Pin 2 is the Rx pin, pin 3 is the Tx pin.
String readString;
 int led1 = 13;
 int flag=0;
void setup()
{
  Serial.begin(9600);
  pinMode(53, OUTPUT);
   digitalWrite(53, LOW);
  Serial.println("Setup_Begin");
  Serial1.begin(2400);
  delay(10000);
  Serial.println("Setup_End");
  jsn="hello salieabs";
  //smsFunc(jsn);

}

void loop()
{
  parseCmd("AT+CMGR=");
   parseCmd("1");

  
}

void parseCmd(String command)
{
  
  Serial1.println(command);
  delay(200);
  //Serial.print("Response: ");
  while (Serial1.available() > 0)
  {
    inchar = Serial1.read();
    readString+=inchar ;
    delay(100);
    Serial.print(readString);
  
  }
    delay(1000);
}

void Read_Sms()
{
  parseCmd("AT"); //Check AT
  parseCmd("AT+CMGF=1\r"); 
  parseCmd("at+clip=1\r"); 
  parseCmd("at+cscs=\"GSM\"\r"); // configure sms as standard text messages
  parseCmd ("at+cnmi=1,2,0,0,0\r"); // show received sms and store in sim  
  parseCmd("AT+CMGL=\"REC UNREAD\"");  
}


