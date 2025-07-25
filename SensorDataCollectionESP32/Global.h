#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>

// Pin Configuration (extern declarations)
extern const uint8_t LDR_PIN;
extern const uint8_t LAMP_PIN;
extern const uint8_t IR_PIN;
extern const uint8_t PIR_MOTION_PIN;
extern const uint8_t LED_BUILTIN;

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
    unsigned long prevLEDFlash = 0;
    unsigned long prevLog = 0;
};

extern unsigned long currentMillis;

// struct instance
extern SystemState state;
extern Timeing timeing;

#endif