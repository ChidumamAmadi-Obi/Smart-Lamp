#include "Config.h"

#define DEBUG_PREDICTION true
#if DEBUG_PREDICTION == 1 
#define debugPrediction(x) Serial.print(x)
#define debugPredictionln(x) Serial.println(x)
#else 
#define debugPrediction(x)
#define debugPredictionln(x)
#endif

int8_t activity[Config::NUM_SLOTS] = {0}; // divide 24hrs into 15 minute slots
uint8_t currentSlot = 0;
bool wasLampOn = false;

bool midnightPassed() {
    if ( hourOfDay >= 23.57f ) {
        debugPredictionln("MIDNIGHT PASSED");
        return true;
    } else {
        return false;
    }
}

int getTimeSlot() { 
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
    debugPrediction("PREDICTED LAMP STATUS: ");
    debugPrediction(predicted.lampStatus);
    debugPrediction(" LOGGED AT TIMESLOT: "); 
    debugPrediction(currentSlot); 
    debugPrediction("/96 "); 
    debugPredictionln(activity[currentSlot]);
}

void decayActivity() { //keep current trends dominant
    for (int i = 0 ; i < Config::NUM_SLOTS ; i++) {
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

    if (midnightPassed()) {
        decayActivity();
    }
    wasLampOn = state.currentLampStatus;
}

void smartPredict( bool movement, float light_lux ) { // predicts user habits

    handleLogging();
    if (currentSlot >= 0 && currentSlot < Config::NUM_SLOTS) {

        if (activity[currentSlot] < Config::THRESHOLD) {
            debugPredictionln("PREDICTED LAMP OFF");
            predicted.lampStatus = 0;
            predicted.brightness = 0;
        }
        else if ((activity[currentSlot] >= Config::THRESHOLD) && (light_lux < Config::LUX_THRESHOLD) && movement) {
            debugPredictionln("PREDICTED LAMP ON");
            predicted.lampStatus = 1;
            predicted.brightness = 3;
        }
    }
}
