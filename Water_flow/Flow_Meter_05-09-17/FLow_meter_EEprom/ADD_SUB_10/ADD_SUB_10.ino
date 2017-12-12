char increment=0;
char decrement=0;
char  addinglong=0;
int Subractlong=0;
unsigned long sum=0;

void setup() 
{
   Serial.begin(9600);
   pinMode(7,INPUT_PULLUP);
   pinMode(2,INPUT_PULLUP);
  
}

void subracting()
{
  decrement=digitalRead(7);
 
   if(decrement==0)
   {
    Subractlong=-10;
   }
}
void Adding()
{
  increment=digitalRead(2);
  
 if(increment==0)
  {
   addinglong=10;
 }
}

void loop()
{  delay(5000);
    Adding();
    subracting();
    sum=(sum+addinglong+(Subractlong));
    addinglong=0;
    Subractlong=0;
   Serial.println(sum);
}
