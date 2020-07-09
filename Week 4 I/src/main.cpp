#include <Arduino.h>

volatile bool interruptState = false;
int totalInterruptCounter=0;

hw_timer_s *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;


void IRAM_ATTR onTimer() {
   portENTER_CRITICAL(&timerMux);
   interruptState = true;
   portEXIT_CRITICAL (&timerMux);
}

void setup() {
  // put your setup code here, to run once:

  // 80MHz = 80.000.000 dibagi 8000(begin) = 10.000(write)
  Serial.begin(9600);
  timer = timerBegin(0,8000,true);
timerAttachInterrupt(timer, &onTimer, true);
timerAlarmWrite(timer,10000, true);
timerAlarmEnable(timer); 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(interruptState)
  {
    portENTER_CRITICAL(&timerMux);
    interruptState = false;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;
    String sentences = "Total Interrupt " +String (totalInterruptCounter);
    Serial.print(sentences);
  }
}
