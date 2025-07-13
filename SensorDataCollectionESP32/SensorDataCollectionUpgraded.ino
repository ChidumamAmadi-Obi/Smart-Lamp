// Gathering sensor data with ESP32
// Update: PIR sensor integration and IR control added

#include <stdio.h>
#include "IRremote.hpp"
#include "Calibration.h"
#include "Logic.h"
#include "Global.h"

#include <WiFi.h>
#include "time.h"

#define LOG_DATA true

  #if LOG_DATA == true //if debug is 1....
  #define log(x) Serial.print(x)
  #define logln(x) Serial.println(x)
  #else //if debug is not 1.....
  #define log(x)
  #define logln(x)
  #endif //end of the if statment

#define DEBUG_WIFI 0

  #if DEBUG_WIFI == 1 //if debug is 1....
  #define debugWiFi(x)    Serial.print(x)
  #define debugWiFiln(x)  Serial.println(x)
  #define debugWiFif(x,y) Serial.printf(x,y)
  #else //if debug is not 1.....
  #define debugWiFi(x)
  #define debugWiFiln(x)
  #define debugWiFif(x,y)
  #endif //end of the if statment

#define LOGGING_INTERVAL 3000000
#define AMBIENT_LIGHT_THRESHHOLD 100

bool motion;
int prevAmbientLight = 0;
int ambientLight; 
int loggedBrightness = 0;
int prevLoggedBrightness = 0;

const char* ssid       = ".....";
const char* password   = ".....";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

bool LEDBuzzerState = LOW;

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
  ambientLight = Lux_Value();
  loggedBrightness = lampStateMachine();
  getHourOfDay();

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

  llog(hour_of_day);               log(" , ");
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

void getHourOfDay(){
  struct tm timeinfo;
  if ( !getLocalTime(&timeinfo) ){
    debugWiFiln("Failed to obtain time");
    return;
  }
  hour_of_day = timeinfo.tm_hour + timeinfo.tm_min/60.0; 
}

void connectToWiFi(){
  //connect to WiFi
  debugWiFif("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      debugWiFi(".");
  }
  debugWiFiln(" CONNECTED");
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  getHourOfDay();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}
