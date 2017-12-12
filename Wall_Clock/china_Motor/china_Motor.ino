
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
  
    digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
    delayMicroseconds(900);
    digitalWrite(coil3, LOW);
    digitalWrite(coil4, LOW);
    delayMicroseconds(400);
    digitalWrite(coil3, HIGH);
    digitalWrite(coil4, LOW);
    delayMicroseconds(900);
    digitalWrite(coil3,LOW);
    digitalWrite(coil4, LOW);
    delayMicroseconds(400);
     digitalWrite(coil3,HIGH);
    digitalWrite(coil4, LOW);
    delayMicroseconds(900);
     digitalWrite(coil3,LOW);
    digitalWrite(coil4,HIGH );
    delayMicroseconds(400); 
     digitalWrite(coil3,LOW);
    digitalWrite(coil4,LOW );
    delayMicroseconds(900);
     digitalWrite(coil3,LOW);
    digitalWrite(coil4,HIGH );
    delayMicroseconds(400);
      digitalWrite(coil3,LOW);
    digitalWrite(coil4,LOW );
    delayMicroseconds(900); 
      digitalWrite(coil3,LOW);
    digitalWrite(coil4, HIGH );
    delayMicroseconds(400);  
}
    
    
    
