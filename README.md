# Smart-Lamp

An ESP32 based smart lamp that learns user behavior to predictively control lighting using motion, ambient light, and time of day.

## ✨ Features

  * Real-time data collection (motion, ambient light, manual toggles)
  * Time-based patterns (learns when you usually turn on the lamp)
  * Predictive automation (turns on lights before you enter the room)
  * Adaptive brightness (adjusts based on ambient light and habits)

## 🛠 Setup

  ### 1) Hardware:

  * ESP32
  * PIR SENSOR 
  * LDR
  * IR Receiver and Remote (38KHz)

  ### 2) Data Collection

  Sensor data is gathered by the ESP32 and sent to a PC over USB Serial (UART)
  
    Serial.print(lampStatus);
    Serial.print(",");
    Serial.print(lampBrightness);
    Serial.print(",");
    Serial.print(motion);
    Serial.print(",");
    Serial.println(ambientLight);
    
    // output: lampStatus,lampBrightness,motion,ambientLight

  Using a python script the data is logged automaticaly into a csv file

  ### 3) ML Training

    model.fit(X_train, y_train)  # Predicts "turn on lamp?" (0/1)

  ### 4) Depployment

  flash trained model onto the ESP32 for real time decisions

  🌟 Future Goals

  * Integrate WiFi for remote monitoring
  * Add MQTT for cloud logging
  * Energy-saving mode (dim during low-usage hours)

  ### Why It’s Cool

* No more fumbling for switches — the lamp anticipates your needs.
* Self-learning — improves accuracy over time.
