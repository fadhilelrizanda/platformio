#include <Arduino.h>
// Define pin
#define potPin 4
#define led1 27
#define led2 26
#define led3 25
#define led4 33

//Define Variable
float detectedTemp;
float potVolt;
float potValue;

//function to turn on LED
void ledOn(int ledPin, String ledName){
  
//Reset the LED
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
  digitalWrite(led4,LOW);

//Turn On Selected LED
  digitalWrite(ledPin,HIGH);
  Serial.println("Led On is: " + String(ledName) + "\n");
}

void setup() {
  // put your setup code here, to run once:

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  Serial.begin(9600);
}


void loop() {
  // put your main code here, to run repeatedly:
  //Convert Potensiometer Value
  potValue = analogRead(potPin);
  potVolt = potValue/4095*3.3;
  Serial.println("Detected Voltage: " + String(potVolt) + " Volt");
  detectedTemp = potVolt/3.3 *100;
  Serial.println("Detected Temperatures: " + String(detectedTemp) + " Celcius");
  
   if(detectedTemp <10 )
  {
    ledOn(led1,"led 1");
  }
  else if(detectedTemp >=10 && detectedTemp<20)
  {
    ledOn(led2,"led 2");
  }
  else if(detectedTemp >= 20 && detectedTemp <=30)
  {
    ledOn(led3,"led 3");
  }
  else if (detectedTemp >30)
  {
    ledOn(led4,"led 4");
  }
  else{
    Serial.print("Error Detected ! \n ");
  }  
  delay(2000);
}


