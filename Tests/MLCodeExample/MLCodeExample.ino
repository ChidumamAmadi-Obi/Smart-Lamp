#include <stdio.h>
#include "Calibration.h"
#include "Logic.h"
#include "Global.h"

#define LOG_DATA 0

  #if LOG_DATA == 1 //if debug is 1....
  #define log(x) Serial.print(x)
  #define logln(x) Serial.println(x)
  #else //if debug is not 1.....
  #define log(x)
  #define logln(x)
  #endif //end of the if statment

bool LEDBuzzerState = LOW;
int motion = false;
int lux = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LAMP_PIN, OUTPUT);

  pinMode(BUTTON_PLUS, INPUT);
  pinMode(BUTTON_MINUS, INPUT);
  pinMode(SWITCH_PIN, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  pinMode(LED_BUILTIN,OUTPUT);

  analogReadResolution(12); 

  analogWrite(LAMP_PIN, LOW);
}

void loop() {
  currentMillis = millis();

  motion = motionSensorHR_S04();
  lux = Lux_Value();
  lampStateMachine(); // handles lamp control
  blinkLED();

  processData(currentMillis);
}

void processData(unsigned long currentMillis){ 

  // MOCK MODEL, will be replaced by true machine learning model
  if ( ( motion == true ) && ( lux < 500 ) ){ //dark + movement full brightness
    predicted.lampStatus = true;
    predicted.brightness = 3; 
    timeing.prevAutoToggle = currentMillis;
  } else if (( motion == true ) && ( lux < 1000 )) { // partial dark + movement partial brightness
    predicted.lampStatus = true;
    predicted.brightness = 1; 
    timeing.prevAutoToggle = currentMillis;
  } else if ( currentMillis - timeing.prevAutoToggle >= 10000) { // after a certain time the lamp auto turns off
      predicted.lampStatus = false;
      predicted.brightness = 0; 
  }

  //debug
  log("Status:");                 log(state.currentLampStatus);
  log(" , ");
  log("Brightness:");             log(state.currentBrightnessNumber);
  log(" , ");
  log("Motion:");                 log(motion);
  log(" , ");
  log("Ambient Light (lux):");    log(lux);

  log(" // ");

  log("Pred Status: ");           log(predicted.lampStatus);
  log(" , ");
  log("Pred Brightness:");        logln(predicted.brightness);
}

void blinkLED() { // uses millis() to blink the LED                     
  currentMillis = millis(); 
  
  if (LEDBuzzerState == HIGH) {
    if (currentMillis - timeing.prevLEDFlash >= 250) { //turns LED on for "a" milliseconds
      timeing.prevLEDFlash = currentMillis;          //when the state of led is high this checks if it is time to turn the led state low
      LEDBuzzerState = LOW;
      digitalWrite(LED_BUILTIN, LEDBuzzerState);
    }
  } else {                                          //if the led state is low it checks if it is time to turn the led state  high
    if (currentMillis - timeing.prevLEDFlash >= 1000) { //turns LEDcoff for "b" milliseconds
      timeing.prevLEDFlash = currentMillis;
      LEDBuzzerState = HIGH;
      digitalWrite(LED_BUILTIN, LEDBuzzerState);
    }
  }
}