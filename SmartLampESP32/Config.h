#pragma once
#include <Arduino.h>

namespace Config {
  // Debug settings
  // constexpr bool DEBUG_WIFI = true;
  // constexpr bool DEBUG_CALIBRATION = false;
  // constexpr bool DEBUG_PREDICTION = true;
  // constexpr bool DEBUG_LOGIC = false;
  // constexpr bool LOG_DATA = false;
  
  // Pin Configuration
  constexpr uint8_t LDR_PIN = 4;
  constexpr uint8_t LAMP_PIN = 27;
  constexpr uint8_t IR_PIN = 23;
  constexpr uint8_t PIR_MOTION_PIN = 19;
  constexpr uint8_t LED_BUILTIN = 2;
  constexpr uint8_t AUTO_LED_PIN = 13;
  
  // IR Remote Commands
  constexpr uint8_t IR_ON = 69;
  constexpr uint8_t IR_OFF = 71;
  constexpr uint8_t IR_BRIGHTEN = 90;
  constexpr uint8_t IR_DIM = 8;
  constexpr uint8_t IR_AUTO = 68;
  constexpr uint8_t IR_INVALID = 255;
  
  // Timing Constants
  constexpr unsigned long FADING_RATE = 5;
  constexpr unsigned long MANUAL_OVERRIDE_TIMEOUT = 3000; //3 seconds for testing
  constexpr unsigned long MOTION_SCAN_INTERVAL = 100;
  
  // LDR Calibration
  constexpr int MAX_ADC_READING = 4095;
  constexpr float ADC_REF_VOLTAGE = 3.33f;
  constexpr int REF_RESISTANCE = 10000;
  constexpr long LUX_CALC_SCALAR = 2355175;
  constexpr float LUX_CALC_EXPONENT = -1.2109f;
  
  // Prediction Parameters
  constexpr int NUM_SLOTS = 96; 
  constexpr float DECAY_FACTOR = 0.99f; 
  constexpr int THRESHOLD = 2;
  constexpr int LUX_THRESHOLD = 500; 
  
  // Brightness Levels
  constexpr int brightnessIndex[6] = {0, 51, 102, 153, 204, 255}; //pwm values
  
  void initializePins();
}

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
extern float hourOfDay;
extern bool manualOverrideTimer;
extern bool autoToggle;

extern const char* ssid;
extern const char* password;
extern const char* ntpServer;
extern const char* backupNtpServer; 

// GLOBAL STRUCT INSTANCES
extern SystemState state;
extern Timeing timeing;
extern Predict predicted;