// https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/ 

#define LDR_PIN                   4   // LDR connected to analoug input pin
#define MAX_ADC_READING           4095 
#define ADC_REF_VOLTAGE           5.05 // Voltage supply to the voltage divider.. (Measure it)
#define REF_RESISTANCE            10000 
#define LUX_CALC_SCALAR           2355175
#define LUX_CALC_EXPONENT         -1.2109   

int Lux_Value() {
  uint16_t   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  int ldrLux;
 
  ldrRawData = analogRead(LDR_PIN);
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);   // This is the formula that calculates Lux... See lab notes that I havent written yet(20/2/2020)!!!!!

  //Serial.print("LDR Raw Data   : "); Serial.println(ldrRawData);
  //Serial.print("LDR Voltage    : "); Serial.print(ldrVoltage); Serial.println(" volts");
  //Serial.print("LDR Resistance : "); Serial.print(ldrResistance); Serial.println(" Ohms");
  Serial.print("LDR Illuminance: "); Serial.print(ldrLux); Serial.println(" lux");
 
  return(ldrLux);
}



