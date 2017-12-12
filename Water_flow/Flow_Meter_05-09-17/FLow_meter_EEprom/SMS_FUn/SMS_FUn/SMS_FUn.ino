


String jsn;
void setup()
{
  Serial.begin(9600);
  Serial.println("Setup_Begin");
  Serial1.begin(2400);
  delay(10000);
  Serial.println("Setup_End");
  jsn="hello";
  //smsFunc(jsn);
  parseCmd("AT"); //Check AT
  parseCmd("AT+CMGD=1");    // Delete the sms
  parseCmd("AT+CMGF=1");    // Text Mode
  parseCmd("AT+CMGS=\"7904925631\"");
   delay(1000);
  parseCmd("Hello");
  Serial1.println((char)26);
  
 // parseCmd();
}

void loop()
{
 //smsFunc();
  
}

void parseCmd(String command)
{
  
  Serial1.println(command);
  delay(200);
  //Serial.print("Response: ");
  while (Serial1.available() > 0)
  {
    Serial.print(char(Serial1.read()));
  }
}

void smsFunc(String json)
{
  parseCmd("AT"); //Check AT
  parseCmd("ATE0");
 
}
