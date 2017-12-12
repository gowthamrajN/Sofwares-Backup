#include <EEPROM.h>

char result_Bit_13,result_Bit_12,result_Bit_11,result_Bit_10,result_Bit_9,result_Bit_8,result_Bit_7,result_Bit_6,result_Bit_5,result_Bit_4,result_Bit_3,result_Bit_2,result_Bit_1,result_Bit_0;
char addr[10]={'0','1','2','3','4','5','6','7','8','9'};
String result;
unsigned long lit_EEPROM;

double litres;        //Serial 0 value in website
String Litre_Str;          //Litre string
unsigned long litres_long=0;
String lit_total;           //Sum value String
//double Sum;                  //Sum of litres
unsigned long Sum = 100;
char Litre_chr[40];
unsigned long units;
String units_str;
void setup() {
 Serial.begin(9600);
   Eepromread();

}

void loop() {
  // put your main code here, to run repeatedly: 
  
}

void Eepromread()
{
    result_Bit_9 = EEPROM.read(addr[0]);
    result_Bit_8 = EEPROM.read(addr[1]);
    result_Bit_7 = EEPROM.read(addr[2]);
    result_Bit_6 = EEPROM.read(addr[3]);
    result_Bit_5 = EEPROM.read(addr[4]);
    result_Bit_4 = EEPROM.read(addr[5]);
    result_Bit_3 = EEPROM.read(addr[6]);
    result_Bit_2 = EEPROM.read(addr[7]);
    result_Bit_1 = EEPROM.read(addr[8]);
    result_Bit_0 = EEPROM.read(addr[9]); 
   char temp[30]={result_Bit_9,result_Bit_8,result_Bit_7,result_Bit_6,result_Bit_5,result_Bit_4,result_Bit_3,result_Bit_2,result_Bit_1,result_Bit_0};
  // Serial.println(temp);
   Sum =atol(temp);
   Serial.println(Sum);
   //result=String(litres_long);
   //Serial.println(result);
  
  
}
