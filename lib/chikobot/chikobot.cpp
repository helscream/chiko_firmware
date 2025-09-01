
#include "chikobot.h"

#define BUTTON_PIN 33

// Battery monitoring constants
#define BATTERY_VOLTAGE_PIN 34 // Pin to read battery voltage (ADC1_6)
#define VOLTAGE_DIVIDER_RATIO 2.0 // Assuming a 1:1 voltage divider (adjust if different)
#define BATTERY_LOW_VOLTAGE 3.3 // Voltage threshold for low battery (in volts)
#define BATTERY_CHECK_INTERVAL_MS 1000 // Check battery every 1 seconds
#define BATTERY_AVERAGE_SAMPLES 10

// Declare joint objects for the robot's limbs.
// Each Joint object represents a servo or actuator controlling a limb segment.
Joint LeftLeg, RightLeg, LeftFoot, RightFoot;
// Accelerometer object for gesture detection (double-tap events)
BMA250 accelrometer;
// Action object for walking routine.
// Encapsulates the walking state machine (enter, loop, exit routines)

// Variables for interrupt and timing
volatile bool buttonLow = false;
volatile unsigned long buttonLowStart = 0;
volatile bool actionReady = false;

void goToDeepSleep() {
  Serial.println("Going to deep sleep...");
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, HIGH); // Wake up when pin is HIGH
  esp_deep_sleep_start();
}

void goToLightSleep() {
  Serial.println("Going to light sleep...");
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, HIGH); // 1 = wake on HIGH
  esp_light_sleep_start();
  Serial.println("Woke up from light sleep.");
}

void ButtonPressTask(void* parameter) {
  // If button is held LOW, check how long
  while (true) {
	if (buttonLow) {
		if (millis() - buttonLowStart >= HOLD_TIME_MS && !actionReady) {
			actionReady = true;
      Serial.println("Ready to sleep ... You can release the button.");
      while(digitalRead(BUTTON_PIN) == HIGH) {
        // Wait for button release to avoid immediate wake-up
        delay(100);
      }
      delay(100); // Small delay to ensure button is fully released
      Serial.println("ZZZzzzz...!");
      goToLightSleep();
		}
	} else {
		actionReady = false; // Reset when button released
    vTaskDelete(NULL); // Delete this task when button is released
	}
  vTaskDelay(100 / portTICK_PERIOD_MS); // Check every 100ms  
}
}

void batteryMonitorTask(void* parameter) {
  Serial.println("Battery monitoring started.");
  while (true) {
    float voltage = readBatteryVoltage();
    Serial.print("Battery Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");
    if (voltage < BATTERY_LOW_VOLTAGE) {
      Serial.println("Warning: Low Battery!");
      // Optionally, you could add code here to put the robot into a low-power state
    }
    vTaskDelay(BATTERY_CHECK_INTERVAL_MS / portTICK_PERIOD_MS); // Wait before next check
  }
}

void IRAM_ATTR handleButtonInterrupt() {
	// Called when button state changes
	if (digitalRead(BUTTON_PIN) == HIGH) {
		// Button just went LOW
		buttonLowStart = millis();
		buttonLow = true;
    xTaskCreatePinnedToCore(
      ButtonPressTask,   // Function to implement the task
      "ButtonPressTask", // Name of the task
      10000,             // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      0);               // Core where the task should run
	} else {
		// Button released
		buttonLow = false;
		buttonLowStart = 0;
	}
}


void initilize_chikobot(void){
    pinMode(BUTTON_PIN, INPUT_PULLDOWN);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, CHANGE);

	if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
		Serial.println("Hello Again!");
	} else {
		Serial.println("Hello.");
	}
    initialize_face();
    Serial.println("Face Initialized.");
    // Setup battery monitoring
    analogReadResolution(12); // Set ADC resolution to 12 bits (0-4095)
    analogSetPinAttenuation(BATTERY_VOLTAGE_PIN,ADC_0db); // Set attenuation for full-scale voltage (0-3.3V)
    Serial.print("Starting battery monitor...");
    xTaskCreatePinnedToCore(
      batteryMonitorTask,   // Function to implement the task
      "BatteryMonitorTask", // Name of the task
      5000,             // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
    initialize_Wifi(); 
    Serial.println("WiFi Initialized.");

    initialize_joints(&LeftLeg, &RightLeg, &LeftFoot, &RightFoot);
    Serial.println("Joints Initialized.");
    accelrometer.initialize();
    Serial.println("Joints and Accelerometer Initialized.");
    Serial.println("ChikoBot Initialized!");
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
