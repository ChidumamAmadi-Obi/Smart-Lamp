//using the hr-s04 as a motion sensor
//esp32 

#include <stdio.h>
#include "Calibration.h"

#define TRIG_PIN 17
#define ECHO_PIN 16

void setup() {
Serial.begin(115200);

}

void loop() {
  motionSensorHR_S04();
}
