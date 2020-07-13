#include <Arduino.h>
#include <EEPROM.h> //for EEPROM
#include <SPI.h>    // for Sensors
#include <Wire.h>   //for Sensors
#include <w25q64.hpp>

//* EEPROM Config
#define EEPROM_SIZE 128
char readData[128], receivedData[128], ssid[128], pwd[128];
int dataIndex = 0;
char ledState[128];
bool state;
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

//* LED config
#define LED1 2
#define LED2 4
#define LED3 16
#define LED4 17
#define button 15
int btnState;

//* interrupt
volatile bool interruptState = false;
int totalInterruptCounter = 0;
hw_timer_s *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE gpioIntMux = portMUX_INITIALIZER_UNLOCKED;

unsigned char ledStatus = LOW;
bool changeLedStatus = false;

//function

void writeState();

void IRAM_ATTR gpioISR()
{
  detachInterrupt(digitalPinToInterrupt(button));
  portENTER_CRITICAL(&gpioIntMux);
  changeLedStatus = true;
  portEXIT_CRITICAL(&gpioIntMux);
  delay(50);
  attachInterrupt(button, &gpioISR, FALLING);
}

void autoBrightness(bool condition);
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  EEPROM.begin(EEPROM_SIZE); // Initialize EEPROM
  delay(1000);

  //Actuator Pin
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  pinMode(BUILTIN_LED, OUTPUT);
  attachInterrupt(button, &gpioISR, FALLING);

  //Read EEPROM
  readEEPROM(0, readData);
  Serial.print("Autobrightness Mode: ");
  Serial.println(readData);
  readEEPROM(32, ssid);
  Serial.print("SSID: ");
  Serial.println(ssid);
  readEEPROM(64, pwd);
  Serial.print("PWD: ");
  Serial.println(pwd);
  if (readData[0] == '1')
  {
    ledStatus = true;
    digitalWrite(BUILTIN_LED, HIGH);
  }
  else
  {
    ledStatus = false;

    digitalWrite(BUILTIN_LED, LOW);
  }
  Serial.println("Masukkan ssid;password");
  delay(2000);
}

void changeChar();

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
      Serial.println("Success ssid & pwd");
    }
  }
  if (changeLedStatus)
  {
    portENTER_CRITICAL(&gpioIntMux);
    changeLedStatus = false;
    portEXIT_CRITICAL(&gpioIntMux);
    ledStatus = !ledStatus;
    if (ledStatus)
    {
      ledState[0] = '1';
    }
    else
    {
      ledState[0] = '0';
    }

    writeEEPROM(0, ledState);
    digitalWrite(BUILTIN_LED, ledStatus);
    autoBrightness(ledStatus);
    Serial.println(ledStatus);
    delay(100);
  }
  autoBrightness(ledStatus);
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

void autoBrightness(bool condition)
{

  if (condition == true)
  {
    bh1750Req(bh1750Address);
    delay(200);
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

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
  }
  delay(100);
}
