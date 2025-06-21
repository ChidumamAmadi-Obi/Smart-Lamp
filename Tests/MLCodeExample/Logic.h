#include "Global.h"

#define DEBUG_LOGIC 1

  #if DEBUG_LOGIC == 1 //if debug is 1....
  #define debugLogic(x) Serial.print(x)
  #define debugLogicln(x) Serial.println(x)
  #else //if debug is not 1.....
  #define debugLogic(x)
  #define debugLogicln(x)
  #endif //end of the if statment

#define FADING_RATE  5

extern const uint8_t  SWITCH_PIN;
extern const uint8_t  LAMP_PIN;
extern const uint8_t  BUTTON_PLUS;
extern const uint8_t  BUTTON_MINUS;

const int brightnessIndex[6] = {0, 51, 102, 153, 204, 255};

static bool manualOverride = false;

void manualOverrideTimeout() {
  currentMillis = millis();
  if (currentMillis - timeing.prevManualToggle >= 3000){
    manualOverride = false;
  }
}

// fades lamp in and out using pwm
void lampFade(bool fading_status, int starting_brightness, int final_brightness) { 
  state.isFading = true;
  int fade_number;

  if (fading_status == true) {
    for (fade_number = starting_brightness ; fade_number <= final_brightness ; fade_number++){
      analogWrite(LAMP_PIN, fade_number);
      debugLogic(" Lamp Brightness: ");
      debugLogic(fade_number);
      debugLogicln("/255 ");
      delay(FADING_RATE);
    }
  } 
  else {
    for (fade_number = starting_brightness ; fade_number >= final_brightness ; fade_number--){
      analogWrite(LAMP_PIN, fade_number);
      debugLogic(" Lamp Brightness: ");
      debugLogic(fade_number);
      debugLogicln("/255 ");
      delay(FADING_RATE);
    }
  }
  state.isFading = false;
}

void lampToggle(bool stateMachine){
  if (stateMachine == false) { // going from off to on
    state.currentLampStatus = true;
    state.currentBrightnessNumber = 3;
    lampFade(true,0,brightnessIndex[state.currentBrightnessNumber]);
  } else { // going from on to off
    lampFade(false,brightnessIndex[state.currentBrightnessNumber],0);
    state.currentLampStatus = false;
  }
}

// handles change in brightness
int brightnessSelect() { 

    // MANUAL TOGGLE
    if (digitalRead(BUTTON_PLUS) == 1) {
      manualOverride = true;
      timeing.prevManualToggle = currentMillis;
      state.currentBrightnessNumber = min(state.currentBrightnessNumber + 1, 5);
      debugLogic(" MANUAL: Brightness increased ");
      delay(50);
    }
    else if (digitalRead(BUTTON_MINUS) == 1) {
      manualOverride = true;
      timeing.prevManualToggle = currentMillis;
      state.currentBrightnessNumber = max(state.currentBrightnessNumber - 1, 0);
      debugLogic(" MANUAL: Brightness decreased ");
      delay(50);
    }

    manualOverrideTimeout();

    // AUTO toggle
    if ((manualOverride == false) && (state.currentBrightnessNumber != predicted.brightness) && (state.currentLampStatus == true)) {
      state.currentBrightnessNumber = predicted.brightness; //changes current brightness to predicted no.
      debugLogicln(" AUTO: Brightness adjusted ");
    }
  return state.currentBrightnessNumber;
} 

// handles lamp control
void lampStateMachine() {
  bool lamp_switch = digitalRead(SWITCH_PIN);
  bool plus_button = digitalRead(BUTTON_PLUS);
  bool minus_button = digitalRead(BUTTON_MINUS); 

  if ( state.currentLampStatus == false ) { //lamp is off and brightness is 0
    state.currentBrightnessNumber = 0;
    analogWrite(LAMP_PIN,0);

    if ( lamp_switch == HIGH ){ // manual
      lampToggle(state.currentLampStatus);
      manualOverride = true;
      timeing.prevManualToggle = currentMillis;
      debugLogicln(" MANUAL: Turning Lamp on... ");
    }
    manualOverrideTimeout();
    if ( (manualOverride == false) && (predicted.lampStatus == true) ){ // auto
        lampToggle(state.currentLampStatus);
        debugLogicln(" AUTO: Turning Lamp on... ");
      }

    state.lastBrightnessNumber = state.currentBrightnessNumber;
  }
  
  if ( state.currentLampStatus == true ) { // handle partial lamp fade
    state.currentBrightnessNumber = brightnessSelect();

    if ( state.currentBrightnessNumber != state.lastBrightnessNumber) { // handles partial lamp fade in/out
      if ( state.currentBrightnessNumber > state.lastBrightnessNumber ){
        lampFade(true, brightnessIndex[state.lastBrightnessNumber]  , brightnessIndex[state.currentBrightnessNumber]);
      } else {
        lampFade(false, brightnessIndex[state.lastBrightnessNumber]  , brightnessIndex[state.currentBrightnessNumber]);
      }
      state.lastBrightnessNumber = state.currentBrightnessNumber;
    }
    
    analogWrite(LAMP_PIN,brightnessIndex[state.currentBrightnessNumber]);

    if (!state.isFading) {
      if ( lamp_switch == HIGH ){
        lampToggle(state.currentLampStatus);
        manualOverride = true;
        timeing.prevManualToggle = currentMillis;
        debugLogicln(" MANUAL: Turning Lamp off... ");
      }
      manualOverrideTimeout();
      if ( (manualOverride == false) && (predicted.lampStatus == false) ){
        lampToggle(state.currentLampStatus);
        debugLogicln(" AUTO: Turning Lamp off... ");
      }
    }
  }
  debugLogic(" Lamp Status: ");    debugLogic(state.currentLampStatus);
  debugLogic(" Brightness lvl: "); debugLogicln(state.currentBrightnessNumber);
}