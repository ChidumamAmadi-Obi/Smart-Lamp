#define DEBUG_LOGIC 1

#if DEBUG_LOGIC == 1 //if debug is 1....
#define debugLogic(x) Serial.print(x)
#define debugLogicln(x) Serial.println(x)
#else //if debug is not 1.....
#define debugLogic(x)
#define debugLogicln(x)
#endif //end of the if statment

#define SWITCH_PIN 15
#define LAMP_PIN 20
#define BUTTON_PLUS 13
#define BUTTON_MINUS 14

bool predicted_lamp_status = 0;

bool manual_override = false;
unsigned long prevManualToggle = 0;

static bool last_lamp_status = false; // Track previous state for transitions
bool lampStatus = false;

const int fading_rate = 5;
bool isFading = false;

const int brightness_state[6] {0, 51 , 102 , 153 , 204 , 255 }; // array of pwm brightnesses
int last_brightness_no;
int brightness_no = 0; // brightness number 0-5

void manualOverrideTimeout() {
  currentMillis = millis();
  if (currentMillis - prevManualToggle >= 3000){ // if its time for automatic light control...
    manual_override = false;
  }
}

void lampFade(bool fading_status, int starting_brightness, int final_brightness) { //fades lamp in and out using pwm
  isFading = true;
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
  isFading = false;
}

int brightnessSelect(int predicted_brightness) { 
  currentMillis = millis();
  if (lampStatus == true){
    // manual toggle
    if (digitalRead(BUTTON_PLUS) == 1) {
      prevManualToggle = currentMillis;
      last_brightness_no = brightness_no;
      brightness_no = min(brightness_no + 1, 5);
      manual_override = true;
      debugLogic(" MANUAL: Brightness increased ");
    }
    else if (digitalRead(BUTTON_MINUS) == 1) {
      prevManualToggle = currentMillis;
      last_brightness_no = brightness_no;
      brightness_no = max(brightness_no - 1, 0);
      manual_override = true;
      debugLogic(" MANUAL: Brightness increased ");
    }

    // manual override timeout
    manualOverrideTimeout();

    // auto toggle
    if (manual_override == false) {
      last_brightness_no = brightness_no;
      brightness_no = predicted_brightness;
    }
  }
  debugLogic(" Brightness: "); debugLogic(brightness_no); debugLogic(" , "); debugLogicln(predicted_brightness);
  return brightness_no;
}


void toggleLamp(bool predicted_lamp_status){
  currentMillis = millis();

  bool manual_override_switch = digitalRead(SWITCH_PIN);

  //MANUAL  override
  if ((manual_override_switch == HIGH) && (!isFading)) {
    manual_override = true;
    lampStatus = !lampStatus;  // Toggle state
    prevManualToggle = currentMillis;
    debugLogic(lampStatus ? " MANUAL: ON  " : "MANUAL: OFF  ");
    delay(100);  // Simple debounce (replace with millis() later)
  }

  manualOverrideTimeout();

  //Auto toggling on
  if ((manual_override == false) && (!isFading) ) { // 
    if ( predicted_lamp_status == false ) {
      lampStatus = false;
      // debugLogicln(" ML CMD: Turning Lamp off  ");
    }
    else if (( predicted_lamp_status == true )) {
      lampStatus = true;
      // debugLogicln(" ML CMD: Turning Lamp on  ");
    }
  }

  if ( lampStatus != last_lamp_status ) { // handle fading between lamp state changes
    // State changed - apply fading
    if (lampStatus == true) {
      lampFade(true,0,255); // Fade in
    } else {
      lampFade(false,255,0); // Fade out
    }
    last_lamp_status = lampStatus;
  } 

  else if ( brightness_no != last_brightness_no ) {
    if (brightness_no > last_brightness_no) {
      lampFade(true,brightness_state[last_brightness_no],brightness_state[brightness_no]); // Fade in
    } else {
      lampFade(false,brightness_state[last_brightness_no],brightness_state[brightness_no]); // Fade out
    }
    last_brightness_no = brightness_no;
    }
  else {


    if ( (lampStatus == true)  ) {
        analogWrite(LAMP_PIN,brightness_state[brightness_no]);
    } else {
        //turn off lamp
        analogWrite(LAMP_PIN,0);
    }
  }
}
