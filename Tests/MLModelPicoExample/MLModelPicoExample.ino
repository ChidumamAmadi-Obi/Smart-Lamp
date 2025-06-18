#include <stdio.h>
#include "Calibration.h"
#include "Logic.h"

#define DEBUG_MAIN 0

#if DEBUG_MAIN == 1 //if debug is 1....
#define debugMain(x) Serial.print(x)
#define debugMainln(x) Serial.println(x)
#else //if debug is not 1.....
#define debugMain(x)
#define debugMainln(x)
#endif //end of the if statment

#define LDR_PIN 26

#define LAMP_PIN 20
#define BUTTON_PLUS 13
#define BUTTON_MINUS 14

#define TRIG_PIN 17
#define ECHO_PIN 16
#define SWITCH_PIN 15

bool prevlampStatus;
int prevlampBrightness;

bool motion;
float ambientLight; 
int lampBrightness;

unsigned long prevAutoToggle = 0;
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
  currentMillis = millis();

  motion = motionSensorHR_S04();
  ambientLight = Lux_Value(); 
  lampBrightness = brightnessSelect();

  toggleLamp(predicted_lamp_status);
  
  processData(ambientLight,motion);
}

void processData(int lux, int motion){ // prints data into serial port to be colleted by the python script
  currentMillis = millis();

  debugMain("Status:");       debugMain(lampStatus);
  debugMain(" , ");
  debugMain("Brightness:");  debugMain(lampBrightness);
  debugMain(" , ");
  debugMain("Motion:");           debugMain(motion);
  debugMain(" , ");
  debugMain("Ambient Light (lux):");
  debugMain(ambientLight);

  debugMain(" // ");
  debugMain("Pred Status:");       debugMain(predicted_lamp_status); 
  debugMain(" , ");
  debugMain("Pred Brightness:");   debugMainln(predicted_brightness);

  // Mock model, will be replaced my true machine learning model
  if ( ( motion == true ) && ( lux < 500 ) ){
    predicted_lamp_status = true;
    predicted_brightness = 5; 
    prevAutoToggle = currentMillis;
  } else if (( motion == true ) && ( lux < 1000 )) {
    predicted_lamp_status = true;
    predicted_brightness = 1; 
    prevAutoToggle = currentMillis;
  }

  if ( currentMillis - prevAutoToggle >= 10000) {
    if ( motion == false ) {
      predicted_lamp_status = false;
      predicted_brightness = 0; 
    }
    else {
      predicted_lamp_status = false;
      predicted_brightness = 0; 
    }
  }
}