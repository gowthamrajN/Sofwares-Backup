

unsigned long FlowFrequency=0;
 unsigned int litres_Hour;
 unsigned int litres_Minute;


void check()
{
 FlowFrequency++;   
}

void setup()
{
  Serial.begin(9600); 
pinMode(2,INPUT);
attachInterrupt(0, check, FALLING);  
  

}


void loop()
{
      
  calcWaterflow();          

    
}

void calcWaterflow()
{
  Serial.println(FlowFrequency);
  litres_Hour= (FlowFrequency*60/7.5);
  //Serial.print(litres_Hour);
  litres_Minute=(litres_Hour/60);
  Serial.print(" , ");
  //Serial.println(litres_Minute);
  float litres=FlowFrequency/(60*7.5);
   Serial.print("LITRES : ");
  Serial.println(litres);
  FlowFrequency=0;
  delay(60000); 
}



