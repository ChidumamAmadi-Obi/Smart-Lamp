#ifndef PREDICT
#define PREDICT

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
RTC_DATA_ATTR uint8_t currentSlot = 0;

RTC_DATA_ATTR uint16_t dayNumber;
RTC_DATA_ATTR uint16_t prevDay;
RTC_DATA_ATTR uint16_t currentDay;

bool wasLampOn = false;
float isolatedAmbientLux;
unsigned long prevAmbientLightCheck = 0;

void initPredictionSystem() { // Checks if prevDay has never been set 
    if (prevDay == 0 && timeinfo.tm_yday != 0) {
        prevDay = timeinfo.tm_yday; // Initialize with the current day
        dayNumber = 1;
        debugPredictionln("Prediction system initialized.");
    }
}

void getIsolatedAmbientLight(){ // prevents positive feedback loop between the brightness of the lamp and the ldr
    uint8_t lampBrightness = state.currentBrightnessNumber; // save last brightness
    if (!state.isFading) {
        analogWrite(Config::LAMP_PIN,0);
        isolatedAmbientLux = Lux_Value();
        analogWrite(Config::LAMP_PIN,Config::brightnessIndex[lampBrightness]);
        debugPrediction("ISOLATED LUX:"); debugPredictionln(isolatedAmbientLux);
    }
} 

bool checkDay(){
    uint16_t currentDay = timeinfo.tm_yday;
    if (prevDay == 0) {
        prevDay = currentDay;
        dayNumber = 1;
        return false;
    }
    if (currentDay != prevDay) {
        dayNumber++;
        prevDay = currentDay;
        return true;
    }
    return false;
}

uint8_t getTimeSlot() { // divides each day into 15 minute intervals
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
    debugPrediction("DAY: ");
    debugPrediction(dayNumber);
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
    if (checkDay() && dayNumber != 1) { // check if next day
        decayActivity();
    }
    
    wasLampOn = state.currentLampStatus;
}

uint8_t predictLampBrightness() { // auto mode does not increase to full brightness
    uint8_t clampedLux = constrain(isolatedAmbientLux, Config::MIN_LUX, Config::MAX_LUX);
    float normalizedLux = float(clampedLux - Config::MIN_LUX)/(Config::MAX_LUX - Config::MIN_LUX);
    float darknessFactor = 1.0f - normalizedLux;
    float brightnessFloat = 0.0f + round(darknessFactor * (5.0f - 0.0f));
    uint8_t brightnessLevel = round(brightnessFloat);

    brightnessLevel = constrain(brightnessLevel,1,5);
    return brightnessLevel; 
}

void smartPredict( bool movement ) { // predicts user habits
    handleLogging();
    currentMillis = millis();
    if (currentMillis - prevAmbientLightCheck >= 2000) {
        delay(2.5);
        getIsolatedAmbientLight();
        delay(2.5); 
        prevAmbientLightCheck = currentMillis;
    }

    if (currentSlot >= 0 && currentSlot < Config::NUM_SLOTS) {
        if ((activity[currentSlot] < Config::ACTIVITY_THRESHOLD) || (isolatedAmbientLux > Config::LUX_MAX_THRESHOLD)) {
            predicted.lampStatus = 0;
            predicted.brightness = 0;
        } else if ((activity[currentSlot] >= Config::ACTIVITY_THRESHOLD) && (isolatedAmbientLux < Config::LUX_MIN_THRESHOLD) && movement) {
            predicted.lampStatus = 1;
            predicted.brightness = predictLampBrightness();
        }
    }
}

#endif
