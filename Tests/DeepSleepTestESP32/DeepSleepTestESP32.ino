#include <Arduino.h>
#include "SleepManagment.h"
#include "driver/rtc_io.h"

unsigned long currentMillis;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN,OUTPUT);
  pinMode(BUTTON_PIN,INPUT);

  checkIfWakeingUp();
  rtc_gpio_isolate(BUTTON_PIN); 
  delay(10);
  
  esp_sleep_enable_ext0_wakeup(BUTTON_PIN, 1); // Wake on HIGH level
}

void loop() {
  currentMillis = millis();

  digitalWrite(LED_PIN,HIGH);  // ESP32 actively running code
  count++;
  Serial.print("COUNT: ");
  Serial.println(count);

  checkForSleep(currentMillis);
}