#include <Arduino.h>

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
int tempId = 121;
int humidId = 122;
int lightId = 123;

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

void setup()
{
  Serial.begin(9600); // for debugging
  //* PWM Setup
  ledcSetup(ledChannel, pwmFreq, pwmRes);
  ledcAttachPin(ledPin, ledChannel);
  SerialBT.begin("ESP 32 BT Classic");
  dht.begin();  //dht 11
  Wire.begin(); // Light Sensor
}

void loop()
{
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

void luxRead()
{
  if (bh1750Read(bh1750Address) == bh1750Len)
    delay(200);
  {
    lux = (((unsigned short)buff[0] << 8) | (unsigned short)buff[1]) / 1.2;
    SerialBT.println(lux);
  }
}

void ledPwm()
{
  //* increase Brightness
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++)
  {
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
  //*decrease brightness
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--)
  {
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }
}

void ReadBT()
{
  if (SerialBT.available())
  {
    char receivedData = SerialBT.read();
    receiveString += receivedData;
    if (receivedData == '\n')
    {
      if (receiveString == "GET, 121:TEMP")
      {
        temp = dht.readTemperature();
        SerialBT.println(temp);
      }
      else if (receiveString == "GET, 122:HUMID")
      {
        humid = dht.readHumidity();
        SerialBT.println(temp);
      }
      else if (receiveString == "GET, 123:LUX")
      {
        luxRead();
      }
      else if (receiveString == "SET, 124:AUTOBRIGHTNESS,ON")
      {
            }
    }
  }