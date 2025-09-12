#ifndef SLEEP
#define SLEEP

#include "Config.h"

void handleWakeUp(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  bootCount++;
  Serial.println("Boot no. " + String(bootCount));

  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("EXITING DEEP SLEEP"); Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    default : Serial.printf("Wakeup was not caused by Deep Sleep: %d\n",wakeup_reason); break;
  }

}

void enterDeepSleep(){
  Serial.println("ENTERING DEEP SLEEP...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  esp_sleep_enable_ext0_wakeup(Config::WAKEUP_PIN,1);
  analogWrite(Config::WIFI_LED_PIN,0);
  analogWrite(Config::AUTO_LED_PIN,0);

  Serial.flush(); 
  esp_deep_sleep_start();
}

void handleSleep( bool motion ){
  if (!motion) {
    Serial.println("ROOM UNOCCUPIED, ENTERING LIGHT SLEEP...");   
    if (!PIRmotionSensor()) {
      lampToggle(true);
      Serial.flush();
      esp_sleep_enable_timer_wakeup(Config::LIGHT_SLEEP_TIMER * uS_TO_S_FACTOR); // 3 seconds in light sleep
      esp_light_sleep_start();
    } 
    if (!PIRmotionSensor()){
      enterDeepSleep();
    }
  } else if ( IR_command == Config::IR_SLEEP ) {
    Serial.println("MANUAL TOGGLE, ENTERING LIGHT SLEEP...");
    lampToggle(true);
    Serial.flush();
    esp_sleep_enable_timer_wakeup(Config::LIGHT_SLEEP_TIMER * uS_TO_S_FACTOR); // 3 seconds in light sleep
    esp_light_sleep_start();
    enterDeepSleep();
  }
  IR_command = 0;
}

#endif