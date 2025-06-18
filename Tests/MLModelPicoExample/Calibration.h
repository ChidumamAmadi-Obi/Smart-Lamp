#define DEBUG_CALIBRATION 0

#if DEBUG_CALIBRATION == 1 //if debug is 1....
#define debugCal(x) Serial.print(x)
#define debugCalln(x) Serial.println(x)
#else //if debug is not 1.....
#define debugCal(x)
#define debugCalln(x)
#endif //end of the if statment

// https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/ 

#define LDR_PIN                   26  // LDR connected to analoug input pin on the arduino - I used Pin 1
#define MAX_ADC_READING           4095 // already set - leave this alone.
#define ADC_REF_VOLTAGE           5.05  // Voltage supply to the voltage divider.. (Measure it)
#define REF_RESISTANCE            10000  // measure this for best results, Fergus is using a 10K resistor so set this to 10K
#define LUX_CALC_SCALAR           2355175 //from Excell sheet - scalar (10^(y-intercept))  - This Y intecept is found by experimentation using the light box and a lux meter. I need to develope a lab sheet for this.
#define LUX_CALC_EXPONENT         -1.2109   // From Excell sheet - Slope of plot - from Excel

#define TRIG_PIN 17
#define ECHO_PIN 16
#define SOUND_SPEED 0.034

float prevDistanceCm = 0;
unsigned long prevMotionScan = 0;
unsigned long currentMillis;

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
  if ( currentMillis - prevMotionScan >= 100) {
    prevMotionScan = currentMillis;

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
  }
    return motionSensorStatus;
}


