#include <Arduino.h>
#include <SPI.h>
#include <DHT.h>
#include <Wire.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
#define DHT_PIN 5
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
float temperature, humid;
void UpdateData();
int lastTransmit = 0;
String receive;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  SerialBT.begin("ESP 32 Classic");
  dht.begin();
  pinMode(BUILTIN_LED, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (SerialBT.available())
  {
    char receiveData = SerialBT.read();
    receive += receiveData;
    if (receiveData == '!')
    {
      if (receive == "led on!")
      {
        Serial.println("Turn On LED");
        SerialBT.println("Turn On LED");
        digitalWrite(BUILTIN_LED, HIGH);
      }
      else if (receive == "led off!")
      {
        Serial.println("Turn OFF LED");
        SerialBT.println("Turn OFF LED");
        digitalWrite(BUILTIN_LED, LOW);
      }
      else
      {
        Serial.println("404");
        SerialBT.println("404");
      }
      receive = "";
      SerialBT.flush();
    }
  }
  if (millis() - lastTransmit > 5000)
  {
    UpdateData();
    String sentence = "Temperature: " + String(temperature) + "Humidity: " + String(humid);
    Serial.println(sentence);
    SerialBT.println(sentence);
    lastTransmit = millis();
  }
}

void UpdateData()
{
  temperature = dht.readTemperature();
  humid = dht.readHumidity();
}