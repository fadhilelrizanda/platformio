#include <Arduino.h>
/*
 * PIR sensor tester
 */

int ledPin = 13;    // choose the pin for the LED
int inputPin = 6;   // choose the input pin (for PIR sensor)
int pirState = LOW; // we start, assuming no motion detected
int val = 0;        // variable for reading the pin status

void setup()
{
  pinMode(ledPin, OUTPUT);  // declare LED as output
  pinMode(inputPin, INPUT); // declare sensor as input

  Serial.begin(9600);
}

void loop()
{
  Serial.println(analogRead(inputPin));
  Serial.println(digitalRead(inputPin));
  delay(1000);
}