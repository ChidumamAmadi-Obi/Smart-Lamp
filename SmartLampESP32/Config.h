#pragma once
#ifndef CONFIG
#define CONFIG

#include <Arduino.h>

#define DEBUG_MODE 2   // 0 = no debuging
                        // 1 = basic debugging
                        // 2 = Logic debugging 
                        // 3 = calibration debugging
                        // 4 = thorough debugging
#if DEBUG_MODE == 0
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif
#if DEBUG_MODE == 1
#define DEBUG_WIFI true
#define DEBUG_CALIBRATION false
#define DEBUG_PREDICTION true
#define DEBUG_LOGIC false
#define DEBUG_SLEEP true
#endif
#if DEBUG_MODE == 2 
#define DEBUG_WIFI true
#define DEBUG_CALIBRATION false
#define DEBUG_PREDICTION true
#define DEBUG_LOGIC true
#define DEBUG_SLEEP false
#endif
#if DEBUG_MODE == 3
#define DEBUG_WIFI true
#define DEBUG_CALIBRATION true
#define DEBUG_PREDICTION false
#define DEBUG_LOGIC false
#define DEBUG_SLEEP false
#endif

#define uS_TO_S_FACTOR 1000000  // Conversion factor for microseconds to seconds

namespace Config {
    
  // Pin Configuration
  constexpr uint8_t LDR_PIN = 4;
  constexpr uint8_t LAMP_PIN = 27;
  constexpr uint8_t IR_PIN = 23;
  constexpr uint8_t PIR_MOTION_PIN = 19;
  constexpr uint8_t LED_BUILTIN = 2;
  constexpr uint8_t AUTO_LED_PIN = 26;
  constexpr uint8_t WIFI_LED_PIN = 25;
  constexpr uint8_t ERROR_LED_PIN = 33;

  constexpr gpio_num_t WAKEUP_PIN = GPIO_NUM_35;

  // IR Remote Commands
  constexpr uint8_t IR_ON = 69;
  constexpr uint8_t IR_OFF = 71;
  constexpr uint8_t IR_BRIGHTEN = 90;
  constexpr uint8_t IR_DIM = 8;
  constexpr uint8_t IR_AUTO = 68;
  constexpr uint8_t IR_SLEEP = 67;
  constexpr uint8_t IR_INVALID = 255;
  
  // Timing Variables
  constexpr unsigned long FADING_RATE = 5;
  constexpr unsigned long MANUAL_OVERRIDE_TIMEOUT = 10000; //10 seconds for testing
  constexpr unsigned long MOTION_SCAN_INTERVAL = 100;

  constexpr long GMTOFFSET_SEC = 0;
  constexpr uint16_t DAYLIGHTOFFSET_SEC = 3600;
  
  // LDR Calibration
  constexpr uint16_t MAX_ADC_READING = 4095;
  constexpr float ADC_REF_VOLTAGE = 3.33f;
  constexpr uint16_t REF_RESISTANCE = 10000;
  constexpr long LUX_CALC_SCALAR = 2355175;
  constexpr float LUX_CALC_EXPONENT = -1.2109f;
  constexpr float LUX_EMA_ALPHA = 0.3;
  constexpr uint16_t MAX_LDR_READING = 70000; 
  
  // Prediction Parameters
  constexpr uint8_t NUM_SLOTS = 96; 
  constexpr float DECAY_FACTOR = 0.99f; 
  constexpr uint8_t ACTIVITY_THRESHOLD = 2;
  constexpr uint8_t LUX_MAX_THRESHOLD = 50; 
  constexpr uint8_t LUX_MIN_THRESHOLD = 40; 

  constexpr uint8_t MAX_LUX = 45.0f;
  constexpr uint8_t MIN_LUX = 5.0f;

  // Sleep Parameters
  constexpr uint8_t LIGHT_SLEEP_TIMER = 3; // 3 seconds for testing
  
  // Brightness Levels
  constexpr uint8_t brightnessIndex[6] = {0, 51, 102, 153, 204, 255}; //pwm values
  
  void initializePins();
}

void decodeIrReceiver();

// LAMP STATE
struct SystemState {
    bool isFading = false;         
    bool lastLampStatus = false;  
    bool currentLampStatus = false;    
    uint8_t lastBrightnessNumber = 0;
    uint8_t currentBrightnessNumber = 0;  
};

// TIME KEEPING
struct Timeing {
    unsigned long prevMotionScan = 0;
    unsigned long prevRoomOccupation = 0;
    unsigned long prevLEDFlash = 0;
    unsigned long prevLog = 0;
    unsigned long prevAutoToggle = 0;
    unsigned long prevManualToggle = 0;
};

// MOCK ML MODEL PREDICTIONS
struct Predict {
    bool lampStatus = false;
    uint8_t brightness = 0;
};

// GLOBAL VARIABLES
extern unsigned long currentMillis;
extern unsigned long sleepEntryTime;
extern float hourOfDay;
extern bool manualOverrideTimer;
extern bool autoToggle;
extern uint8_t IR_command;

extern const char* ssid;
extern const char* password;
extern const char* ntpServer;
extern const char* backupNtpServer; 

// variables will survive after deep sleep
extern RTC_DATA_ATTR int bootCount;

// GLOBAL STRUCT INSTANCES
extern SystemState state;
extern Timeing timeing;
extern Predict predicted;
extern struct tm timeinfo;

#endif