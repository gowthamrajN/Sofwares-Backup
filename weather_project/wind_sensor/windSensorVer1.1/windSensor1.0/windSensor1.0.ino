# define IR 2                 // Receive the data from sensor
# define Led 13               // Light a led

// Constants definitions
const float pi = 3.14159265;  // pi number
int period = 10000;           // Measurement period (miliseconds)
int delaytime = 10000;        // Time between samples (miliseconds)
int radio = 20;               // Radio from vertical anemometer axis to a cup center (mm)
char* winds[] = {"Calm", "Light air", "Light breeze", "Gentle breeze", "Moderate breeze", "Fresh breeze", "Strong breeze", "Moderate gale", "Fresh gale", "Strong gale", "Storm", "Violent storm", "Hurricane"};

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
 Serial.print(";  RPM: ");
 RPMcalc();
 Serial.print(RPM);
 Serial.print(";  Wind speed: ");
 WindSpeed();
 Serial.print(speedwind);
 Serial.print(";  Wind speed in km/p: ");
 Serial.print(speedwindKm);
 Serial.print(" ;  Wind force (Beaufort Scale): ");
 Serial.print(windforce);
 Serial.print(" - ");
 Serial.println(winds[windforce]);
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
 if (speedwind <= 0.3){                           // Calculate Wind force depending of wind velocity
   windforce = 0;  // Calm
 }
 else if (speedwind <= 1.5){
   windforce = 1;  // Light air
 }
 else if (speedwind <= 3.4){
   windforce = 2;  // Light breeze
 }
 else if (speedwind <= 5.4){
   windforce = 3;  // Gentle breeze
 }
 else if (speedwind <= 7.9){
   windforce = 4;  // Moderate breeze
 }
 else if (speedwind <= 10.7){
   windforce = 5;  // Fresh breeze
 }
 else if (speedwind <= 13.8){
   windforce = 6;  // Strong breeze
 }
 else  if (speedwind <= 17.1){
   windforce = 7;  // High wind, Moderate gale, Near gale
 }
 else if (speedwind <= 20.7){
   windforce = 8;  // Gale, Fresh gale
 }
 else if (speedwind <= 24.4){
   windforce = 9;  // Strong gale
 }
 else if (speedwind <= 28.4){
   windforce = 10;  // Storm, Whole gale
 }
 else if (speedwind <= 32.6){
   windforce = 11;  // Violent storm
 }
 else {
   windforce = 12;  // Hurricane (from thi point, apply the Fujita Scale)
 }
}

void addcount(){
 counter++;
}
