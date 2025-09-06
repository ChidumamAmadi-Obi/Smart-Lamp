#define LDR_PIN                   4   // LDR connected to analoug input pin
#define MAX_ADC_READING           4095 
#define ADC_REF_VOLTAGE           3.33f // Voltage supply to the voltage divider.. (Measure it)
#define REF_RESISTANCE            10000 
#define LUX_CALC_SCALAR           2355175
#define LUX_CALC_EXPONENT         -1.2109   
#define MAX_LDR_READING           70000
#define LUX_EMA_ALPHA 0.3

static float emaLux = 0;
static bool emaInitialized = false;

float exponentialMovingAverage(float newVal) {
  if (!emaInitialized) {
    emaLux = newVal;
    emaInitialized = true;
    return newVal;
  }
  emaLux = (LUX_EMA_ALPHA * newVal) + ((1 - LUX_EMA_ALPHA) * emaLux);
  return emaLux;
}
float Lux_Value() { // returns inf in bright light?
  uint16_t   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  float rawLdrLux;
  float filteredLdrLux;
 
  ldrRawData = analogRead(LDR_PIN);
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  rawLdrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);  

  if (rawLdrLux > MAX_LDR_READING) rawLdrLux = MAX_LDR_READING;

  filteredLdrLux = exponentialMovingAverage(rawLdrLux);
  
  //Serial.print("LDR Raw Data   : "); Serial.println(ldrRawData);
  //Serial.print("LDR Voltage    : "); Serial.print(ldrVoltage); Serial.println(" volts");
  //Serial.print("LDR Resistance : "); Serial.print(ldrResistance); Serial.println(" Ohms");
  Serial.print("LDR unfiltered :"); Serial.print(rawLdrLux);
  Serial.print(" / LDR filtered   :"); Serial.println(filteredLdrLux);
  
  return(filteredLdrLux);
}






