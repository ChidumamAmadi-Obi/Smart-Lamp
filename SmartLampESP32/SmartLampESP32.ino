// DUMB LAMP

#include <stdio.h>
#include <WiFi.h>

#include "IRremote.hpp"
#include "time.h"
#include "Calibration.h"
#include "Logic.h"
#include "Global.h"
#include "LampLogic.h"

#define LOG_DATA 1

  #if LOG_DATA == 1 
  #define log(x) Serial.print(x)
  #define logln(x) Serial.println(x)
  #else 
  #define log(x)
  #define logln(x)
  #endif 

bool LEDState = LOW;
int motion = false;
int lux = 0;
float hour = 0.0;

const char* ssid       = "Google-Mesh";
const char* password   = "Chichi12";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

void setup() {
  Serial.begin(115200);

  pinMode(LAMP_PIN, OUTPUT);

  pinMode(PIR_MOTION_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  pinMode(LED_BUILTIN,OUTPUT);

  IrReceiver.begin(IR_PIN, ENABLE_LED_FEEDBACK);

  analogWrite(LAMP_PIN, LOW);

  connectToWiFi();
}

void loop() {
  currentMillis = millis();
  motion = PIRmotionSensor();
  lux = Lux_Value();
  hour = getHourOfDay();   
  lampStateMachine(); // handles lamp control

  processData( currentMillis );

  lampStateMachine(); // handles lamp control
}

//_______________________________________________________________________________________

float getHourOfDay(){
  struct tm timeinfo;
  if ( !getLocalTime(&timeinfo) ){
    Serial.println("Failed to obtain time");
  }
  float hour_of_day = timeinfo.tm_hour + timeinfo.tm_min/60.0; 

  // Serial.println(hour_of_day);

  return hour_of_day;
}

void processData(unsigned long currentMillis){ 

  predict_brightness( hour , motion , lux );

  //debug
    log("Time:");                   log(hour);
    log(" , ");
    log(" Status:");                log(state.currentLampStatus);
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

void connectToWiFi(){
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getHourOfDay();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
