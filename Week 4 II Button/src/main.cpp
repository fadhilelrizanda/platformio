#include <Arduino.h>

#define btn 21
int ledStatus = LOW;

volatile bool interruptState = false;
int totalInterruptCounter=0;

hw_timer_s *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR gpioISR() {
ledStatus = !ledStatus;
digitalWrite(BUILTIN_LED,ledStatus);
}
void setup() {
  // put your setup code here, to run once:
pinMode(BUILTIN_LED,OUTPUT);
pinMode(btn, INPUT_PULLUP);
attachInterrupt(btn,&gpioISR, RISING); //rising akan menyala jika dilepas falling ketika dipencet akan hidup
}

void loop() {
  // put your main code here, to run repeatedly:
}