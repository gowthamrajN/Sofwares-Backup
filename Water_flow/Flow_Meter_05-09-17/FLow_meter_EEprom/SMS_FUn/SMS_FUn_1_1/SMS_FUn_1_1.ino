


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
  parseCmd(AT+CGREG?);
  parseCmd(AT+CMGF=1);
  parseCmd(AT+CLIP=1);
  parseCmd(AT+CMGD=1);
  parseCmd(AT+CMGS="+917904925631");
  parseCmd("Hello");
  Serial1.write((char)26);
 
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
