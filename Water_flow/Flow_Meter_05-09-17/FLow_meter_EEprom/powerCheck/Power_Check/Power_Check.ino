void setup() 
{
  Serial.begin(9600);

}

void loop() 
{
  int Voltage=(analogRead(A0));
  Serial.println(Voltage);
  if (Voltage>680)
  {
    Serial.println("Only Adapter");
  }
  else if ((600<Voltage) && (Voltage<=650))
  {
    Serial.println("Adapter + Battery(Full)");
  }
    else if (Voltage<600 && Voltage>100)
  {
    Serial.println("Only Battery(Empty) and Adapter");
  }
  else
  {
   Serial.println("Condition Fails"); 
  }
  
  delay(1000);  
 
  
}
