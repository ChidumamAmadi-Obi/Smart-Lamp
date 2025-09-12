#ifndef LOGIC
#define LOGIC

#include "Config.h"

#if DEBUG_LOGIC
#define debugLogic(x) Serial.print(x)
#define debugLogicln(x) Serial.println(x)
#else 
#define debugLogic(x)
#define debugLogicln(x)
#endif 

void manualOverrideTimeout() {
  currentMillis = millis();
  if (currentMillis - timeing.prevManualToggle >= Config::MANUAL_OVERRIDE_TIMEOUT){
    manualOverrideTimer = false;
  }
}

void handleAutoToggle() {
  if (IR_command == Config::IR_AUTO) {
    autoToggle = !autoToggle;
    delay(100);
    digitalWrite(Config::AUTO_LED_PIN, autoToggle ? 1 : 0);
    debugLogic("AUTO TOGGLE ");
    debugLogicln(autoToggle ? "ACTIVATED" : "DE-ACTIVATED");
  }
}

void lampFade(bool fading_status, int starting_brightness, int final_brightness) { 
  state.isFading = true;
  int fade_number;

  if (fading_status == true) {
    for (fade_number = starting_brightness ; fade_number <= final_brightness ; fade_number++){
      analogWrite(Config::LAMP_PIN, fade_number);
      // debugLogic(" Lamp Brightness: "); debugLogic(fade_number); debugLogicln("/255 ");
      delay(Config::FADING_RATE);
    }
  } 
  else {
    for (fade_number = starting_brightness ; fade_number >= final_brightness ; fade_number--){
      analogWrite(Config::LAMP_PIN, fade_number);
      // debugLogic(" Lamp Brightness: "); debugLogic(fade_number); debugLogicln("/255 ");
      delay(Config::FADING_RATE);
    }
  }
  state.isFading = false;
}

void lampToggle(bool stateMachine){
  if (!stateMachine) { // going from off to on
    state.currentLampStatus = true;
    state.currentBrightnessNumber = 3;
    lampFade(true,0,Config::brightnessIndex[state.currentBrightnessNumber]);
  } else { // going from on to off
    lampFade(false,Config::brightnessIndex[state.currentBrightnessNumber],0);
    state.currentLampStatus = false;
  }
}

uint8_t brightnessSelect() { 
    // MANUAL TOGGLE
    if (IR_command == Config::IR_BRIGHTEN) {
      manualOverrideTimer = true;
      timeing.prevManualToggle = currentMillis;
      state.currentBrightnessNumber = min(state.currentBrightnessNumber + 1, 5);
      debugLogicln(" MANUAL: Brightness increased ");
      delay(50);
    }
    else if (IR_command == Config::IR_DIM) {
      manualOverrideTimer = true;
      timeing.prevManualToggle = currentMillis;
      state.currentBrightnessNumber = max(state.currentBrightnessNumber - 1, 1);
      debugLogicln(" MANUAL: Brightness decreased to "); debugLogic(state.currentBrightnessNumber); debugLogicln("/5");
      delay(50);
    }
    manualOverrideTimeout();

    // AUTO toggle
    if ( (autoToggle) && (manualOverrideTimer == false) && (state.currentBrightnessNumber != predicted.brightness) && (state.currentLampStatus == true)) {
      state.currentBrightnessNumber = predicted.brightness; //changes current brightness to predicted no.
      debugLogic(" AUTO: Brightness adjusted to "); debugLogic(state.currentBrightnessNumber); debugLogicln("/5");
    }
  return state.currentBrightnessNumber;
} 

void lampStateMachine() {
  handleAutoToggle();

  if ( !state.currentLampStatus ) {
    state.currentBrightnessNumber = 0;
    analogWrite(Config::LAMP_PIN,0);

    if (IR_command == Config::IR_ON) {
      lampToggle(state.currentLampStatus);
      manualOverrideTimer = true;
      timeing.prevManualToggle = currentMillis;
      debugLogicln(" MANUAL: Turning Lamp on... ");
    }
    manualOverrideTimeout();
    if ((autoToggle) && (manualOverrideTimer == false) && (predicted.lampStatus == true)){ // auto
        lampToggle(state.currentLampStatus);
        debugLogicln(" AUTO: Turning Lamp on... ");
      }

    state.lastBrightnessNumber = state.currentBrightnessNumber;
  }
  
  if ( state.currentLampStatus ) { // handle partial lamp fade
    state.currentBrightnessNumber = brightnessSelect();

    if ( state.currentBrightnessNumber != state.lastBrightnessNumber) { // handles partial lamp fade in/out
      if ( state.currentBrightnessNumber > state.lastBrightnessNumber ){
        lampFade(true, Config::brightnessIndex[state.lastBrightnessNumber]  , Config::brightnessIndex[state.currentBrightnessNumber]);
      } else {
        lampFade(false, Config::brightnessIndex[state.lastBrightnessNumber]  , Config::brightnessIndex[state.currentBrightnessNumber]);
      }
      state.lastBrightnessNumber = state.currentBrightnessNumber;
    }
    
  analogWrite(Config::LAMP_PIN,Config::brightnessIndex[state.currentBrightnessNumber]);

  if (IR_command == Config::IR_OFF) {
    lampToggle(state.currentLampStatus);
    manualOverrideTimer = true;
    timeing.prevManualToggle = currentMillis;
    debugLogicln(" MANUAL: Turning Lamp off... ");
  }
  manualOverrideTimeout();
  if ( (autoToggle) && (manualOverrideTimer == false) && (predicted.lampStatus == false) ){
    lampToggle(state.currentLampStatus);
    debugLogicln(" AUTO: Turning Lamp off... ");
    }
  }

  // debugLogic(" Lamp Status: ");    debugLogic(state.currentLampStatus);
  // debugLogic(" Brightness lvl: "); debugLogicln(state.currentBrightnessNumber);
  IR_command = 0; //reset ir command 
}

#endif