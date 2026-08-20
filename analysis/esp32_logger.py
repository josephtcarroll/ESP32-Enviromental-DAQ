import serial
import time
import pandas as pd

PORT = "COM4"
BAUD = 115200
DURATION = 20

ser = serial.Serial(
    PORT,
    BAUD,
    timeout=1
)

time.sleep(2)

rows = []

start = time.time()

print(
    f"Collecting data for {DURATION} seconds..."
)

while time.time() - start < DURATION:

    line = (
        ser.readline()
        .decode(errors="ignore")
        .strip()
    )

    if not line:
        continue

    parts = line.split(",")

    if len(parts) != 6:
        continue

    try:

        rows.append([
            float(parts[0]),
            float(parts[1]),
            float(parts[2]),
            float(parts[3]),
            float(parts[4]),
            float(parts[5])
        ])

    except ValueError:
        pass

ser.close()

df = pd.DataFrame(
    rows,
    columns=[
        "time_ms",
        "raw_adc",
        "filtered_adc",
        "temp_c",
        "humidity",
        "pressure_hpa"
    ]
)

df.to_csv(
    "esp32_data.csv",
    index=False
)

print(
    f"Samples collected: {len(df)}"
)
