#include <Arduino.h>

//* EEPROM Config
#include <EEPROM.h>
#define eepromSize 32
void readEEPROM(int address, char *data);
void writeEEPROM(int address, char *data);
char readData[128];
char pwdData[128];

//* LED & button Var
#define button 21

//* interrupt
volatile bool interruptState = false;
int totalInterrutpCounter = 0;
hw_timer_s *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE gpioIntMux = portMUX_INITIALIZER_UNLOCKED;
unsigned char ledStatus = LOW;
bool resetSSID_PWD = false;

void IRAM_ATTR gpioISR()
{
  detachInterrupt(digitalPinToInterrupt(button));
  portENTER_CRITICAL(&gpioIntMux);
  resetSSID_PWD = true;
  portEXIT_CRITICAL(&gpioIntMux);
  delay(50);
  attachInterrupt(button, &gpioISR, FALLING);
}

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(eepromSize);
  delay(2000);
  attachInterrupt(button, &gpioISR, FALLING);

  //Read EEPROM
  readEEPROM(0, readData);
  Serial.println("ISI EEPROM 0 :" + String(readData));
  readEEPROM(32, pwdData);
  Serial.println("ISI EEPROM 32 :" + String(pwdData));
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("State RESET ");
  Serial.println(resetSSID_PWD);
  delay(2000);
}

void readEEPROM(int address, char *data)
{
  for (int i = 0; i < 32; i++)
  {
    data[i] = EEPROM.read(address + 1);
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