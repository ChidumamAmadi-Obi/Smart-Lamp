# Data Collection With Pico
### Heres a diagram of whats going on in this data collection system
![image](https://github.com/user-attachments/assets/6a49523f-35dd-4db9-acb2-cfb075a23696)

#### How it works
1. The Pico Collects sensor data and user inputs
2. The Pico sends this data to the PC serial port
3. Python script accesses the serial port and logs the data into a CSV file

### Hardware set up
![image](https://github.com/user-attachments/assets/cb3a1f16-a888-4a36-aeef-44fe7973d681)

## Problems
* LDR error: it seems like during peak dyalight hours, the LDR acts like a short. Needs eror handling here.
  ![image](https://github.com/user-attachments/assets/283795f4-e31e-4d73-8b6a-69be3991f9df)

