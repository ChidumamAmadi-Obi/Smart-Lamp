#include "Global.h"

#define DEBUG_LOGIC 0

  #if DEBUG_LOGIC == 1 //if debug is 1....
  #define debugLogic(x) Serial.print(x)
  #define debugLogicln(x) Serial.println(x)
  #else //if debug is not 1.....
  #define debugLogic(x)
  #define debugLogicln(x)
  #endif //end of the if statment

extern const uint8_t  SWITCH_PIN;
extern const uint8_t  LAMP_PIN;
extern const uint8_t  BUTTON_PLUS;
extern const uint8_t  BUTTON_MINUS;

const int fading_rate = 5;
const int brightnessIndex[6] = {0, 51, 102, 153, 204, 255};

void lampFade(bool fading_status, int starting_brightness, int final_brightness) { //fades lamp in and out using pwm
  state.isFading = true;
  int fade_number;

  if (fading_status == true) {
    for (fade_number = starting_brightness ; fade_number <= final_brightness ; fade_number++){
      analogWrite(LAMP_PIN, fade_number);
      debugLogic(" Lamp Brightness: ");
      debugLogic(fade_number);
      debugLogicln("/255 ");
      delay(fading_rate);
      // if (digitalRead(SWITCH_PIN) == HIGH) break; // Check for manual override during fade
    }
  } 
  else {
    for (fade_number = starting_brightness ; fade_number >= final_brightness ; fade_number--){
      analogWrite(LAMP_PIN, fade_number);
      debugLogic(" Lamp Brightness: ");
      debugLogic(fade_number);
      debugLogicln("/255 ");
      delay(fading_rate);
      // if (digitalRead(SWITCH_PIN) == HIGH) break; // Check for manual override during fade
    }
  }
  state.isFading = false;
}

int brightnessSelect() { 
    if (digitalRead(BUTTON_PLUS) == 1) {
      state.currentBrightnessNumber = min(state.currentBrightnessNumber + 1, 5);
      debugLogic(" MANUAL: Brightness increased ");
      delay(50);
    }
    else if (digitalRead(BUTTON_MINUS) == 1) {
      state.currentBrightnessNumber = max(state.currentBrightnessNumber - 1, 1);
      debugLogic(" MANUAL: Brightness decreased ");
      delay(50);
    }
  return state.currentBrightnessNumber;
} 

// Handles lamp control
int lampStateMachine() {
  bool lamp_switch = digitalRead(SWITCH_PIN);
  bool plus_button = digitalRead(BUTTON_PLUS);
  bool minus_button = digitalRead(BUTTON_MINUS); 

  if ( state.currentLampStatus == false ) { //lamp is off and brightness is 0
    state.currentBrightnessNumber = 0;
    analogWrite(LAMP_PIN,0);

    if ( lamp_switch == HIGH ){
      state.currentLampStatus = true;
      state.currentBrightnessNumber = 3;
      lampFade(true,0,brightnessIndex[state.currentBrightnessNumber]);
      state.lastBrightnessNumber = state.currentBrightnessNumber;
    }
  }
  
  else if ( state.currentLampStatus == true) {
    state.currentBrightnessNumber = brightnessSelect();

    if ( state.currentBrightnessNumber != state.lastBrightnessNumber ) { // handles partial lamp fade in/out
      if ( state.currentBrightnessNumber > state.lastBrightnessNumber ){
        lampFade(true, brightnessIndex[state.lastBrightnessNumber]  , brightnessIndex[state.currentBrightnessNumber]);
      } else {
        lampFade(false, brightnessIndex[state.lastBrightnessNumber]  , brightnessIndex[state.currentBrightnessNumber]);
      }
      state.lastBrightnessNumber = state.currentBrightnessNumber;
    }

    analogWrite(LAMP_PIN,brightnessIndex[state.currentBrightnessNumber]);

      if ( lamp_switch == HIGH ){
      state.currentLampStatus = false;
      lampFade(false,brightnessIndex[state.currentBrightnessNumber],0);
    }
  }

  debugLogic(" Lamp Status: "); debugLogic(state.currentLampStatus);
  debugLogic(" Brightness lvl: "); debugLogicln(state.currentBrightnessNumber);

  return state.currentBrightnessNumber;
}