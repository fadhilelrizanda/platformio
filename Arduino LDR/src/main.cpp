#include <Arduino.h>
#define buzzer 4
int sensorPin = A0; // select the input pin for LDR

int sensorValue = 0; // variable to store the value coming from the sensor
void setup()
{
  Serial.begin(9600); //sets serial port for communication
  pinMode(buzzer, OUTPUT);
}
void loop()
{
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  Serial.println(sensorValue);         //prints the values coming from the sensor on the screen
  if (sensorValue <= 500)
  {
    digitalWrite(buzzer, HIGH);
    delay(500);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
  delay(500);
}