#ifndef CALIBRATION
#define CALIBRATION

#include "Config.h"

#if DEBUG_CALIBRATION
#define debugCal(x) Serial.print(x)
#define debugCalln(x) Serial.println(x)
#else 
#define debugCal(x)
#define debugCalln(x)
#endif

static float emaLux = 0;
static bool emaInitialized = false;
static bool motionSensorStatus = false;

float exponentialMovingAverage(float newVal) {
  if (!emaInitialized) {
    emaLux = newVal;
    emaInitialized = true;
    return newVal;
  }
  emaLux = (Config::LUX_EMA_ALPHA * newVal) + ((1 - Config::LUX_EMA_ALPHA) * emaLux);
  return emaLux;
}
float Lux_Value() { // calculates Lux value

  int ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float rawLdrLux;
  float filteredLdrLux;
 
  ldrRawData = analogRead(Config::LDR_PIN);
  resistorVoltage = float(ldrRawData) / Config::MAX_ADC_READING * Config::ADC_REF_VOLTAGE;
  ldrVoltage = Config::ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * Config::REF_RESISTANCE;
  rawLdrLux = Config::LUX_CALC_SCALAR * pow(ldrResistance, Config::LUX_CALC_EXPONENT);

  if (rawLdrLux > Config::MAX_LDR_READING) rawLdrLux = Config::MAX_LDR_READING;

  filteredLdrLux = exponentialMovingAverage(rawLdrLux);

  //debugCal("LDR Raw Data   : "); debugCalln(ldrRawData);
  //debugCal("LDR Voltage    : "); debugCal(ldrVoltage); debugCalln(" volts");
  //debugCal("LDR Resistance : "); debugCal(ldrResistance); debugCalln(" Ohms");
  debugCal("LDR Illuminance: "); debugCal(filteredLdrLux); debugCal(" lux  ");
 
  return(filteredLdrLux);
}
bool PIRmotionSensor(){ // senses motion with a PIR sensor
  timeing.prevMotionScan = currentMillis;
  motionSensorStatus = digitalRead(Config::PIR_MOTION_PIN);

  if ( motionSensorStatus ){
    debugCalln("!!! Movement detected !!!");
    timeing.prevRoomOccupation = millis();
  } else {
    debugCalln("No Movement detected!");
  }
  return motionSensorStatus;
}
 
#endif



