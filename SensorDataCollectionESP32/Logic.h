#include "Global.h"

#define DEBUG_LOGIC 0

  #if DEBUG_LOGIC == 1 //if debug is 1....
  #define debugLogic(x) Serial.print(x)
  #define debugLogicln(x) Serial.println(x)
  #else //if debug is not 1.....
  #define debugLogic(x)
  #define debugLogicln(x)
  #endif //end of the if statment

extern const uint8_t  LAMP_PIN;

const int fading_rate = 5;
const int brightnessIndex[6] = {0, 51, 102, 153, 204, 255};
static uint16_t IR_command = 0;

uint16_t handleIRremote() { // initialize IR_remote
  if (IrReceiver.decode()) {
    IR_command = IrReceiver.decodedIRData.command;
    IrReceiver.resume();
  }
  return IR_command;
}

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
  if (IrReceiver.decode()) {
    IR_command = IrReceiver.decodedIRData.command;
    if (IR_command == 90) {
      state.currentBrightnessNumber = min(state.currentBrightnessNumber + 1, 5);
      debugLogic(" MANUAL: Brightness increased ");
      delay(50);
    }
    else if (IR_command == 8) {
      state.currentBrightnessNumber = max(state.currentBrightnessNumber - 1, 1);
      debugLogic(" MANUAL: Brightness decreased ");
      delay(50);
    }
    IrReceiver.resume();
  }
  return state.currentBrightnessNumber;
} 

// Handles lamp control
int lampStateMachine() {
  if ( state.currentLampStatus == false ) { //lamp is off and brightness is 0
    state.currentBrightnessNumber = 0;
    analogWrite(LAMP_PIN,0);

      if ( handleIRremote() == 69 ){
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

      if ( handleIRremote() == 71 ){
        state.currentLampStatus = false;
        lampFade(false,brightnessIndex[state.currentBrightnessNumber],0);
      }
  }

  debugLogic(" Lamp Status: "); debugLogic(state.currentLampStatus);
  debugLogic(" Brightness lvl: "); debugLogicln(state.currentBrightnessNumber);
  debugLogicln(IR_command);

  return state.currentBrightnessNumber;
}