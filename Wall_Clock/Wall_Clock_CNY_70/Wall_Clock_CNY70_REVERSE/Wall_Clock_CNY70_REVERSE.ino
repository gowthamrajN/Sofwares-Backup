#define base 200
#define coil1 6
#define coil2 5
#define coil3 2
#define coil4 3


void setup() {
  Serial.begin(9600);
  pinMode (coil1, OUTPUT);
 pinMode (coil2, OUTPUT);
  pinMode (coil3, OUTPUT);
pinMode (coil4, OUTPUT);



}

void loop()
{


/*Serial.println("SUCESS");
  digitalWrite(coil1, HIGH);
  digitalWrite(coil2, LOW);

  delay(80);
  digitalWrite(coil1, LOW);
  digitalWrite(coil2, LOW);

 delay(200);
  digitalWrite(coil1, LOW);
  digitalWrite(coil2, HIGH);

  delay(80);
  digitalWrite(coil1, LOW);
  digitalWrite(coil2, LOW);
  delay(200);*/


 Serial.println("SUCESS");
  digitalWrite(coil3, HIGH);
  digitalWrite(coil4, LOW);

  delay(15);
  digitalWrite(coil3, LOW);
  digitalWrite(coil4, LOW);

 delay(13);
  digitalWrite(coil3, LOW);
  digitalWrite(coil4, HIGH);

  delay(15);
  digitalWrite(coil3, LOW);
  digitalWrite(coil4, LOW);
delay(13);


}
