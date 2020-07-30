#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <SPI.h>
#include <DHT.h>
#include <time.h>

#define DHT_PIN 23
#define DHT_TYPE DHT11

typedef struct sensorData
{
  struct tm timeInfo;
  float temp = 0;
  float humid = 0;
} sensorData;

const char *ssid = "TP-LINK 2";
const char *pwd = "asdf1234";
const char *ntpServer = "pool.ntp.org";
const long getOffSec = 25200;
const int dayLightOff = 0;
sensorData dht11Data;

DHT dht(DHT_PIN, DHT_TYPE);

String translateEncryption(wifi_auth_mode_t encryptionType);
void scanNetworks();
void connectToNetworks();
void updateDhtData();
void getLocalTime();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  connectToNetworks();
  configTime(getOffSec, dayLightOff, ntpServer);
}

void loop()
{
  // put your main code here, to run repeatedly:
  getLocalTime();
  updateDhtData();
  Serial.print("Temperature: ");
  Serial.print(dht11Data.temp);
  Serial.print(" C and Humidity: ");
  Serial.print(dht11Data.humid);
  Serial.print(" % at ");
  Serial.println(&dht11Data.timeInfo, "%A, %B %d %Y %H:%M:%S");
  delay(5000);
}

void getLocalTime()
{
  if (!getLocalTime(&dht11Data.timeInfo))
  {
    Serial.println("Failed To Get Time");
    return;
  }
}

void updateDhtData()
{
  dht11Data.humid = dht.readHumidity();
  dht11Data.temp = dht.readTemperature();
}

void connectToNetworks()
{
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Establishing Networks");
  }
  Serial.println("Connected To Networks");
}