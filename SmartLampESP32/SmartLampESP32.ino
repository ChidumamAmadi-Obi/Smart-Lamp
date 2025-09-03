#include <stdio.h>
#include <WiFi.h>

#include "IRremote.hpp"
#include "time.h"
#include "Config.h"
#include "Calibration.h"
#include "Logic.h"
#include "Predict.h"
#include "WifiManager.h"

#define LOG_DATA true
#if LOG_DATA == 1 
#define log(x) Serial.print(x)
#define logln(x) Serial.println(x)
#else 
#define log(x)
#define logln(x)
#endif 

bool motion = false;
int lux = 0;

void setup() {
  Serial.begin(115200);
  Config::initializePins;
  IrReceiver.begin(Config::IR_PIN, ENABLE_LED_FEEDBACK);
  connectToWiFi();
}

void loop() {
  currentMillis = millis();
  motion = PIRmotionSensor();
  lux = Lux_Value();

  getHourOfDay();   
  smartPredict(motion,lux);  
  lampStateMachine();
}