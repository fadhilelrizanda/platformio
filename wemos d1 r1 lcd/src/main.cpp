#include<Arduino.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>

#define LED D5
#define fan D4
#define pump D6
#define DHTPIN D8
#define soilMoisture A0


int soilValue;

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);  // Set the LCD I2C address

void setup()
{
  lcd.begin (16,2); // for 16 x 2 LCD module
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home ();  // go home
    pinMode(LED,OUTPUT);
  pinMode(fan,OUTPUT);
  pinMode(pump,OUTPUT);
  Serial.begin(9600);
  
    
}

void loop()
{
  
  
   lcd.print("Hello World");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.print ("I2C Address 0x27");  
   delay(10000);
  lcd.clear();
  delay(5000);

/*
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
   Serial.print(F("Humidity: "));
   lcd.print("Humidity" + String(h));

  Serial.println(h);
  Serial.print(F("%  Temperature: "));
  lcd.setCursor(0,1);
  lcd.print("Temperature" + String(t));
  Serial.println(t);
  
  soilValue = analogRead(soilMoisture);
  Serial.println(soilValue);
  delay(2000);
  lcd.clear();
 */

}

/*
#include <Arduino.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(16x2); 

//define pin for actuator and sensor
#define LED D5
#define fan D4
#define pump D6
#define DHTPIN D7
#define soilMoisture A0

//define type from adafruit lib
#define DHTTYPE DHT11   // DHT 11

//define value for soil moisture sensor
int soilValue;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // define pin
  pinMode(LED,OUTPUT);
  pinMode(fan,OUTPUT);
  pinMode(pump,OUTPUT);
lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("hello everyone");
  lcd.setCursor(1,1);
  lcd.print("konichiwaa");
 
  dht.begin();
  Serial.begin(9600);
}

void loop() {



  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
   Serial.print(F("Humidity: "));
  Serial.println(h);
  Serial.print(F("%  Temperature: "));
  Serial.println(t);
  delay(2000);
  soilValue = analogRead(soilMoisture);
  Serial.println(soilValue);

  
  
}
*/