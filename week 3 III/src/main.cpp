#include <Arduino.h>
#include <SPI.h>
#include <w25q64.hpp>

#define CS_PIN 5
#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

#define LEN_ID 4

#define GET_CHIP_ID 0x9F

byte chipId[4];

w25q64 spiChip;
unsigned char writepage[256] = "";
unsigned char readpage[256] = "";

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
spiChip.begin();
delay(3000);

spiChip.getId(chipId);
Serial.print("Chip Id");
for(int i = 0; i<LEN_ID;i++)
{
Serial.print(chipId[i],HEX);
}
Serial.println();

//test write data
/*
memcpy(writepage, "Nusantech Academy", sizeof("Nusantech Academy"));
spiChip.erasePageSector(0xFFFF);
spiChip.pageWrite(writepage,0XFFFF);
Serial.println("Writing is Done");
delay(1000);
*/
//test read data
Serial.println("Reading...");
spiChip.readPages(readpage,0XFFFF, 1) ;
for (int i = 0; i < 256; i++)
{
  if (readpage[i]>8 && readpage[i]<127)
  {
    Serial.print((char) readpage[i]);
  }

}
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
}

