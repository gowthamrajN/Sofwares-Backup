
int one=0;
int two=0;
void setup() {
Serial.begin(9600);



}

void loop()
{
one=analogRead(A0);
two=analogRead(A1);

Serial.print(one);
Serial.print(" ,");

Serial.println(two);
delay (1000);
  
       
      

  
}
