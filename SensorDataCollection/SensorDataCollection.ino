// This sketch uses a raspbery pi pico to collect sensor data and sends it through serial port
// hr-s04, ldr, 3 touch buttons

// error: ldr output inf lux, fix 

#include <stdio.h>
#include "Calibration.h"

#define LDR_PIN 26

#define LAMP_PIN 20
#define BUTTON_PLUS 13
#define BUTTON_MINUS 14

#define TRIG_PIN 17
#define ECHO_PIN 16
#define SWITCH_PIN 15

int prevlampStatus, prevlampBrightness;

unsigned long previousMillis = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LAMP_PIN, OUTPUT);

  pinMode(BUTTON_PLUS, INPUT);
  pinMode(BUTTON_MINUS, INPUT);
  pinMode(SWITCH_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  analogReadResolution(12); 

  analogWrite(LAMP_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  bool motion = motionSensorHR_S04();
  float ambientLight = Lux_Value(); 
  int lampBrightness = brightnessSelect();
  
  if ((motion == 1) || (prevlampStatus != lampStatus) || (prevlampBrightness != lampBrightness)){ // if these parameterrs change, send data
    prevlampStatus = lampStatus;
    prevlampBrightness = lampBrightness;
    logData();
  }  
  else if (currentMillis - previousMillis >= 3000000) {
    previousMillis = currentMillis;
    logData();
  }
}

void logData(){ // prints data into serial port to be colleted by the python script
  bool motion = motionSensorHR_S04(); // true = motion detected, false = motion undetected
  float ambientLight = Lux_Value(); 
  int lampBrightness = brightnessSelect();

  Serial.print(lampStatus);
  Serial.print(",");
  Serial.print(lampBrightness);
  Serial.print(",");
  Serial.print(motion);
  Serial.print(",");
  Serial.println(ambientLight);

  // output: lampStatus,lampBrightness,motion,ambientLight
  // eg: 1,4,0,254
}

