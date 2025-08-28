#include "chikobot.h"

#define BUTTON_PIN 33

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
      esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, HIGH); // Wake up when pin is HIGH
      esp_deep_sleep_start();
		}
	} else {
		actionReady = false; // Reset when button released
    vTaskDelete(NULL); // Delete this task when button is released
	}
  vTaskDelay(100 / portTICK_PERIOD_MS); // Check every 100ms  
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
}