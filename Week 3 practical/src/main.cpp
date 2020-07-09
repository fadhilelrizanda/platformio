#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <w25q64.hpp>

#define csPin 5
#define sckPin 18
#define misoPin 19
#define mosiPin 23
#define lenId 4
#define getChipId 0x9F
byte chipId[4];
w25q64 spiChip;
unsigned char writepage[256] ="";
unsigned char readpage[256] ="";

#define BH1750Address 0x23
#define BH1750Len 2

void bh1750Req(int address);
int bh1750read(int address);

byte buff[2];
unsigned short lux= 0;
//define pin
#define LED1 2
#define LED2 4
#define LED3 16
#define LED4 17
#define button 15

int btnState;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  Serial.begin(9600);
  Wire.begin();
  spiChip.begin();
  spiChip.getId(chipId);
  delay(2000);
  for(int i = 0; i<lenId; i++)
  {
    Serial.print(chipId[i],HEX);
  }
  Serial.println();

  

}

void loop() {
spiChip.readPages(readpage, 0XFFFF,1);
  for (int i=0; i<256;i++)
  {
    if (readpage[i]> 8 && readpage[i]<127)
    {
      Serial.print((char) readpage[i]);
    }
  }
  Serial.println();

  btnState = digitalRead(button);
  // put your main code here, to run repeatedly:


bh1750Req(BH1750Address);
delay(300);
if (btnState == !1)
{
memcpy(writepage, "ON", sizeof("ON"));
spiChip.erasePageSector(0xFFFF);
spiChip.pageWrite(writepage,0XFFFF);
Serial.println("Writing is Done");

if (bh1750read(BH1750Address)== BH1750Len)
{
  lux = (((unsigned short)buff[0] << 8) | (unsigned short)buff[1])/1.2;
  Serial.println("LUX : " + String(lux));
}

if(lux >=0 && lux <= 250)
{
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,HIGH);

}
else if(lux >=251 && lux <= 500)
{
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,HIGH);
  digitalWrite(LED4,LOW);

}
else if(lux >=501 && lux <= 750)
{
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,HIGH);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);

}
else if(lux >=751 && lux <= 1000)
{
  digitalWrite(LED1,HIGH);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);

}
else{
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);

}

}
else{
  memcpy(writepage, "OFF", sizeof("OFF"));
spiChip.erasePageSector(0xFFFF);
spiChip.pageWrite(writepage,0XFFFF);
Serial.println("Writing is Done");
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);
}
delay(2000);
}





void bh1750Req(int address)
{
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
