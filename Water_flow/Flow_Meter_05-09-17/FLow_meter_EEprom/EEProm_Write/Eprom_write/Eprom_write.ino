/*
*
Function Name    :EepromWrite
Purpose          :To know the Value present in the address 0 to 9

*/
#include <EEPROM.h>

String data="0";   //To change Value in EEPROM from Address 0 to 9
char value[30];     // To change String to Char
char addr[10]={'0','1','2','3','4','5','6','7','8','9'};



void setup() 
{
  
  Serial.begin(9600);

}

void loop()
{
  
  EepromWrite();
  
}

void EepromWrite()
{
  data.toCharArray(value,11);

  for (int i=0; i<11; i++)
  {
    EEPROM.write(addr[i], value[i]);
  }
  delay(1000);
}
