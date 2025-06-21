#include "Global.h"

unsigned long currentMillis;

// PIN CONFIGURATION
const uint8_t LDR_PIN = 26;
const uint8_t SWITCH_PIN = 15;
const uint8_t LAMP_PIN = 20;
const uint8_t BUTTON_PLUS = 13;
const uint8_t BUTTON_MINUS = 14;
const uint8_t TRIG_PIN = 17;
const uint8_t ECHO_PIN = 16;

// SYSTEM STATE
SystemState state;
Timeing timeing;