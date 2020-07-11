#include <Arduino.h>
#include <EEPROM.h> //for EEPROM
#include <SPI.h>    // for Sensors
#include <Wire.h>   //for Sensors
#include <w25q64.hpp>

//* EEPROM Config
#define EEPROM_SIZE 128
char readData[32], receivedData[32], ssid[32], pwd[32];
int dataIndex = 0;

// parsing data
char *strings[128];
char *ptr = NULL;

//EEPROM Functions
void readEEPROM(int address, char *data);
void writeEEPROM(int address, char *data);

//*Light Sensors
#define bh1750Address 0x23
#define bh1750Len 2

//functions
void bh1750Req(int address);
int bh1750Read(int address);

// variable
byte buff[2];
unsigned short lux = 0;
w25q64 spiChip;
unsigned char writepage[256] = "";
unsigned char readpage[256] = "";

//* LED config
#define LED1 2
#define LED2 4
#define LED3 16
#define LED4 17
#define button 15
int btnState;

//* interrupt
volatile bool interruptState = false;
int totalInterruptCiunter = 0;
hw_timer_s *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE); // Initialize EEPROM
  delay(100);
  //Read EEPROM
  readEEPROM(0, readData);
  Serial.print("Autobrightness: ");
  Serial.println(readData);
  readEEPROM(32, ssid);
  Serial.print("SSID: ");
  Serial.println(ssid);
  readEEPROM(64, pwd);
  Serial.print("PWD: ");
  Serial.println(pwd);
  //Actuator Pin
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(button, INPUT_PULLUP);
}

void changeChar();

void loop()
{
  changeChar();
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
  Wire.requestFrom(address, 2);
  while (Wire.available())
  {
    buff[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  return i;
}

void readEEPROM(int address, char *data)
{
  for (int i = 0; i < 32; i++)
  {
    data[i] = EEPROM.read(address + i);
  }
}

void writeEEPROM(int address, char *data)
{
  Serial.println("Start Writing to EEPROM");
  for (int i = 0; i < 32; i++)
  {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.commit();
}

void changeChar()
{
  if (Serial.available())
  {
    receivedData[dataIndex] = Serial.read();
    dataIndex++;

    if (receivedData[dataIndex - 1] == '\n')
    {

      byte indexstrok = 0;
      ptr = strtok(receivedData, ";");
      while (ptr != NULL)
      {
        strings[indexstrok] = ptr;
        indexstrok++;
        ptr = strtok(NULL, ";");
      }
      Serial.println("String: " + String(strings[0]));
      Serial.println("String: " + String(strings[1]));
      dataIndex = 0;
      writeEEPROM(32, strings[0]);
      writeEEPROM(64, strings[1]);
      Serial.println("Success!");
    }
  }
}

void autoBrightness()
{
  if (btnState == !1)
  {
    memcpy(writepage, "ON", sizeof("ON"));
    spiChip.erasePageSector(0xFFFF);
    spiChip.pageWrite(writepage, 0XFFFF);
    Serial.println("Writing is Done");

    if (bh1750read(bh1750Address) == bh1750Len)
    {
      lux = (((unsigned short)buff[0] << 8) | (unsigned short)buff[1]) / 1.2;
      Serial.println("LUX : " + String(lux));
    }

    if (lux >= 0 && lux <= 250)
    {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
    }
    else if (lux >= 251 && lux <= 500)
    {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, LOW);
    }
    else if (lux >= 501 && lux <= 750)
    {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
    }
    else if (lux >= 751 && lux <= 1000)
    {
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
    }
    else
    {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      digitalWrite(LED4, LOW);
    }
  }
  else
  {
    memcpy(writepage, "OFF", sizeof("OFF"));
    spiChip.erasePageSector(0xFFFF);
    spiChip.pageWrite(writepage, 0XFFFF);
    Serial.println("Writing is Done");
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  }
  delay(2000);
}