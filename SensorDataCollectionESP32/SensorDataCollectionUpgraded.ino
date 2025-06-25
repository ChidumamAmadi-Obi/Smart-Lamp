// Gathering sensor data with ESP32
// Update: PIR sensor integration and IR control added

#include <stdio.h>
#include "IRremote.hpp"
#include "Calibration.h"
#include "Logic.h"
#include "Global.h"

#define LOG_DATA true

  #if LOG_DATA == true //if debug is 1....
  #define log(x) Serial.print(x)
  #define logln(x) Serial.println(x)
  #else //if debug is not 1.....
  #define log(x)
  #define logln(x)
  #endif //end of the if statment

#define LOGGING_INTERVAL 3000000
#define AMBIENT_LIGHT_THRESHHOLD 100

bool motion;
int prevAmbientLight = 0;
int ambientLight; 
int loggedBrightness = 0;
int prevLoggedBrightness = 0;

bool LEDBuzzerState = LOW;

void setup() {
  Serial.begin(115200);

  pinMode(LAMP_PIN, OUTPUT);

  pinMode(PIR_MOTION_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  pinMode(LED_BUILTIN,OUTPUT);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  analogWrite(LAMP_PIN, LOW);
}

void loop() {
  currentMillis = millis();

  motion = PIRmotionSensor();
  ambientLight = Lux_Value();
  loggedBrightness = lampStateMachine();

  if (( currentMillis - timeing.prevMotionScan >= 10000 ) || (state.lastLampStatus != state.currentLampStatus) || (prevLoggedBrightness != loggedBrightness) || ( abs(prevAmbientLight - ambientLight) == AMBIENT_LIGHT_THRESHHOLD )){ // if these parameterrs change, send data
    state.lastLampStatus = state.currentLampStatus;
    prevLoggedBrightness = loggedBrightness;
    prevAmbientLight = ambientLight;
    logData();
  }  
  else if (currentMillis - timeing.prevLog >= LOGGING_INTERVAL) {
    timeing.prevLog = currentMillis;
    logData();
  }
  blinkLED();
}

void logData(){ // prints data into serial port to be colleted by the python script

  log(state.currentLampStatus);   log(" , ");
  log(loggedBrightness);          log(" , ");
  log(motion);                    log(" , ");
  logln(ambientLight);
}

void blinkLED() { // uses millis() to blink the LED                     
  currentMillis = millis(); 
  
  if (LEDBuzzerState == HIGH) {
    if (currentMillis - timeing.prevLEDFlash >= 250) { //turns LED on for 250 milliseconds
      timeing.prevLEDFlash = currentMillis;          //when the state of led is high this checks if it is time to turn the led state low
      LEDBuzzerState = LOW;
      digitalWrite(LED_BUILTIN, LEDBuzzerState);
    }
  } else {                                          //if the led state is low it checks if it is time to turn the led state  high
    if (currentMillis - timeing.prevLEDFlash >= 1000) { //turns LEDcoff for 1000 milliseconds
      timeing.prevLEDFlash = currentMillis;
      LEDBuzzerState = HIGH;
      digitalWrite(LED_BUILTIN, LEDBuzzerState);
    }
  }
}