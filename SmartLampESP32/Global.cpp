#include "Global.h"

unsigned long currentMillis;

// PIN CONFIGURATIONS
const uint8_t LDR_PIN = 4;
const uint8_t LAMP_PIN = 27;
const uint8_t IR_PIN = 23;
const uint8_t PIR_MOTION_PIN = 19;
const uint8_t LED_BUILTIN = 2;

// VALID IR REMOTE COMMANDS
const uint8_t IR_ON = 69;
const uint8_t IR_OFF = 71;
const uint8_t IR_BRIGHTEN = 90;
const uint8_t IR_DIM = 8;
const uint8_t IR_INVALID = -1;


SystemState state;
Timeing timeing;
Predict predicted;
Model model;