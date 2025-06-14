#define DEBUG 0

#if DEBUG == 1 //if debug is 1....
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else //if debug is not 1.....
#define debug(x)
#define debugln(x)
#endif //end of the if statment

// https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/ 

#define LDR_PIN                   26  // LDR connected to analoug input pin on the arduino - I used Pin 1
#define MAX_ADC_READING           4095 // already set - leave this alone.
#define ADC_REF_VOLTAGE           5.05  // Voltage supply to the voltage divider.. (Measure it)
#define REF_RESISTANCE            10000  // measure this for best results, Fergus is using a 10K resistor so set this to 10K
#define LUX_CALC_SCALAR           2355175 //from Excell sheet - scalar (10^(y-intercept))  - This Y intecept is found by experimentation using the light box and a lux meter. I need to develope a lab sheet for this.
#define LUX_CALC_EXPONENT         -1.2109   // From Excell sheet - Slope of plot - from Excel

#define LAMP_PIN 20
#define BUTTON_PLUS 13
#define BUTTON_MINUS 14

#define TRIG_PIN 17
#define ECHO_PIN 16
#define SOUND_SPEED 0.034

#define SWITCH_PIN 15

float prevDistanceCm = 0;
int buttonPress = 0;
int brightness_state[5] { 51 , 102 , 153 , 204 , 255 }; // array of pwm brightnesses
int cnt = 4;

int Lux_Value() { // calculates Lux value
  int   ldrRawData;
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  int ldrLux;
 
  ldrRawData = analogRead(LDR_PIN);
  resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
  ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);

  //debug("LDR Raw Data   : "); Serial.println(ldrRawData);
  //debug("LDR Voltage    : "); Serial.print(ldrVoltage); Serial.println(" volts");
  //debug("LDR Resistance : "); Serial.print(ldrResistance); Serial.println(" Ohms");
  debug("LDR Illuminance: "); debug(ldrLux); debugln(" lux");
 
  return(ldrLux);
}

int brightnessSelect(){ // allows user to turn brightness of lamp up and down with 2 buttons

  int button_plus = digitalRead(BUTTON_PLUS);
  int button_minus = digitalRead(BUTTON_MINUS);

  if (buttonPress == 1) {
    if (digitalRead(BUTTON_PLUS) == 1) {
      cnt++;
      if (cnt > 4) {
        cnt = 4;
        debug("Brightness highest ");
      }
      analogWrite(LAMP_PIN,brightness_state[cnt]);
      debugln("Turning Brighness UP ");
      delay(300);
    }

    if (digitalRead(BUTTON_MINUS) == 1) {
      cnt--;
      if (cnt < 0) {
        cnt = 0;
        debug("Brightness Lowest ");
      }
      analogWrite(LAMP_PIN,brightness_state[cnt]);
      debugln("Turning Brighness DOWN ");
      delay(300);
    } 
  } else { 
    analogWrite(LAMP_PIN,0); 
    }
  
  return cnt;
}

bool motionSensorHR_S04(){ // senses motion with a HR_S04

  bool status;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2); 
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW); 

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distanceCm = duration * 0.0344 / 2;

  if ( ((distanceCm > 1) && (prevDistanceCm > 1)) && (abs(distanceCm - prevDistanceCm) > 3)){
    debugln("!!! Movement detected !!!");
    status = true;
  } 

  else {
    debugln("No Movement detected!");
    status = false;
  }
  delay(100);

  prevDistanceCm = distanceCm;

  return status;
}

bool toggleLight(){ // turns lamp on and off with button
  int brightness_state[5];
  int cnt;

  if (buttonPress == 0) { //if the lamp is off it can be turned on
    if (digitalRead(SWITCH_PIN) == 1) {
      buttonPress++;
      digitalWrite(LAMP_PIN,HIGH);
      cnt = 4;
      debugln("Lamp has been turned on!");
      delay(500);
    }
  }
  if (buttonPress == 1) {
    if(digitalRead(SWITCH_PIN) == 1) {
      buttonPress--;
      digitalWrite(LAMP_PIN,LOW);
      debugln("Lamp has been turned off!");
      delay(500);
    }
  }
  return buttonPress;
}


