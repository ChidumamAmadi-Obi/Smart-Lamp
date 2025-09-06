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

  rtc_gpio_isolate(Config::WAKEUP_PIN);
  delay(10);

  IrReceiver.begin(Config::IR_PIN, ENABLE_LED_FEEDBACK);
  connectToWiFi();
}

void loop() {
  currentMillis = millis();
  getSensorReadings(motion,lux);

  getHourOfDay();   
  smartPredict(motion,lux);  
  lampStateMachine();
  
  static unsigned long lastSleepCheck = 0;
  if ( (currentMillis - lastSleepCheck > 5000) && (!motion)) {
    handleSleep();
    lastSleepCheck = currentMillis;
  } 
}

void getSensorReadings(bool &motion, float &lux) {
  motion = PIRmotionSensor();
  lux = Lux_Value();
}

// if the room has been left unoccupied for an extended period of time (5 mins), enter light sleep
// if room has been left unoccupied for a longer period (10 mins), enter deep sleep
  // deep sleep can be triggered by button as well
