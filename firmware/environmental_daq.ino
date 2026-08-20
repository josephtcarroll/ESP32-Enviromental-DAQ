#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

// -------------------- Pin Configuration --------------------

const int lightPin = 34;

// -------------------- Moving Average Filter --------------------

const int numSamples = 20;

int samples[numSamples];
int sampleIndex = 0;
long sampleSum = 0;

// -------------------- Timing --------------------

unsigned long startTime;

// -------------------- Setup --------------------

void setup() {

  Serial.begin(115200);

  // Initialize I2C using ESP32 SDA and SCL pins
  Wire.begin(21, 22);

  // Initialize BME280 at detected I2C address
  if (!bme.begin(0x76)) {
    Serial.println("ERROR: BME280 not found.");
    while (1);
  }

  // Initialize moving-average buffer with current ADC value
  int initialADC = analogRead(lightPin);

  for (int i = 0; i < numSamples; i++) {
    samples[i] = initialADC;
    sampleSum += initialADC;
  }

  startTime = millis();

  // CSV header
  Serial.println(
    "time_ms,raw_adc,filtered_adc,temp_c,humidity,pressure_hpa"
  );
}

// -------------------- Main Loop --------------------

void loop() {

  // Read analog light sensor
  int rawADC = analogRead(lightPin);

  // Remove oldest sample from running sum
  sampleSum -= samples[sampleIndex];

  // Store newest ADC measurement
  samples[sampleIndex] = rawADC;

  // Add newest sample to running sum
  sampleSum += rawADC;

  // Move circular-buffer index
  sampleIndex++;

  if (sampleIndex >= numSamples) {
    sampleIndex = 0;
  }

  // Calculate digital moving average
  float filteredADC =
      sampleSum / (float)numSamples;

  // Read BME280 environmental measurements
  float temperature =
      bme.readTemperature();

  float humidity =
      bme.readHumidity();

  float pressure =
      bme.readPressure() / 100.0F;

  // Calculate elapsed system time
  unsigned long timeMs =
      millis() - startTime;

  // Output measurements in CSV format
  Serial.print(timeMs);
  Serial.print(",");

  Serial.print(rawADC);
  Serial.print(",");

  Serial.print(filteredADC, 2);
  Serial.print(",");

  Serial.print(temperature, 2);
  Serial.print(",");

  Serial.print(humidity, 2);
  Serial.print(",");

  Serial.println(pressure, 2);

  delay(20);
}
