unsigned long prevButtonPress;

// variables will survive after deep sleep
RTC_DATA_ATTR int count;
RTC_DATA_ATTR unsigned long sleepEntryTime = 0;
RTC_DATA_ATTR int bootCount = 0;

#define BUTTON_PIN GPIO_NUM_35
#define LED_PIN 27
#define uS_TO_S_FACTOR 1000000  // Conversion factor for microseconds to seconds
#define TIME_UNTIL_SLEEP 3000 // 3secs
#define BUFFER 2000 // wait 2sec to prevent rapi

void trackButtonPress() {
  if (digitalRead(32)){
    prevButtonPress = millis();
    Serial.print("BUTTON PRESSED");
  }
}

void checkIfWakeingUp(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();

  bootCount++;
  Serial.println("Boot no. " + String(bootCount));

  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    default : Serial.printf("Wakeup was not caused by Deep Sleep: %d\n",wakeup_reason); break;
  }
}


void enterDeepSleep(){
  Serial.println("ENTERING DEEP SLEEP...");
  digitalWrite(27,LOW);
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  delay(10);

  Serial.flush(); 
  esp_deep_sleep_start();
}

void checkForSleep(unsigned long currentMillis){
  currentMillis = millis();
  if (currentMillis - prevButtonPress >= TIME_UNTIL_SLEEP) { // if the button isnt pressed once every five seconds, enter light sleep
    Serial.println("PREPARING TO ENTER LIGHT SLEEP");
    if (sleepEntryTime == 0) {
      sleepEntryTime = millis();
    }
    if (currentMillis - sleepEntryTime >= BUFFER) {
      Serial.println("ENTERING LIGHT SLEEP...");   
      digitalWrite(27,LOW);
      delay(10);
      esp_sleep_enable_timer_wakeup(3 * uS_TO_S_FACTOR); // 3 second
      Serial.flush();
      esp_light_sleep_start();

      // if light sleep time passed without a button press, enter deep sleep
      enterDeepSleep();
    } else {
      sleepEntryTime = 0;
    }
  } 
}