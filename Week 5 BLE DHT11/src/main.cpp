#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Wire.h>
#include <SPI.h>
#include <DHT.h>

#define SERVICE_UUID "3180aa31-ab6b-4730-afeb-daaba2ece8b0"
#define CHARACTERISTIC_UUID "11f7955a-678f-4a67-bad7-24f6d415ed64"

#define DHT_PIN 5
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);
float temperature = 0, humidity = 0;

BLECharacteristic *pCharacteristic;

void updateDhtData();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

  BLEDevice::init("ESP32-BLE");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ |
                               BLECharacteristic::PROPERTY_NOTIFY |
                               BLECharacteristic::PROPERTY_INDICATE);
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  pServer->getAdvertising()->setMinPreferred(0x06);
  pServer->getAdvertising()->setMaxPreferred(0x12);
  pServer->getAdvertising()->start();
  Serial.println("Characteristic defined!");
}

void loop()
{
  // put your main code here, to run repeatedly:
  updateDhtData();
  String sensorData = String(temperature) + ";" + String(humidity);
  Serial.println(sensorData);

  pCharacteristic->setValue(sensorData.c_str());
  pCharacteristic->notify();
  delay(5000);
}

void updateDhtData()
{
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}