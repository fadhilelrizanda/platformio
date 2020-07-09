#include <Arduino.h>
#include <SPI.h>

#define CS_PIN 5
#define SCK_PIN 18
#define MISO_PIN 19
#define MOSI_PIN 23

#define LEN_ID 4

#define GET_CHIP_ID 0x9F

void chipInit();
void chipGetId();

byte chipId[4];

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
delay(3000);
chipInit();
chipGetId();

}

void loop() {
  // put your main code here, to run repeatedly:
}

void chipInit(){
 pinMode(CS_PIN, OUTPUT);
 SPI.begin(SCK_PIN,MISO_PIN,MOSI_PIN,CS_PIN);
 digitalWrite(CS_PIN, HIGH);
}

void chipGetId()
{
digitalWrite(CS_PIN,LOW);
SPI.transfer(GET_CHIP_ID);
//receive id
for (int i = 0; i <LEN_ID; i++)
{
chipId[i] = SPI.transfer(0);
}
digitalWrite(CS_PIN,HIGH);
Serial.print("Chip id: ");

for(int i=0;i<LEN_ID; i++)
{
  Serial.print(chipId[i],HEX);
}
Serial.println();
}