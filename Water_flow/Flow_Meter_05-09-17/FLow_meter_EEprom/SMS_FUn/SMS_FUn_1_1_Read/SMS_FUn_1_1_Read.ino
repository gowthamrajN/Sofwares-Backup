


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
  parseCmd("AT"); //Check AT
  parseCmd("AT+CMGF=1\r"); 
  parseCmd("at+clip=1\r"); 
  parseCmd("at+cscs=\"GSM\"\r"); // configure sms as standard text messages
  parseCmd ("at+cnmi=1,2,0,0,0\r"); // show received sms and store in sim  
  
}

void loop()
{
  
 smsFunc();

  
}

void parseCmd(String command)
{
  
  Serial1.println(command);
  delay(200);
  //Serial.print("Response: ");
  while (Serial1.available() > 0)
  {
    //char ch=Serial1.read();
    Serial.print(char(Serial1.read()));
  
  }
    delay(1000);
}

void smsFunc()
{
 while(Serial1.available() > 0) {inchar = Serial1.read(); readString+=inchar ;delay(10);}
    Serial.print(readString);  /// Declare a string    " String readString; "
    for (int i=0; i<200; i++)
    {  
        if(readString.substring(i,i+2)=="ON")
        {  flag=1;
       
        // break;
        } 
      else if(readString.substring(i,i+3)=="OFF")
        { 
        flag=2;
         //break;
        } 
    } 
    
    
    if (flag==1)
    {
       digitalWrite(53,HIGH);
    }
    else if(flag==2)
    {
       digitalWrite(53,LOW);
    } 
  
  
  
}
