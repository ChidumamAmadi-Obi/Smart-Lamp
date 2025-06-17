// Lampbrightness control with pushbuttons

#include <stdio.h>

#define DEBUG 1

#if DEBUG == 1 //if debug is 1....
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else //if debug is not 1.....
#define debug(x)
#define debugln(x)
#endif //end of the if statment

#define LAMP_PIN 20
#define BUTTON_PLUS 13
#define BUTTON_MINUS 14

#define LAMP_SWITCH_PIN 15

int brightness_state[6] {0, 51 , 102 , 153 , 204 , 255 }; // array of pwm brightnesses
int brighntess_number; 
bool on_button = LOW;
bool lamp_status = 0;;

void setup() {
  Serial.begin(115200);

  pinMode(LAMP_PIN, OUTPUT);
  pinMode(BUTTON_PLUS, INPUT);
  pinMode(BUTTON_MINUS, INPUT);

  // Start with lamp off (brightness 0)
  analogWrite(LAMP_SWITCH_PIN, brightness_state[brighntess_number]);
}

void loop() {
  int brightness = brightnessSelect();

  debug("Lamp Status: ");
  debug(lamp_status);
  debug(" Brightness: ");
  debugln(brightness);
}

int brightnessSelect(){ // allows user to turn brightness of lamp up and down with 2 buttons

  on_button = digitalRead(LAMP_SWITCH_PIN); // keeps track of the state of the button

  if (on_button == HIGH) {
    lamp_status = !lamp_status; // toggles button
    debugln("Lamp has been toggled!");
    delay(100); //debounce
  }

  if (lamp_status == HIGH) {
    if (digitalRead(BUTTON_PLUS) == 1) {
      brighntess_number++;
      if (brighntess_number > 5) { brighntess_number = 5; debug("Maximum Brightness Reached"); }
      debugln("Turning Brighness UP ");
      delay(50);
      }

    if (digitalRead(BUTTON_MINUS) == 1) {
      brighntess_number--;
      if (brighntess_number < 1) { brighntess_number = 1; debug("Minimum Brightness Reached"); }
      debugln("Turning Brighness DOWN ");
      delay(50);
    } 
  }
  if (lamp_status == HIGH) {
    analogWrite(LAMP_PIN,brightness_state[brighntess_number]);
  } else {
  analogWrite(LAMP_PIN,0);    
  }
  return brighntess_number;
}
