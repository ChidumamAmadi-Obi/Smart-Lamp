#include <stdio.h>
#include "Calibration.h"
#include "Logic.h"
#include "Global.h"

#define DEBUG_MAIN 0

  #if DEBUG_MAIN == 1 //if debug is 1....
  #define debugMain(x) Serial.print(x)
  #define debugMainln(x) Serial.println(x)
  #else //if debug is not 1.....
  #define debugMain(x)
  #define debugMainln(x)
  #endif //end of the if statment

bool motion;
float ambientLight; 
int lampBrightness;

bool LEDBuzzerState = LOW;

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
  ambientLight = Lux_Value();
  toggleLamp(predict.MLlampstatus);
  lampBrightness = brightnessSelect(predict.MLbrightness);
  
  processData(ambientLight,motion);
  blinkLED();
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
  debugMain("Pred Status:");       debugMain(predict.MLlampstatus); 
  debugMain(" , ");
  debugMain("Pred Brightness:");   debugMainln(predict.MLbrightness);

  // Mock model, will be replaced my true machine learning model
  if ( ( motion == true ) && ( lux < 500 ) ){
    predict.MLlampstatus = true;
    predict.MLbrightness = 5; 
    timeing.prevAutoToggle = currentMillis;
  } else if (( motion == true ) && ( lux < 1000 )) {
    predict.MLlampstatus = true;
    predict.MLbrightness = 1; 
    timeing.prevAutoToggle = currentMillis;
  }

  if ( currentMillis - timeing.prevAutoToggle >= 10000) {
    if ( motion == false ) {
      predict.MLlampstatus = false;
      predict.MLbrightness = 0; 
    }
    else {
      predict.MLlampstatus = false;
      predict.MLbrightness = 0; 
    }
  }
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