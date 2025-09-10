# Smart-Lamp
An ESP32-powered smart desk lamp that adjusts lighting based on ambient conditions and user activity. The system uses sensors to measure brightness, motion, and environmental factors, enabling both manual and predictive lighting control.

### Demo
Remote control Demontration

https://github.com/user-attachments/assets/f81e4caa-da95-4633-842e-62d66da5330c

### ✨ Core Functionality
  * Predictive Behavior - Learns user patterns and anticipates lighting needs
  * Deep Sleep - Ultra low power mode when idle
  * Time-based Logic - Adjusts brightness levels throughout the day
  * Real-time Sensor Integration - Motion detection, ambient light sensing, and manual controls
  * IR Remote Control - Complete control (power ON/OFF, brightness adjustment, dimming)
  * Automatic Adaptive Lighting - Intelligent behaviour based on ambient conditions, time of day, and room occupancy

### Hardware:
  * **ESP32 Dev Board**
  * **LED Lamp or light module**
  * **PIR Motion Sensor** - For measureing ambient light levels
  * **LDR (Light Dependant Resistor)** - for activity detection
  * **IR Receiver and Remote (38KHz)** - for remote control
  * **Transistor (TIP122G)** -to control lamp power
  * (Optional) **5mm LEDs** - for debugging and testing

### How It Works
 1) Manual control through IR remote (on/off, brightness).
 2) Automatic control kicks in if no manual override:
    * Lamp stays off if room is bright enough.
    * Lamp turns on when dark and motion is detected.
    * Time-based logic adjusts brightness levels throughout the day.
 3) Deep Sleep Mode allows ultra-low power operation when user wants the lamp inactive.
 4) The lamp will learns usage patterns and adapts lighting automatically overtime.

### Setup Instructions 
**1** Open ArduinoIDE and ensure ESP32 board support is installed. (esp32 by Espressif Systems)

**2** Start a new sketch, download and include the "IRremote by shirriff, z3t0, ArminJo" library.
```
#include "IRremote.hpp"
```
#### Configuring IR remote Commands
To determine the IR protocol and key codes used by your unique remote, follow this guide ![here](https://www.makerguides.com/ir-receiver-remote-arduino-tutorial/).

**3.1** Wire the output pin of the IR receiver to pin 2 of your ESP32 and flash the code below:
```
#include "IRremote.hpp"

#define IR_RECEIVE_PIN 2

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    Serial.println(command);
    delay(100);  // wait a bit
    IrReceiver.resume();
  }
}
```
**3.2** Open the serial monitor and observe the values that print when the corresponding button is pressed on your IR remote.
  
**3.3** Clone this repository:
```
git clone https://github.com/ChidumamAmadi-Obi/Smart-Lamp.git
cd Smart-Lamp/SmartLampESP32
```
**3.4** Open the *Config.h* file and replace the default IR command values with your observed key codes here:
```
  // IR Remote Commands
  constexpr uint8_t IR_ON = 69;
  constexpr uint8_t IR_OFF = 71;
  constexpr uint8_t IR_BRIGHTEN = 90;
  constexpr uint8_t IR_DIM = 8;
  constexpr uint8_t IR_AUTO = 68;
  constexpr uint8_t IR_SLEEP = 67;
  constexpr uint8_t IR_INVALID = 255;
```
**3.5** Save this file,your lamp will now respond to your custom remote buttons.

*NOTE: IR:INVALID is a placeholder for "no valid command", leave this as 255 unless you need a special case.
If your remote sends different bit lengths or protocols, make sure you’re using the correct mode in the IRremote library.*
**4** Open *Config.cpp* and update the wifi credentials to fit your home WiFi:
```
const char* ssid = "YOUR_SSID";
const char* password   = "YOUR_PASSWORD";
```
**5** Open the SmartLampESP32.ino file with Arduino IDE.
**6** Connect sensors and components as shown in the wiring diagram.

<img width="971" height="728" alt="image" src="https://github.com/user-attachments/assets/018cb398-74d9-4d2a-8f06-2845002019ad" />

**6** Flash the code to your ESP32.
**7** Power the circuit and observe lamp control via sensors and manual override.

### Roadmap
- [x] IR Remote Control
- [x] PIR + LDR + Time-based auto control
- [x] Basic pattern learning
- [x] Deep Sleep mode
- [ ] MQTT for remote monitoring

### Future Goals
  * Decrease the amount of program storage space used (currently 73%)
  * Increase robustness and error handling
  * Add MQTT for cloud logging and monitoring
  * Design Custom PCB with aggressive anti-noise properties
  * Design housing

