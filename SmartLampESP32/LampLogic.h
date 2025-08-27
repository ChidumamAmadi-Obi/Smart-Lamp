#include "Global.h"

void predict_brightness(float hour, bool movement, float light_lux) {

    if ( (movement == 1) && (light_lux < 300) && ((5 < hour < 8) || (20 < hour < 22.5)) ) { 
        predicted.lampStatus = 1;
        predicted.brightness = 3;
    } else {
        predicted.lampStatus = 0;
        predicted.brightness = 0;
    }

    // Serial.print("Predicted brightness number: ");  Serial.print(state.currentBrightnessNumber);  Serial.println("/5");
    // Serial.print("Hour: ");                         Serial.println(hour);
    // Serial.print("Movement: ");                     Serial.println(movement);
    // Serial.print("Lux: ");                          Serial.println(light_lux);
}
