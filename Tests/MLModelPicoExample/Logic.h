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
int predicted_brightness = 0; 

bool manual_override = false;
unsigned long prevManualToggle = 0;

static bool last_lamp_status = false; // Track previous state for transitions
bool lampStatus = false;

int fading_rate = 5;
bool isFading = false;

int brightness_state[6] {0, 51 , 102 , 153 , 204 , 255 }; // array of pwm brightnesses
int brightness_no = 0; // brightness number 0-5


int brightnessSelect(){ // allows user to turn brightness of lamp up and down with 2 buttons
  currentMillis = millis();

    // User can turn up or down brightness
  if (lampStatus == true) {
    if (digitalRead(BUTTON_PLUS) == 1) {
      brightness_no++;
      if (brightness_no > 5) { brightness_no = 5; debugLogic("Maximum Brightness Reached"); }
        debugLogicln("Turning Brighness UP ");
        delay(50);
      }

  if (digitalRead(BUTTON_MINUS) == 1) {
    brightness_no--;
    if (brightness_no < 1) { brightness_no = 1; debugLogic("Minimum Brightness Reached"); }
      debugLogicln("Turning Brighness DOWN ");
      delay(50);
    } 
  }

  

  analogWrite(LAMP_PIN,brightness_state[brightness_no]); 
  return brightness_no;
}

void lampFade(bool fading_status) { //fades lamp in and out using pwm
  isFading = true;
  int fade_number;

  if (fading_status == true) {
    for (fade_number = 0 ; fade_number <=255 ; fade_number++){
      analogWrite(LAMP_PIN, fade_number);
      debugLogic(" Lamp Brightness: ");
      debugLogic(fade_number);
      debugLogicln("/255 ");
      delay(fading_rate);
      if (digitalRead(SWITCH_PIN) == HIGH) break; // Check for manual override during fade
    }
  } 
  else {
    for (fade_number = 255 ; fade_number >=0 ; fade_number--){
      analogWrite(LAMP_PIN, fade_number);
      debugLogic(" Lamp Brightness: ");
      debugLogic(fade_number);
      debugLogicln("/255 ");
      delay(fading_rate);
      if (digitalRead(SWITCH_PIN) == HIGH) break; // Check for manual override during fade
    }
  }
  isFading = false;
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

  if (currentMillis - prevManualToggle >= 3000){ // if its time for automatic light control...
    manual_override = false;
    // debugLogic("MANUAL toggle false     ");
  }

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

  if (lampStatus != last_lamp_status) { // handle fading between lamp state changes
    // State changed - apply fading
    if (lampStatus == true) {
      lampFade(true); // Fade in
    } else {
      lampFade(false); // Fade out
    }
    last_lamp_status = lampStatus;
  } 
  else {


    if ( (lampStatus == true) && (!isFading) ) {
      if (manual_override == true) {
        analogWrite(LAMP_PIN,255);
      } else {
        // turn on lamp
        analogWrite(LAMP_PIN,brightness_state[predicted_brightness]);
      } 
    } else {
        //turn off lamp
        analogWrite(LAMP_PIN,0);
    }
  }
}
