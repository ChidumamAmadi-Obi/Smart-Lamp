#include "Config.h"
#include "driver/rtc_io.h"

unsigned long currentMillis;
unsigned long sleepEntryTime = 0;
float hourOfDay;
bool manualOverrideTimer = false;
bool autoToggle = false;
uint8_t IR_command = 0;


const char* ssid = "xxx";
const char* password   = "xxx";
const char* ntpServer = "pool.ntp.org";
const char* backupNtpServer = "time.nist.gov"; 

RTC_DATA_ATTR int bootCount = 0;

void initializePins() {
    pinMode(Config::LDR_PIN, INPUT);
    pinMode(Config::IR_PIN, INPUT);
    pinMode(Config::WAKEUP_PIN, INPUT);
    pinMode(Config::PIR_MOTION_PIN, INPUT);
    pinMode(Config::LED_BUILTIN, OUTPUT);
    pinMode(Config::LAMP_PIN, OUTPUT);

    analogWrite(Config::LAMP_PIN, 0);
}

SystemState state;
Timeing timeing;
Predict predicted;
struct tm timeinfo;