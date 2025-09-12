#ifndef WIFI
#define WIFI

#include "Config.h"

#if DEBUG_WIFI
#define debugWiFi(x) Serial.print(x)
#define debugWiFiln(x) Serial.println(x)
#else 
#define debugWiFi(x)
#define debugWiFiln(x)
#endif

const long  gmtOffset_sec = 0;
const uint16_t   daylightOffset_sec = 3600;

void connectToWiFi() {
  debugWiFi("Connecting to "); debugWiFiln(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    debugWiFi(".");
  }
  debugWiFiln(" CONNECTION SUCCESSFUL");
  analogWrite(Config::WIFI_LED_PIN,255);
  configTime(Config::GMTOFFSET_SEC, Config::DAYLIGHTOFFSET_SEC, ntpServer, backupNtpServer);  
}

void getHourOfDay(){
  if ( !getLocalTime(&timeinfo) ){
    debugWiFiln("Failed to obtain time");
    connectToWiFi();
    return;
  }
  hourOfDay = timeinfo.tm_hour + timeinfo.tm_min/60.0; 
}

void handleWiFi(){
  connectToWiFi();
  getHourOfDay();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF); //saves power
}

#endif