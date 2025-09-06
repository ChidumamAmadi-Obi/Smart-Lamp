#include "Config.h"

#if DEBUG_PREDICTION
#define debugPrediction(x) Serial.print(x)
#define debugPredictionln(x) Serial.println(x)
#else 
#define debugPrediction(x)
#define debugPredictionln(x)
#endif

RTC_DATA_ATTR int8_t activity[Config::NUM_SLOTS] = {0}; // divide 24hrs into 15 minute slots
RTC_DATA_ATTR bool wasMidnightPassed = false;
uint8_t currentSlot = 0;
bool wasLampOn = false;

uint8_t getTimeSlot() { 
    return (hourOfDay * 60.0f) / 15;
}

void logUsage(bool reinforce){
    currentSlot = getTimeSlot();
    if (currentSlot >= 0 && currentSlot < Config::NUM_SLOTS) {
        if (reinforce) {
            activity[currentSlot] += 1; //reinforce this slot (Slots with higher numbers = times user typically use the lamp)
        } else {
            activity[currentSlot] -= 1;
        }
    }
    debugPrediction(" LOGGED AT TIMESLOT: "); 
    debugPrediction(currentSlot); 
    debugPrediction("/96 "); 
    debugPredictionln(activity[currentSlot]);
}

void decayActivity() { //keep current trends dominant
    for (uint8_t i = 0 ; i < Config::NUM_SLOTS ; i++) {
        activity[i] *= Config::DECAY_FACTOR; 
    }
    debugPredictionln("DECAYING DATA");
}

void handleLogging(){ 

    if (manualOverrideTimer) {
        if (state.currentLampStatus && !wasLampOn) {
            logUsage(true);
        } 
        else if (!state.currentLampStatus && wasLampOn) {
            logUsage(false);
        }
    }
    /*
        wasMidnightPassed = passingMidnight();
    if (wasMidnightPassed && !hasDecayedToday) {
        decayActivity();
        hasDecayedToday = true;
        lastCheckedDay++;
    }
    if ( hourOfDay >= 0.0f ) {
        hasDecayedToday = 
    }
    */
    wasLampOn = state.currentLampStatus;
}

uint8_t predictLampBrightness(float ambientLux) { // auto mode does not increase to full brightness
    uint8_t clampedLux = constrain(ambientLux, Config::MIN_LUX, Config::MAX_LUX);
    float normalizedLux = float(clampedLux - Config::MIN_LUX)/(Config::MAX_LUX - Config::MIN_LUX);
    float darknessFactor = 1.0f - normalizedLux;
    float brightnessFloat = 0.0f + round(darknessFactor * (5.0f - 0.0f));
    uint8_t brightnessLevel = round(brightnessFloat);

    brightnessLevel = constrain(brightnessLevel,1,5);
    return brightnessLevel; 
}

void smartPredict( bool movement, float light_lux ) { // predicts user habits
    handleLogging();
    if (currentSlot >= 0 && currentSlot < Config::NUM_SLOTS) {
        if ((activity[currentSlot] < Config::ACTIVITY_THRESHOLD) || ( light_lux > Config::LUX_THRESHOLD)) {
            // debugPredictionln("PREDICTED LAMP OFF");
            predicted.lampStatus = 0;
            predicted.brightness = 0;
        } else if ((activity[currentSlot] >= Config::ACTIVITY_THRESHOLD) && (light_lux < Config::LUX_THRESHOLD) && movement) {
            // debugPredictionln("PREDICTED LAMP ON");
            predicted.lampStatus = 1;
            predicted.brightness = predictLampBrightness(light_lux);
            Serial.print("PREDICTED BRIGHTNESS: "); Serial.println(predicted.brightness);
        }
    }
}
