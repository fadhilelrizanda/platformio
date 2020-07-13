#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 128
int eepromAddress[3] = {0, 32, 64};
String sentences[3] = {"State success", "SSID success", "PWD success"};
char readData[32], receivedData[32], ssid[32], pwd[32];
int dataIndex = 0;

char *strings[128];
char *ptr = NULL;
char test2[128];
bool test = true;
char *First;
char *Second;

void readEEPROM(int address, char *data);
void writeEEPROM(int address, char *data);

void setup()
{
  Serial.begin(9600);

  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  delay(100);
  readEEPROM(0, readData);
  Serial.print("Isi EEPROM 0: ");
  Serial.println(readData);
  readEEPROM(32, ssid);
  Serial.print("Isi EEPROM 32: ");
  Serial.println(ssid);
  readEEPROM(64, pwd);
  Serial.print("Isi EEPROM 64: ");
  Serial.println(pwd);
}

void changeChar(char nameValue);

void loop()
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
      test2[0] = '1';
      writeEEPROM(0, test2);

      // dataIndex = 0;
      //writeEEPROM(0, ptr);
      //memset(receivedData, 0, EEPROM_SIZE);
    }
  }
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

void changeChar(char nameValue)
{
  Serial.println("Entering Funcction");
  int i = 0;
  char str[] = {nameValue};
  char delim[] = ",";
  char *token = strtok(str, delim);
  while (i < 3)
  {
    token = strtok(NULL, delim);
    sentences[i] = token;
    i++;
  }
  for (int i = 0; i < 2; i++)
  {
    Serial.println(sentences[i]);
  }
}
/*
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

//light sensor
#define bh1750Address 0x23
#define bh1750Len 2

//function Light Sensor
void bh1750Req(int address);
int bh1750Read(int address);

// variable Light Sensors
byte buff[2];
unsigned short lux = 0;

//define Led and Button Pin
#define LED1 2
#define LED2 4
#define LED3 16
#define LED4 17
#define button 15
int btnState;

//btn
volatile bool interruptState = false;
int totalInterruptCounter = 0;
hw_timer_s *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//
char *strings[128];
char *ptr = NULL;

void IRAM_ATTR gpioISR()
{
  //ledStatus = !ledStatus;
  //digitalWrite(BUILTIN_LED,ledStatus);
}

void autoBrightness(); //auto brightness function

//EEPROM Configurations
#define eepromSize 128
#define eepromState 0
#define eepromSSID 32
#define eepromPWD 64

char readState[32], receivedData[32], readSSID[32], receiveSSID[32], readPWD[32], receivePWD[32];
int dataIndex = 0;
char readSerial[32];
int serialIndex = 0;

//
char *ssid;
char *pwd;

//* Function
void readEEPROM(int address, char *data);
void writeEEPROM(int address, char *data);

void setup()
{
  // put your setup code here, to run once:
  Wire.begin();
  EEPROM.begin(eepromSize);
  delay(200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);

  //* Read EEPROM State
  readEEPROM(eepromState, readState);
  Serial.println("LED State = " + String(readState));

  //* Read EEPROM SSID
  readEEPROM(eepromSSID, readSSID);
  Serial.println("SSID = " + String(readSSID));

  //*Read EEPROM PWD
  readEEPROM(eepromPWD, readPWD);
  Serial.println("PWD = " + String(readPWD));
}

void loop()
{

  //! Write EEPROM
  if (Serial.available())
  {
    receiveSSID[dataIndex] = Serial.read();
    dataIndex++;

    if (receiveSSID[dataIndex - 1] == '\n')
    {
      //Parsing Data

      byte indexstrok = 0;
      ptr = strtok(receivedData, ";");
      while (ptr != NULL)
      {
        strings[indexstrok] = ptr;
        indexstrok++;
        ptr = strtok(NULL, ";");
      }
      ssid = strings[0];
      pwd = strings[1];

      dataIndex = 0;
      writeEEPROM(eepromSSID,ssid);
      memset(ssid, 0, eepromSize);

       writeEEPROM(eepromSSID, receiveSSID);
      memset(receiveSSID, 0, eepromSize);
    }
  }
  //! AutoBRightness
  /*


  if (Serial.available())
  {
    Serial.println("Masukkan SSID");

    receiveSSID[dataIndex] = Serial.read();
    dataIndex++;

    if (receiveSSID[dataIndex - 1] == '\n')
    {
      dataIndex = 0;
      writeEEPROM(eepromSSID, readSSID);
      memset(receiveSSID, 0, eepromSize);
    }
*/
/*else if (Serial.read() == '2')
    {
      receivePWD[dataIndex] = Serial.read();
      dataIndex++;

      if (receivePWD[dataIndex - 1] == '\n')
      {
        dataIndex = 0;
        writeEEPROM(eepromPWD, readPWD);
        memset(receivePWD, 0, eepromSize);
      }
    }

    else if (Serial.read() == '3')
    {
      receiveState[dataIndex] = Serial.read();
      dataIndex++;

      if (receiveState[dataIndex - 1] == '\n')
      {
        dataIndex = 0;
        writeEEPROM(eepromState, readState);
        memset(receiveState, 0, eepromSize);
      }
  }
}
}
*/
/*
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
*/
/*void autoBrightness()
{
  if (btnState == !1)
  {
    memcpy(writepage, "ON", sizeof("ON"));
    spiChip.erasePageSector(0xFFFF);
    spiChip.pageWrite(writepage, 0XFFFF);
    Serial.println("Writing is Done");

    if (bh1750read(bh1750address) == bh1750Len)
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
*/
/*
//EEPROM Configurations
void readEEPROM(int address, char *data)
{
  for (int i = 0; i < eepromSize; i++)
  {
    data[i] = EEPROM.read(address + 1);
  }
}

void writeEEPROM(int address, char *data)
{
  Serial.println("Start Writing to EEPROM");
  for (int i = 0; i < eepromSize; i++)
  {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.commit();
}
*/