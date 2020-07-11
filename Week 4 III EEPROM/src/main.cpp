#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_SIZE 1

#define btn 15
unsigned char ledStatus = LOW;
bool changeLedStatus = false;
volatile bool interruptState = false;
int totalInterruptCounter = 0;

portMUX_TYPE gpioIntMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR gpioISR()
{
  portENTER_CRITICAL(&gpioIntMux);
  changeLedStatus = true;
  portEXIT_CRITICAL(&gpioIntMux);
}
void setup()
{
  // put your setup code here, to run once:
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  attachInterrupt(btn, &gpioISR, FALLING); //rising akan menyala jika dilepas falling ketika dipencet akan hidup

  EEPROM.begin(EEPROM_SIZE);
  delay(100);
  ledStatus = EEPROM.read(0);
  digitalWrite(BUILTIN_LED, ledStatus);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (changeLedStatus)
  {
    portENTER_CRITICAL(&gpioIntMux);
    changeLedStatus = false;
    portEXIT_CRITICAL(&gpioIntMux);
    ledStatus = !ledStatus;
    EEPROM.write(0, ledStatus);
    EEPROM.commit();
    digitalWrite(BUILTIN_LED, ledStatus);
  }
}