#include "Config.h"

unsigned long currentMillis;
float hourOfDay;
bool manualOverrideTimer = false;
bool autoToggle = true;

const char* ssid = "xxx";
const char* password   = "xxx";
const char* ntpServer = "pool.ntp.org";
const char* backupNtpServer = "time.nist.gov"; 

void initializePins() {
    pinMode(Config::LDR_PIN, INPUT);
    pinMode(Config::IR_PIN, INPUT);
    pinMode(Config::PIR_MOTION_PIN, INPUT);
    pinMode(Config::LED_BUILTIN, OUTPUT);
    pinMode(Config::AUTO_LED_PIN, OUTPUT);
    pinMode(Config::LAMP_PIN, OUTPUT);

    analogWrite(Config::LAMP_PIN, 0);
    analogWrite(Config::AUTO_LED_PIN,1);
}

SystemState state;
Timeing timeing;
Predict predicted;