




unsigned long FlowFrequency=0;
double litres;        //Serial 0 value in website
String Litre_Str;          //Litre string
unsigned long litres_int=0;
unsigned long sensorValue=0;
char flag=0;



void setup() 
{
     Serial.println("Setup_begin");
  // set up the LCD's number of columns and rows: 


  Serial.begin(9600);
  Serial.println("Setup_End");
 
}

void loop() 
{


  flowCal();
 print2Serial();

}


void flowCal()
{
  sensorValue = analogRead(A0);
  if (sensorValue <=750)
  {
    flag++;
      if(flag==1)
        {
          litres_int=litres_int+1;
          Litre_Str=String(litres_int);
       }
    
  }
 else
 {
   flag=0;
 }
 
}





void print2Serial()
{

  Serial.print(litres_int);
  
  Serial.print(",");
  Serial.println(sensorValue);
  //delay(500);
}








