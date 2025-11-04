# Smart-Lamp
The Smart Lamp is a predictive system built on the ESP32, employing a custom time-based learning algorithm, sensor fusion, and a multi-layered state machine to anticipate user needs.

### Demo
Remote control Demontration

[](https://github.com/user-attachments/assets/f81e4caa-da95-4633-842e-62d66da5330c)

![](https://github.com/user-attachments/assets/f81e4caa-da95-4633-842e-62d66da5330c)

### Features
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

### Block Diagrams
#### Overall System Functionality
<img width="588" height="486" alt="image" src="https://github.com/user-attachments/assets/7cca02f0-4ed7-4360-8e14-8e172a3521ca" />

#### Power Managment (SleepManager.h)
<img width="962" height="353" alt="image" src="https://github.com/user-attachments/assets/765cefe9-b722-4aa6-92e8-7b2d4892bc5c" />

1) **Active Mode (95-240)mA**: All systems online, processing sensors and logic.
2) **Light Sleep (~0.8)mA**: Entered after a period of no motion, the CPU is paused, but RAM is retained. It wakes up periodically (e.g. every 5 minutes) to quickly check the PIR sensor.
3) **Deep sleep (~10-150)µA**: Entered after extended light sleep with no motion or via IR command *IR_SLEEP*. The system is powered down, leaving only the RTC on, and the ability to wake at the press of a button.

### Setup Instructions 
**1)** Open ArduinoIDE and ensure ESP32 board support is installed. (esp32 by Espressif Systems)

**2)** Start a new sketch, download and include the "IRremote by shirriff, z3t0, ArminJo" library.
```C
#include "IRremote.hpp"
```
#### Configuring IR remote Commands
To determine the IR protocol and key codes used by your unique remote, follow this guide ![here](https://www.makerguides.com/ir-receiver-remote-arduino-tutorial/).

**3.1)** Wire the output pin of the IR receiver to pin 2 of your ESP32 and flash the code below:
```C
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
**3.2)** Open the serial monitor and observe the values that print when the corresponding button is pressed on your IR remote. <br>
  
**3.3)** Clone this repository:
```
git clone https://github.com/ChidumamAmadi-Obi/Smart-Lamp.git
cd Smart-Lamp/SmartLampESP32
```
**3.4)** Open the *Config.h* file and replace the default IR command values with your observed key codes here:
```C
  // IR Remote Commands
  constexpr uint8_t IR_ON = 69;
  constexpr uint8_t IR_OFF = 71;
  constexpr uint8_t IR_BRIGHTEN = 90;
  constexpr uint8_t IR_DIM = 8;
  constexpr uint8_t IR_AUTO = 68;
  constexpr uint8_t IR_SLEEP = 67;
  constexpr uint8_t IR_INVALID = 255;
```
**3.5)** Save this file,your lamp will now respond to your custom remote buttons. <br>

*NOTE: IR:INVALID is a placeholder for "no valid command", leave this as 255 unless you need a special case.
If your remote sends different bit lengths or protocols, make sure you’re using the correct mode in the IRremote library.* <br>
**4)** Open *Config.cpp* and update the wifi credentials to fit your home WiFi:
```C
const char* ssid = "YOUR_SSID";
const char* password   = "YOUR_PASSWORD";
```
**5)** Open the SmartLampESP32.ino file with Arduino IDE. <br>
**6)** Connect sensors and components as shown in the wiring diagram. <br>
```C
  // Pin Configuration
  constexpr uint8_t LDR_PIN = 4;
  constexpr uint8_t LAMP_PIN = 27;
  constexpr uint8_t IR_PIN = 23;
  constexpr uint8_t PIR_MOTION_PIN = 19;
  constexpr uint8_t LED_BUILTIN = 2;
  constexpr uint8_t AUTO_LED_PIN = 26;
  constexpr uint8_t WIFI_LED_PIN = 25;
  constexpr uint8_t ERROR_LED_PIN = 33;
```

<img width="971" height="728" alt="image" src="https://github.com/user-attachments/assets/018cb398-74d9-4d2a-8f06-2845002019ad" /> <br>

**6)** Flash the code to your ESP32. <br>
**7)** Power the circuit and observe lamp control via sensors and manual override.

### Roadmap
- [x] IR Remote Control
- [x] PIR + LDR + Time-based auto control
- [x] Basic pattern learning
- [x] Deep Sleep mode
- [ ] MQTT for remote monitoring

### Future Goals
  * Increase robustness and error handling
  * Add MQTT for cloud logging and monitoring
  * Design Custom PCB with aggressive anti-noise properties
  * Design housing

