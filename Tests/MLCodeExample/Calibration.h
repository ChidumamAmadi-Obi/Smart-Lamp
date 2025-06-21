#include "Global.h"

#define DEBUG_CALIBRATION 0

  #if DEBUG_CALIBRATION == 1 //if debug is 1....
  #define debugCal(x) Serial.print(x)
  #define debugCalln(x) Serial.println(x)
  #else //if debug is not 1.....
  #define debugCal(x)
  #define debugCalln(x)
  #endif //end of the if statment

// https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/ 

#define MAX_ADC_READING           4095 
#define ADC_REF_VOLTAGE           3.33  
#define REF_RESISTANCE            10000 
#define LUX_CALC_SCALAR           2355175 
#define LUX_CALC_EXPONENT         -1.2109 
#define SOUND_SPEED               0.034

extern const uint8_t LDR_PIN;
extern const uint8_t TRIG_PIN;
extern const uint8_t ECHO_PIN;

float prevDistanceCm = 0;

int Lux_Value() { // calculates Lux value
  currentMillis = millis();

  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  int ldrLux;
 
  ldrRawData = analogRead(LDR_PIN);
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

  //debugCal("LDR Raw Data   : "); debugCalln(ldrRawData);
  //debugCal("LDR Voltage    : "); debugCal(ldrVoltage); debugCalln(" volts");
  //debugCal("LDR Resistance : "); debugCal(ldrResistance); debugCalln(" Ohms");
  debugCal("LDR Illuminance: "); debugCal(ldrLux); debugCal(" lux  ");
 
  return(ldrLux);
}


bool motionSensorHR_S04(){ // senses motion with a HR_S04
  bool motionSensorStatus;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2); 
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    

    long duration = pulseIn(ECHO_PIN, HIGH);
    float distanceCm = duration * 0.0344 / 2;

    if ( ((distanceCm > 1) && (prevDistanceCm > 1)) && (abs(distanceCm - prevDistanceCm) > 3)){
      debugCalln("!!! Movement detected !!!");
      motionSensorStatus = true;
    } 

    else {
      debugCalln("No Movement detected!");
      motionSensorStatus = false;
    }
    delay(100);

  prevDistanceCm = distanceCm;
  return motionSensorStatus;
}
 



