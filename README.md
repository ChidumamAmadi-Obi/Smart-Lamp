# Smart-Lamp

ESP32-powered smart lamp combining multiple sensors and controls for energy efficiency and convenience.

## ✨ Features

  * Real-time data collection (motion, ambient light, manual toggles)
  * IR Remote Control (power ON/OFF, Brightness and dimming controls)
  * Automatic Control (Adjusts behaviour based on ambient light, time f day and room occupancy)
  * Deep Sleep Mode (Completly shuts down control logic and greatly reduces power consumption)

### Hardware:

  * ESP32 Dev Board
  * PIR Motion Sensor
  * LDR Sensor
  * IR Receiver and Remote (38KHz)
  * LED Lamp or light module

### How It Works
  1) Manual control through IR remote (on/off, brightness).
  2) Automatic control kicks in if no manual override:
    * Lamp stays off if room is bright enough.
    * Lamp turns on when dark and motion is detected.
    * Time-based logic adjusts brightness levels throughout the day.
  3) Deep Sleep Mode allows ultra-low power operation when user wants the lamp inactive.
  4) In the future, the lamp will learn patterns (wake times, sleep times, occupancy) and adapt lighting automatically.

### Roadmap
- [x] IR Remote Control
- [x] PIR + LDR + Time-based auto control
- [ ] Deep Sleep mode
- [ ] ML-powered habit learning

### Future Goals
  * Implement machine learning to improve accuracy over time
  * Integrate WiFi for remote monitoring
  * Add MQTT for cloud logging
  * Energy-saving mode (dim during low-usage hours)
