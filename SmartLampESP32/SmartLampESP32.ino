#include <stdio.h>
#include <WiFi.h>
#include "IRremote.hpp"
#include "driver/rtc_io.h"

#include "time.h"
#include "Config.h"
#include "Calibration.h"
#include "Logic.h"
#include "Predict.h"
#include "WifiManager.h"
#include "SleepManager.h"

static bool motion = false;
static float lux = 0;

void setup() {
  Serial.begin(115200);
  handleWakeUp();
  Config::initializePins;

  pinMode(Config::AUTO_LED_PIN,OUTPUT);
  pinMode(Config::WIFI_LED_PIN,OUTPUT);
  pinMode(Config::ERROR_LED_PIN,OUTPUT);

  IrReceiver.begin(Config::IR_PIN, ENABLE_LED_FEEDBACK);
  handleWiFi();

  initPredictionSystem();
}
void loop() {
  currentMillis = millis();
  getSensorReadings(motion,lux);
  getHourOfDay();   

  if (autoToggle)      smartPredict(motion);
  decodeIrReceiver();  lampStateMachine();
  decodeIrReceiver();  handleSleep(motion);
}

void getSensorReadings(bool &motion, float &lux) {
  motion = PIRmotionSensor();
  lux = Lux_Value();
}

inline void decodeIrReceiver(){
  if (IrReceiver.decode()) {
    IR_command = IrReceiver.decodedIRData.command;
    IrReceiver.resume();
  } 
}