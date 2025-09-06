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

void getHourOfDay(){
  struct tm timeinfo;
  if ( !getLocalTime(&timeinfo) ){
    debugWiFiln("Failed to obtain time");
    return;
  }
  hourOfDay = timeinfo.tm_hour + timeinfo.tm_min/60.0; 
  dayNumber = timeinfo.tm_yday;
  // debugWiFi(hourOfDay); debugWiFi(" "); debugWiFiln(dayNumber);
}

void connectToWiFi(){
  debugWiFi("Connecting to ");
  debugWiFiln(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      debugWiFi(".");
  }
  debugWiFiln(" CONNECTION SUCCESSFUL");
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer, backupNtpServer);
  getHourOfDay();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF); //saves power
}
