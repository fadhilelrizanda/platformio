#include <Arduino.h>
#include<Wire.h>

#define BH1750_ADDRESS 0x23 //alamat
#define BH1750_DATALEN 2 //panjang data

void bh1750request(int address);
int bh1750read(int address);

byte buff[2];
unsigned short lux = 0 ;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
bh1750request(BH1750_ADDRESS);
delay(200);
if (bh1750read(BH1750_ADDRESS) == BH1750_DATALEN)
{
lux = (((unsigned short)buff[0] << 8) | (unsigned short)buff[1])/1.2;
String sentence = "Nilai Intensitas Cahaya" + String(lux) + "lux";
Serial.println(sentence);
}
delay(1000);

}

void bh1750request(int address){
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}

int bh1750read(int address)
{
  int i = 0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address,2);
  while(Wire.available())
  {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  return i;
}