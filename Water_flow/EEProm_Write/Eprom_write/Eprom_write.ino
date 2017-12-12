
#include <EEPROM.h>
String lit_total="12";
char value[30];

char addr[10]={'0','1','2','3','4','5','6','7','8','9'};
void setup() 
{
  
  Serial.begin(9600);

}

void loop() {
EepromWrite();
  
}

void EepromWrite()
{
  lit_total.toCharArray(value,11);

 for (int i=0; i<11; i++)
  {
    
    EEPROM.write(addr[i], value[i]);
  }
  delay(1000);
}
