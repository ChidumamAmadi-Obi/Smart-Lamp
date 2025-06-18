// Global.cpp
#include "Global.h"

#define DEBUG_LOGIC 1

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
const int brightness_state[6] = {0, 51, 102, 153, 204, 255};

int last_brightness_no;
int brightness_no = 0; // brightness number 0-5

void manualOverrideTimeout() { // at a certain time automatic toggling of the lamp is allowed
  currentMillis = millis();
  if (currentMillis - timeing.prevManualToggle >= 3000){
    state.manualOverride = false;
  }
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

int brightnessSelect(int predicted_brightness) { // manual and automatic brightness selection
  currentMillis = millis();
  if (state.currentLampStatus == true){

    // MANUAL toggle
    if (digitalRead(BUTTON_PLUS) == 1) {
      timeing.prevManualToggle = currentMillis;
      last_brightness_no = brightness_no;
      brightness_no = min(brightness_no + 1, 5);
      state.manualOverride = true;
      debugLogic(" MANUAL: Brightness increased ");
    }
    else if (digitalRead(BUTTON_MINUS) == 1) {
      timeing.prevManualToggle = currentMillis;
      last_brightness_no = brightness_no;
      brightness_no = max(brightness_no - 1, 0);
      state.manualOverride = true;
      debugLogic(" MANUAL: Brightness increased ");
    }

    manualOverrideTimeout();

    // AUTO toggle
    if (state.manualOverride == false) {
      last_brightness_no = brightness_no;
      brightness_no = predicted_brightness;
    }
  } 
  debugLogic(" Brightness No: "); debugLogic(brightness_no); debugLogic(" Predicted Brightness No:"); debugLogicln(predicted_brightness);
  return brightness_no;
}

void toggleLamp(bool predicted_lamp_status){ // manual and automatic toggling of lamp
  currentMillis = millis();

  bool manual_override_switch = digitalRead(SWITCH_PIN);

  //MANUAL  override
  if ((manual_override_switch == HIGH) && (!state.isFading)) {
    state.manualOverride = true;
    state.currentLampStatus = !state.currentLampStatus;  // Toggle state
    timeing.prevManualToggle = currentMillis;
    debugLogic(state.currentLampStatus ? " MANUAL: ON  " : "MANUAL: OFF  ");
    delay(100);  // Simple debounce (replace with millis() later)
  }

  manualOverrideTimeout();

  //Auto toggling on
  if ((state.manualOverride == false) && (!state.isFading) ) { // 
    if ( predicted_lamp_status == false ) {
      state.currentLampStatus = false;
      debugLogic(" ML CMD: Lamp off  ");
    }
    else if (( predicted_lamp_status == true )) {
      state.currentLampStatus = true;
      debugLogic(" ML CMD: Lamp on  ");
    }
  }

  
  if ( brightness_no != last_brightness_no ) { // handle partial fade
    if (brightness_no > last_brightness_no) {
      lampFade(true,brightness_state[last_brightness_no],brightness_state[brightness_no]); // Fade in
    } else {
      lampFade(false,brightness_state[last_brightness_no],brightness_state[brightness_no]); // Fade out
    }
      last_brightness_no = brightness_no;
    }

  else if ( state.currentLampStatus != state.lastLampStatus ) { // handle full fade
    if (state.currentLampStatus == true) {
      lampFade(true,0,brightness_state[brightness_no]); // Fade in
    } else {
      lampFade(false,brightness_state[brightness_no],0); // Fade out
    }
    state.lastLampStatus = state.currentLampStatus;
  }
}
