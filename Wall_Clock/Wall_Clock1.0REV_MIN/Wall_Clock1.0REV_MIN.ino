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


  Serial.println("SUCESS");
  digitalWrite(coil3, HIGH);
  digitalWrite(coil4, LOW);

  delay(25);
  digitalWrite(coil3, LOW);
  digitalWrite(coil4, LOW);
  delay(10);
  digitalWrite(coil3, LOW);
  digitalWrite(coil4, HIGH);
  delay(25);
  digitalWrite(coil3, LOW);
  digitalWrite(coil4, LOW);
  delay(10);





}
