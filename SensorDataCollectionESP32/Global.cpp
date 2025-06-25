#include "Global.h"

unsigned long currentMillis;

// PIN CONFIGURATION
const uint8_t LDR_PIN = 4;
const uint8_t LAMP_PIN = 27;
const uint8_t IR_PIN = 23;
const uint8_t PIR_MOTION_PIN = 19;
const uint8_t LED_BUILTIN = 2;


// SYSTEM STATE
SystemState state;
Timeing timeing;