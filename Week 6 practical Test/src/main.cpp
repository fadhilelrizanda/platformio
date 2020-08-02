#include <Arduino.h>

#define deviceId 122
//* WIFI
#include <WiFi.h>
void connectToNetworks();
char *ssid;
char *pwd;

//* EEPROM Config
#include <EEPROM.h>
#define eepromSize 64
void readEEPROM(int address, char *data);
void writeEEPROM(int address, char *data);
char readData[64];
char pwdData[64];
char nullString[64];
char receivedData[64];

int lastTransmit = 0;

char *strings[64];
int dataIndex = 0;
char *ptr = NULL;

#define btn 15
int resetStatus = HIGH;
bool mode;

//* Bluetooth Config
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
String receiveString;

//* interrupt
volatile bool interruptState = false;
int totalInterrutpCounter = 0;
hw_timer_s *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE gpioIntMux = portMUX_INITIALIZER_UNLOCKED;
unsigned char ledStatus = LOW;
bool resetSSID_PWD = false;
int totalInterruptCounter = 0;

void IRAM_ATTR gpioISR()
{
  detachInterrupt(digitalPinToInterrupt(btn));
  portENTER_CRITICAL(&gpioIntMux);
  resetSSID_PWD = !resetSSID_PWD;
  portEXIT_CRITICAL(&gpioIntMux);
  ledStatus = !ledStatus;
  digitalWrite(BUILTIN_LED, ledStatus);
  delay(50);
  attachInterrupt(btn, &gpioISR, FALLING);
}
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  EEPROM.begin(eepromSize);
  delay(2000);

  //Read EEPROM
  readEEPROM(0, readData);
  Serial.println("SSID :" + String(readData));
  readEEPROM(32, pwdData);
  Serial.println("PASSWORD :" + String(pwdData));
  if (String(readData) == NULL)
  {
    Serial.println("BL Mode");
    mode = true;
    SerialBT.begin("ESP 32 BT Classic");
  }
  else
  {
    ssid = readData;
    pwd = pwdData;
    Serial.println("WIFI Mode");
    // Serial.println(ssid);
    // Serial.println(pwd);
    mode = false;
    connectToNetworks();
  }
  attachInterrupt(btn, &gpioISR, RISING); //rising akan menyala jika dilepas falling ketika dipencet akan hidup
}

void loop()
{
  if (resetSSID_PWD)
  {
    Serial.println("reset ");
    memset(nullString, 0, 128);
    writeEEPROM(0, nullString);
    writeEEPROM(32, nullString);
  }
  else
  {
    // Serial.println("Not gonna reset");
  }

  if (mode)
  {

    if (SerialBT.available())
    {
      receivedData[dataIndex] = SerialBT.read();
      dataIndex++;

      if (receivedData[dataIndex - 1] == '\n')
      {
        byte indexstrok = 0;
        ptr = strtok(receivedData, ",:\r\n");
        while (ptr != NULL)
        {
          strings[indexstrok] = ptr;
          indexstrok++;
          ptr = strtok(NULL, ",:\r\n");
        }
        // SerialBT.println(String(strings[0]));
        // SerialBT.println(String(strings[1]));
        // SerialBT.println(String(strings[2]));
        // SerialBT.println(String(strings[3]));

        if (String(strings[0]) == "SET")
        {
          if (String(strings[1]) == String(deviceId))
          {
            if (String(strings[2]) == "SSID")
            {
              writeEEPROM(0, strings[3]);
              // SerialBT.println("SSID ditulis");
              SerialBT.println("ACK\r\n");
            }
            else if (String(strings[2]) == "PASS")
            {
              writeEEPROM(32, strings[3]);
              // SerialBT.println("Password ditulis");
              SerialBT.println("ACK\r\n");
            }
            else
            {
              Serial.println("Wrong command");
            }
          }
          else
          {
            Serial.println("Wrong command");
          }
        }
        else
        {
          Serial.println("Wrong command");
        }
        dataIndex = 0;
        memset(receivedData, 0, eepromSize);
      }
    }
  }
  else
  {
    if (millis() - lastTransmit > 10000)
    {
      Serial.println("Mode WiFi");
      lastTransmit = millis();
    }
  }
}

void readEEPROM(int address, char *data)
{
  for (int i = 0; i < eepromSize; i++)
  {
    data[i] = EEPROM.read(address + i);
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

void connectToNetworks()
{
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Establishing Networks");
    if (millis() > 35000)
    {
      break;
    }
  }
  Serial.println("Connected To Networks");
}