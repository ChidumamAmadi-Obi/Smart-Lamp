#include "Config.h"
#include "driver/rtc_io.h"

unsigned long currentMillis;
unsigned long sleepEntryTime = 0;
float hourOfDay;
uint8_t prevDayNumber;
bool manualOverrideTimer = false;
bool autoToggle = false;

const char* ssid = "Google-Mesh";
const char* password   = "Chichi12";
const char* ntpServer = "pool.ntp.org";
const char* backupNtpServer = "time.nist.gov"; 

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR uint8_t dayNumber;

void initializePins() {
    pinMode(Config::LDR_PIN, INPUT);
    pinMode(Config::IR_PIN, INPUT);
    pinMode(Config::WAKEUP_PIN, INPUT);
    pinMode(Config::PIR_MOTION_PIN, INPUT);
    pinMode(Config::LED_BUILTIN, OUTPUT);
    pinMode(Config::AUTO_LED_PIN, OUTPUT);
    pinMode(Config::LAMP_PIN, OUTPUT);

    analogWrite(Config::LAMP_PIN, 0);
    analogWrite(Config::AUTO_LED_PIN, 0);
}

SystemState state;
Timeing timeing;
Predict predicted;