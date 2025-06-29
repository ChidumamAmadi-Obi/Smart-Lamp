#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>

// Pin Configuration (extern declarations)
extern const uint8_t LDR_PIN;
extern const uint8_t SWITCH_PIN;
extern const uint8_t LAMP_PIN;
extern const uint8_t BUTTON_PLUS;
extern const uint8_t BUTTON_MINUS;
extern const uint8_t TRIG_PIN;
extern const uint8_t ECHO_PIN;

// Lamp State Structure
struct SystemState {
    bool isFading = false;         
    bool lastLampStatus = false;  
    bool currentLampStatus = false;    
    int lastBrightnessNumber = false;
    int currentBrightnessNumber = false;  
};

//TIME KEEPING
struct Timeing {
    unsigned long prevMotionScan = 0;
    unsigned long prevLuxScan = 0;
    unsigned long prevLEDFlash = 0;
    unsigned long prevLog = 0;
    unsigned long prevAutoToggle = 0;
    unsigned long prevManualToggle = 0;
};

//MOCK ML MODEL PREDICTIONS
struct Predict {
    bool lampStatus = false;
    int brightness = false;
};

extern unsigned long currentMillis;

// struct instance
extern SystemState state;
extern Timeing timeing;
extern Predict predicted;

#endif