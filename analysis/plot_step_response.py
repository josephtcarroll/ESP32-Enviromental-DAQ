import pandas as pd
import matplotlib.pyplot as plt

# Load experimental step-response data
df = pd.read_csv("data/step_response.csv")

# Convert ESP32 time from milliseconds to seconds
time_s = (df["time_ms"] - df["time_ms"].iloc[0]) / 1000.0

# Plot raw and digitally filtered ADC measurements
plt.figure(figsize=(10, 6))

plt.plot(
    time_s,
    df["raw_adc"],
    label="Raw ADC"
)

plt.plot(
    time_s,
    df["filtered_adc"],
    label="20-Sample Moving Average"
)

plt.xlabel("Time (s)")
plt.ylabel("ADC Reading")

plt.title(
    "ESP32 Light Sensor Step Response"
)

plt.legend()
plt.grid()

plt.tight_layout()

plt.savefig(
    "results/esp32_step_response.png",
    dpi=300
)

plt.show()
