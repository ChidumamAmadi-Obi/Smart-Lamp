#include "Config.h"

#define DEBUG_CALIBRATION false
#if DEBUG_CALIBRATION == 1 
#define debugCal(x) Serial.print(x)
#define debugCalln(x) Serial.println(x)
#else 
#define debugCal(x)
#define debugCalln(x)
#endif

// https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/ 

static bool motionSensorStatus = false;

int Lux_Value() { // calculates Lux value

  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  int ldrLux;
 
  ldrRawData = analogRead(Config::LDR_PIN);
  resistorVoltage = (float)ldrRawData / Config::MAX_ADC_READING * Config::ADC_REF_VOLTAGE;
  ldrVoltage = Config::ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * Config::REF_RESISTANCE;
  ldrLux = Config::LUX_CALC_SCALAR * pow(ldrResistance, Config::LUX_CALC_EXPONENT);

  //debugCal("LDR Raw Data   : "); debugCalln(ldrRawData);
  //debugCal("LDR Voltage    : "); debugCal(ldrVoltage); debugCalln(" volts");
  //debugCal("LDR Resistance : "); debugCal(ldrResistance); debugCalln(" Ohms");
  debugCal("LDR Illuminance: "); debugCal(ldrLux); debugCal(" lux  ");
 
  return(ldrLux);
}
bool PIRmotionSensor(){ // senses motion with a PIR sensor
  timeing.prevMotionScan = currentMillis;
  motionSensorStatus = digitalRead(Config::PIR_MOTION_PIN);

  if ( motionSensorStatus == HIGH ){
    debugCalln("!!! Movement detected !!!");
  } else {
    debugCalln("No Movement detected!");
  }
  return motionSensorStatus;
}
 



