#include "chiko_battery.h"

const char LOG_TAG[] = "BATT_MONITOR";
bool isCharging = false;

bool isBatteryCharging(){
    return isCharging;
}

// Reads analog voltage on pin 35 and returns the average of 10 samples (in volts)
float readBatteryVoltage(void) {
  long sum = 0;
  for (int i = 0; i < BATTERY_AVERAGE_SAMPLES; i++) {
    int raw = analogRead(BATTERY_VOLTAGE_PIN);
    // Simple compensation for ESP32 ADC nonlinearity (empirical correction)
    float corrected = raw * 1.1325 - 60; // Adjust these values as needed for your board
    if (corrected < 0) corrected = 0;
    sum += corrected;
    // Serial.print(raw); // Debug: print each sample
    delay(1); // Small delay for stability
  }
  float avg = sum / (float)BATTERY_AVERAGE_SAMPLES;
  // ESP32 ADC is 12-bit (0-4095), reference voltage is typically 3.3V
  float voltage = avg * (3.3 / 4095.0) * (float)VOLTAGE_DIVIDER_RATIO;
  return voltage;
}


void batteryMonitorTask(void* parameter) {
  chikoLog(LOG_TAG,"Battery monitoring started.");
  pinMode(CHARGE_INDICATOR_PIN, INPUT_PULLUP);
  while (true) {
    float voltage = readBatteryVoltage();
    // Optionally, check charging status
    if (digitalRead(CHARGE_INDICATOR_PIN) == LOW) {
        isCharging = true;
        chikoLog(LOG_TAG,"Charging... Battery Voltage: %2f V",voltage);
    } else {
        isCharging = false;
        chikoLog(LOG_TAG,"Not Charging... Battery Voltage: %2f V",voltage);
    }

    if (voltage < BATTERY_LOW_VOLTAGE) {
      chikoLog(LOG_TAG,"Warning: Low Battery!");
      // Optionally, you could add code here to put the robot into a low-power state
    }
    vTaskDelay(BATTERY_CHECK_INTERVAL_MS / portTICK_PERIOD_MS); // Wait before next check
  }
}

void initilize_batteryMonitor(){
// Setup battery monitoring
    analogReadResolution(12); // Set ADC resolution to 12 bits (0-4095)
    analogSetPinAttenuation(BATTERY_VOLTAGE_PIN,ADC_0db); // Set attenuation for full-scale voltage (0-3.3V)
    chikoLog(LOG_TAG,"Starting battery monitor...");
    xTaskCreatePinnedToCore(
      batteryMonitorTask,   // Function to implement the task
      "BatteryMonitorTask", // Name of the task
      5000,             // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
    }