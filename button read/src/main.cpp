#include <Arduino.h>

#define button 18

void setup() {
  // put your setup code here, to run once:
pinMode(button,INPUT_PULLUP);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int btnState = digitalRead(button);
Serial.print("State btn: " +String(btnState));
delay(2000);
}