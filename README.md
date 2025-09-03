# Smart-Lamp

Energy-efficient ESP32-powered smart lamp that combines sensor data, machine learning, and user behaviour patterns to create the perfect lighting experience.

## ✨ Core Functionality
  * Predictive Behavior - Learns user patterns and anticipates lighting needs
  * Time-based Logic - Adjusts brightness levels throughout the day
  * Real-time Sensor Integration - Motion detection, ambient light sensing, and manual controls
  * IR Remote Control - Complete control (power ON/OFF, brightness adjustment, dimming)
  * Automatic Adaptive Lighting - Intelligent behaviour based on ambient conditions, time of day, and room occupancy

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
- [x] Basic pattern learning
- [] MQTT for remote monitoring
- [ ] Deep Sleep mode

### Future Goals
  * Add MQTT for cloud logging
  * Energy-saving mode (dim during low-usage hours)
