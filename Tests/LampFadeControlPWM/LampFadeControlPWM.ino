#include <stdio.h>

#define DEBUG 1

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#define LAMP_PIN 20

int brightness_number = 0;
bool fading_in = true; 
bool fading_out = false;
int fading_rate = 5;

void setup() {
  Serial.begin(115200);
  pinMode(LAMP_PIN, OUTPUT);
}

void loop() {
  lampFade(fading_in);  // Handle fading in and out
  lampFade(fading_out);
}

void lampFade(bool fading_status) {
  if (fading_status == true) {
    for (brightness_number = 0 ; brightness_number <=255 ; brightness_number++){
      analogWrite(LAMP_PIN, brightness_number);
      debug("Lamp Brightness: ");
      debug(brightness_number);
      debugln("/255");
      delay(fading_rate);
    }
  } 
  else {
    for (brightness_number = 255 ; brightness_number >=0 ; brightness_number--){
      analogWrite(LAMP_PIN, brightness_number);
      debug("Lamp Brightness: ");
      debug(brightness_number);
      debugln("/255");
      delay(fading_rate);
    }
  }
}


