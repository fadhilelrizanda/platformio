#include <Arduino.h>

//* EEPROM
#include <EEPROM.h>
#define eepromSize 32
void readEEPROM(int address, char *data);
void writeEEPROM(int address, char *data);
char readData[32];

//* Light Sensor
#include <SPI.h>  //and DHT 11
#include <Wire.h> //and DHT 11
#define bh1750Address 0x23
#define bh1750Len 2
void bh1750Req(int address); //req data sensor
int bh1750Read(int address); // read data sensor
byte buff[2];
unsigned short lux = 0;

//* PWM and LED Setup
#define ledPin 18
#define ledChannel 0
#define pwmFreq 5000
#define pwmRes 8

//* Sensor ID
int dht11 = 121;
int lightId = 123;
int idBrigtness = 124;
#define deviceId 122

//* DHT 11
#include <DHT.h>
#define DHT_PIN 5
#define DHT_TYPE DHT11
int lastTransmit = 0;

DHT dht(DHT_PIN, DHT_TYPE);
float temp, humid;

//* Bluetooth Config
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;
String receiveString;

char *ptr = NULL;
char *strings[128];
char *strings2[128];
String receive;
char receivedData[32];
int dataIndex = 0;
bool condition;
int dutyCycle;

void autoBrightness(bool condition);
void ReadBT();

void setup()
{
  Serial.begin(9600); // for debugging
  //* PWM Setup
  ledcSetup(ledChannel, pwmFreq, pwmRes);
  ledcAttachPin(ledPin, ledChannel);
  SerialBT.begin("ESP 32 BT Classic");
  dht.begin();  //dht 11
  Wire.begin(); // Light Sensor
  EEPROM.begin(eepromSize);
  delay(200);
  readEEPROM(0, readData);
  SerialBT.println("Autobrightness : " + String(readData));
  if (String(readData) == "ON")
  {
    SerialBT.println("ISI EEPROM ON!");
  }
  else
  {
    SerialBT.println("ISI EEPROM OFF!");
  }
  SerialBT.println("*********************");
}

void loop()
{
  if (SerialBT.available())
  {
    receivedData[dataIndex] = SerialBT.read();
    dataIndex++;

    if (receivedData[dataIndex - 1] == '\n')
    {
      byte indexstrok = 0;
      ptr = strtok(receivedData, ",:");
      while (ptr != NULL)
      {
        strings[indexstrok] = ptr;
        indexstrok++;
        ptr = strtok(NULL, ",:");
      }
      if (String(strings[0]) == "GET")
      {
        // SerialBT.println("Getting Sensor data...");
        if (String(strings[1]) == String(deviceId))
        {
          //SerialBT.println("Sensor DHT11");
          if (String(strings[2]) == "TEMP\r\n")
          {
            float nilaiSuhu;
            nilaiSuhu = dht.readTemperature();
            SerialBT.println(nilaiSuhu);
          }
          else if (String(strings[2]) == "HUMID\r\n")
          {
            float nilaiHumid;
            nilaiHumid = dht.readHumidity();
            SerialBT.println(nilaiHumid);
          }
          else if (String(strings[2]) == "LUX\r\n")
          {
            if (condition)
            {
              SerialBT.println(lux);
            }
            else
            {
              SerialBT.println("Light Sensor OFF");
            }

           
          }
          else
          {
            SerialBT.println("Sensor DHT11 404");
          }
        }
      }
      else if (String(strings[0]) == "SET")
      {
        //  SerialBT.println("Set State Success");
        if (String(strings[1]) == String(deviceId))
        {
          // SerialBT.println("Set ID Success");
          if (String(strings[2]) == "AUTOBRIGHTNESS")
          {
            SerialBT.println("ACK");
            writeEEPROM(0, strings[3]);
          }
          else
          {
            SerialBT.println("404");
          }
        }
        else
        {
          SerialBT.println("404");
        }
      }
      else
      {
        SerialBT.println("404");
      }
      memset(receivedData, 0, 32);
      dataIndex = 0;
    }
  }
  if (millis() - lastTransmit > 1000)
  {

    readEEPROM(0, readData);
    if (String(readData) == "ON\r\n")
    {
      condition = true;
    }
    else if (String(readData) == "OFF\r\n")
    {
      condition = false;
    }
    else
    {
      SerialBT.println("Unrecognized Mode");
    }
    lastTransmit = millis();
  }
  autoBrightness(condition);
}
void bh1750Req(int address)
{
  Wire.beginTransmission(address);
  Wire.write(0x10);
  Wire.endTransmission();
}

int bh1750Read(int address)
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

void autoBrightness(bool condition)
{

  if (condition == true)
  {
    bh1750Req(bh1750Address);
    delay(200);
    if (bh1750Read(bh1750Address) == bh1750Len)
    {
      lux = (((unsigned short)buff[0] << 8) | (unsigned short)buff[1]) / 1.2;
    }

    dutyCycle = lux;
    dutyCycle = map(dutyCycle, 3000, 0, 0, 255);
    ledcWrite(ledChannel, dutyCycle);
    delay(20);
  }
  else
  {
    ledcWrite(ledChannel, 0);
  }
}