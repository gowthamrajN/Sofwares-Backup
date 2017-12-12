# define IR 2                 // Receive the data from sensor
# define Led 13               // Light a led

// Constants definitions
const float pi = 3.14159265;  // pi number
int period = 10000;           // Measurement period (miliseconds)
int delaytime = 10000;        // Time between samples (miliseconds)
int radio = 20;               // Radio from vertical anemometer axis to a cup center (mm)

// Variable definitions
unsigned int Sample = 0;       // Sample number
unsigned int counter = 0;      // B/W counter for sensor 
unsigned int RPM = 0;          // Revolutions per minute
float speedwind = 0;           // Wind speed (m/s)
float speedwindKm=0;           //Wind speed (km/h)
unsigned short windforce = 0;  // Beaufort Wind Force Scale


void setup()
{

 pinMode(2, INPUT);
 digitalWrite(2, HIGH);
 pinMode(13, OUTPUT);
 Serial.begin(9600);
 }

void loop() 
{

 windvelocity();

 Serial.print("Counter: ");
 Serial.print(counter);
 Serial.print(";RPM: ");
 RPMcalc();
 Serial.print(RPM);
 Serial.print(";Wind speed(m/s): ");
 WindSpeed();
 Serial.print(speedwind);
 Serial.print(";Wind speed(km/h): ");
 Serial.print(speedwindKm);
 Serial.println();
 delay(10000);
}

// Measure wind speed
void windvelocity(){
 speedwind = 0;
 counter = 0;  
 digitalWrite(Led, HIGH);
 attachInterrupt(0, addcount, CHANGE);
 unsigned long millis();                     
 long startTime = millis();
 while(millis() < startTime + period) {
 }
 digitalWrite(Led, LOW);
 detachInterrupt(1);
}

void RPMcalc(){
 RPM=((counter/2)*60)/(period/1000);  // Calculate revolutions per minute (RPM)
}

void WindSpeed(){
 speedwind  = ((2 * pi * radio * RPM)/60) / 1000;   // Calculate wind speed on m/s
 speedwindKm = speedwind*3.6;
}

void addcount(){
 counter++;
}
