
#define coil1 6
#define coil2 5
#define coil3 3
#define coil4 2
void setup() {
  Serial.begin(9600);
  
  pinMode (coil1, OUTPUT);        //COIL 1 , 2 MEANT FOR SECONDS HAND
  pinMode (coil2, OUTPUT);

  pinMode (coil3, OUTPUT);
  pinMode (coil4, OUTPUT);    //COIL 3 , 4  MEANT FOR MINUTES HAND

}

void loop() {
  
   for (int i = 1; i <= 90; i++)
   {
     digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
    delay(10);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(5);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, HIGH);
    delay(10);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delay(5);
   }
  delay(3000);
}
