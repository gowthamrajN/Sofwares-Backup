



 
int sensorValue = 0;        // value read from the pot
int sensorValue_HOUR = 0; 


void setup() {

  Serial.begin(9600);
  //pinMode(7,INPUT);
}

void loop() {

  sensorValue      = analogRead(A0);
//sensorValue_HOUR = analogRead(A1);

//  sensorValue      = digitalRead(A0);
  
  Serial.print("sensor = ");
  Serial.println(sensorValue);

  // Serial.print(" ");
 // Serial.println(sensorValue_HOUR);
  
 delay(500);
}
