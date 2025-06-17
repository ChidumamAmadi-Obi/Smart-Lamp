// https://www.allaboutcircuits.com/projects/design-a-luxmeter-using-a-light-dependent-resistor/ 

// test code for gathering data through LDR
// esp 32

#include <stdio.h>
#include "Calibration.h"

void setup() 
{
  Serial.begin(115200);
  analogReadResolution(12); // initialize ADC resolution
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop() {    
Lux_Value(); 
delay(100);
}

