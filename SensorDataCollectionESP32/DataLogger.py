# Python script logs incoming data from com port 10 into a csv file for processing
# Must close arduino IDE when running

import serial
import csv
import os

CSV_FILE = "sensor_log.csv"
HEADER = ["Timestamp", "Lamp Brightness", "Movement","Light (Lux)"]

ser = serial.Serial('COM10', 115200, timeout=1)

# Create CSV file with header if it doesn't exist
if not os.path.exists(CSV_FILE):
    with open(CSV_FILE, "w", newline='') as f:
        writer = csv.writer(f)
        writer.writerow(HEADER)
    print(f"Created new file: {CSV_FILE}")
else:
    print(f"Appending to existing file...") 

# Open CSV in append mode
with open(CSV_FILE, "a", newline='') as f:
    writer = csv.writer(f)
    print("Press Ctrl+C to stop...")
    try:
        while True:
            line = ser.readline().decode().strip()  # Read data from Pico
            if line:
                data = line.split(",")  # Split CSV from Pico
                writer.writerow(data)  # Write row
                f.flush()  # Force-save to disk
                print(f"Logged: {data}")
    except KeyboardInterrupt:
        print("Logging stopped.")
