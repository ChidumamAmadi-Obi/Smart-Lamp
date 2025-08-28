#include "Global.h"

#define DEBUG_LOGIC 0

  #if DEBUG_LOGIC == 1 
  #define debugLogic(x) Serial.print(x)
  #define debugLogicln(x) Serial.println(x)
  #else 
  #define debugLogic(x)
  #define debugLogicln(x)
  #endif 

#define FADING_RATE  5 // 5ms intervals
#define MANUAL_OVERRIDE_TIMEOUT 180000 // 3 minutes 

extern const uint8_t  LAMP_PIN;

extern const uint8_t IR_ON;
extern const uint8_t IR_OFF;
extern const uint8_t IR_BRIGHTEN;
extern const uint8_t IR_DIM;
extern const uint8_t IR_AUTO;
extern const uint8_t IR_INVALID;

const int brightnessIndex[6] = {0, 51, 102, 153, 204, 255};
static uint8_t IR_command = 0;
static bool manualOverride = false;
static bool autoToggle = true;

void manualOverrideTimeout() {
  currentMillis = millis();
  if (currentMillis - timeing.prevManualToggle >= MANUAL_OVERRIDE_TIMEOUT){
    manualOverride = false;
  }
}

void autoToggle_() {
  if (IR_command == IR_AUTO) {
    autoToggle = !autoToggle;
    debugLogicln("AUTO TOGGLE ACVITATED");
  }
}

// fades lamp in and out using pwm
void lampFade(bool fading_status, int starting_brightness, int final_brightness) { 
  state.isFading = true;
  int fade_number;

  if (fading_status == true) {
    for (fade_number = starting_brightness ; fade_number <= final_brightness ; fade_number++){
      analogWrite(LAMP_PIN, fade_number);
      debugLogic(" Lamp Brightness: "); debugLogic(fade_number); debugLogicln("/255 ");
      delay(FADING_RATE);
    }
  } 
  else {
    for (fade_number = starting_brightness ; fade_number >= final_brightness ; fade_number--){
      analogWrite(LAMP_PIN, fade_number);
      debugLogic(" Lamp Brightness: "); debugLogic(fade_number); debugLogicln("/255 ");
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
    if (IR_command == IR_BRIGHTEN) {
      manualOverride = true;
      timeing.prevManualToggle = currentMillis;
      state.currentBrightnessNumber = min(state.currentBrightnessNumber + 1, 5);
      debugLogic(" MANUAL: Brightness increased ");
      delay(50);
    }
    else if (IR_command == IR_DIM) {
      manualOverride = true;
      timeing.prevManualToggle = currentMillis;
      state.currentBrightnessNumber = max(state.currentBrightnessNumber - 1, 1);
      debugLogic(" MANUAL: Brightness decreased ");
      delay(50);
    }
    manualOverrideTimeout();

    // AUTO toggle
    if ( (autoToggle) && (manualOverride == false) && (state.currentBrightnessNumber != predicted.brightness) && (state.currentLampStatus == true)) {
      state.currentBrightnessNumber = predicted.brightness; //changes current brightness to predicted no.
      debugLogicln(" AUTO: Brightness adjusted ");
    }
  return state.currentBrightnessNumber;
} 

// handles lamp control
void lampStateMachine() {
  if (IrReceiver.decode()) { // collects command from ir remote
    IR_command = IrReceiver.decodedIRData.command;
    IrReceiver.resume();
  }

  autoToggle_();

  if ( state.currentLampStatus == false ) { //lamp is off and brightness is 0
    state.currentBrightnessNumber = 0;
    analogWrite(LAMP_PIN,0);

    if (IR_command == IR_ON) {
      lampToggle(state.currentLampStatus);
      manualOverride = true;
      timeing.prevManualToggle = currentMillis;
      debugLogicln(" MANUAL: Turning Lamp on... ");
    }
    manualOverrideTimeout();
    if ( (autoToggle) && (manualOverride == false) && (predicted.lampStatus == true) ){ // auto
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

  if (IR_command == IR_OFF) {
    lampToggle(state.currentLampStatus);
    manualOverride = true;
    timeing.prevManualToggle = currentMillis;
    debugLogicln(" MANUAL: Turning Lamp off... ");
  }
  manualOverrideTimeout();
  if ( (autoToggle) && (manualOverride == false) && (predicted.lampStatus == false) ){
    lampToggle(state.currentLampStatus);
    debugLogicln(" AUTO: Turning Lamp off... ");
    }
  }

  debugLogic(" Lamp Status: ");    debugLogic(state.currentLampStatus);
  debugLogic(" Brightness lvl: "); debugLogicln(state.currentBrightnessNumber);
  // debugLogicln(manualOverride);
  // debugLogicln(IR_command);
  IR_command = 0; //reset ir command 

}
