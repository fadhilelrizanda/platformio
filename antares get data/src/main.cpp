#include <Arduino.h>
/*
  This code will fetch the latest data from your antares project device.
  Your Antares project device must have a structure like this:
  (Note that nesting the JSON object can only be done up to 2 levels using this library)
  {
    "temperature": some-integer,
    "humidity": some-integer,
    "wind_speed": some-float,
    "rain_level": some-float,
    "location" : {
      "latitude": "static-string",
      "longitude": "static-string"
    }
  }
  For more information please visit https://antares.id/id/docs.html
*/
#define LED D5
#define fan D4
#define Pump D6

int ledState;
int pumpState;
int fanState;
float limitTemp;
float limitMoist;

#include "AntaresESP8266HTTP.h"

#define ACCESSKEY "017c7e810b75e05b:0932f32db0c81348"
#define WIFISSID "TP-LINK 2"
#define PASSWORD "asdf1234"

#define projectName "antaresChallenge"
#define deviceName "actuatorState"

AntaresESP8266HTTP antares(ACCESSKEY);

void setup() {
  Serial.begin(9600);
  antares.setDebug(true);
  antares.wifiConnection(WIFISSID,PASSWORD);
  
 
   pinMode(LED,OUTPUT);
  pinMode(Pump,OUTPUT);
  pinMode(fan,OUTPUT);
}

void loop() {
  antares.get(projectName, deviceName); // Store latest value in buffer
  /*
    Uncomment the line below to use HTTP instead of HTTPS.
    Will be faster, but less secure
  */
  // antares.getNonSecure(projectName, deviceName);

 //  Check if we're actually getting data
 if(antares.getSuccess()) {
   // Get each values
   ledState = antares.getInt("LED");
   fanState = antares.getInt("Fan");
   pumpState = antares.getInt("Pump");
   limitTemp = antares.getFloat("limitTemp");
   limitMoist = antares.getFloat("limitMoisture");
 

   // Print each values
 


  
 
}
  Serial.println("LED State: " + String(ledState));
  Serial.println("Fan State: " + String(fanState));
  Serial.println("Pump State: " + String(pumpState));
  Serial.println("Limit Temp: " + String(limitTemp));
  Serial.println("Limit Moist: " + String(limitMoist));
  if(ledState==1){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }

if(fanState==1){
  if(limitTemp>60){
    digitalWrite(fan,HIGH);
     yield();
  }

 }
 else{
   digitalWrite(fan,LOW);
 }
if(pumpState==1){
  if(limitMoist<90)
  {
    digitalWrite(Pump,HIGH);
     
  }
}
else{
  digitalWrite(Pump,LOW);
}
  delay(5000);

}