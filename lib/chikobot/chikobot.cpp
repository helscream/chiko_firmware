
#include "chikobot.h"

#define BUTTON_PIN  33
#define LED_PIN     17
#define CHARGE_INDICATOR_PIN 16
#define FUNCTION_BUTTON_PIN 0

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


U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/U8X8_PIN_NONE, /* dc=*/26, /* reset=*/25);

pages ChikoPages;
// Variables for interrupt and timing
//volatile bool actionReady = false;

void goToDeepSleep() {
  Serial.println("Disabling joints for deep sleep.");
  disable_joints();
  Serial.println("Going to deep sleep...");
  delay(100); // Short delay to ensure serial message is sent
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, HIGH); // Wake up when pin is HIGH
  esp_deep_sleep_start();
}

void goToLightSleep() {
  Serial.println("Going to light sleep...");
  esp_sleep_enable_ext0_wakeup((gpio_num_t)BUTTON_PIN, HIGH); // 1 = wake on HIGH
  esp_light_sleep_start();
  Serial.println("Woke up from light sleep.");
  // setFaceEmoji(NORMAL);
}

void ButtonPressTask(void* parameter) {
  // If button is held LOW, check how long
  unsigned long buttonLowStart = millis();
  while (true) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      if (millis() - buttonLowStart >= HOLD_TIME_MS) {
        Serial.println("Ready to sleep ... You can release the button.");
        // setFaceEmoji(SLEEPY);
        
        while(digitalRead(BUTTON_PIN) == HIGH) {
          // Wait for button release to avoid immediate wake-up
          delay(100);
        }
        Serial.println("ZZZzzzz...!");
        // DisplayPage(OFF);
        goToDeepSleep();
      }else{
        // setFaceEmoji(HAPPY);
      }
    } else {
      // setFaceEmoji(NORMAL);
      delay(1); // Before deleting task, give time for face emoji to update
      vTaskDelete(NULL); // Delete this task when button is released
    }
    delay(10); // Polling delay
  }
}



void IRAM_ATTR handleButtonInterrupt() {
	// Called when button state changes
	if (digitalRead(BUTTON_PIN) == HIGH) {
    // Button pressed
    xTaskCreatePinnedToCore(
      ButtonPressTask,   // Function to implement the task
      "ButtonPressTask", // Name of the task
      10000,             // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
	}
}


void FunctionButtonTask(void* parameter) {
  // If function button is held LOW, check how long
  // unsigned long buttonLowStart = millis();
  // while (true) {
  //   if (digitalRead(FUNCTION_BUTTON_PIN) == LOW) {
  //     Serial.println("Function button pressed.");
  //     if(areJointsActive()){
  //       Serial.println("Disabling joints.");
  //       disable_joints();
  //     } else {
  //       Serial.println("Enabling joints.");
  //       enable_joints();
  //     }
  //   } else {
  //     vTaskDelete(NULL); // Delete this task when button is released
  //   }
  //   delay(1000); // Polling delay
  // }
  ChikoPages.btnFunctionAction();
  vTaskDelete(NULL);
}

void IRAM_ATTR handleFunctionButtonInterrupt() {
  // Called when function button state changes
  if (digitalRead(FUNCTION_BUTTON_PIN) == LOW) {
    ChikoPages.btnFunctionSelect();
  }else{
    ChikoPages.btnFunctionSelect(false);

    static unsigned long lastDebounceTime = 0;
    if (millis() - lastDebounceTime > DEBOUNCE_TIME_MS) {
      lastDebounceTime = millis();
      xTaskCreatePinnedToCore(
        FunctionButtonTask,   // Function to implement the task
        "FunctionButtonTask", // Name of the task
        10000,             // Stack size in words
        NULL,             // Task input parameter
        1,                // Priority of the task
        NULL,             // Task handle
        1);               // Core where the task should run
      }
  }
}

void batteryMonitorTask(void* parameter) {
  Serial.println("Battery monitoring started.");
  pinMode(CHARGE_INDICATOR_PIN, INPUT_PULLUP);
  while (true) {
    float voltage = readBatteryVoltage();
    // Optionally, check charging status
    if (digitalRead(CHARGE_INDICATOR_PIN) == LOW) {
      Serial.println("Charging...");
    } else {
      Serial.println("Not Charging.");
    }
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



void statusLedTask(void* parameter) {
  LEDStates currentState = LED_BREATHING;
  ledcAttachChannel(LED_PIN, 5000, 8, 7); // 5 kHz PWM, 8-bit resolution
  while (true) {
    switch (currentState)
    {
    case LED_OFF:
      /* code */
      ledcWrite(LED_PIN, 0);
      break;
    case LED_ON_SOLID:
      ledcWrite(LED_PIN, 255);
      break;
    case LED_DOUBLE_BLINK_FADE:
      ledcWrite(LED_PIN, 255);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ledcWrite(LED_PIN, 0);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ledcWrite(LED_PIN, 255);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ledcWrite(LED_PIN, 0);
      vTaskDelay(700 / portTICK_PERIOD_MS);
      break;
    case LED_SINGLE_BLINK_FADE:
      ledcWrite(LED_PIN, 255);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ledcWrite(LED_PIN, 0);
      vTaskDelay(900 / portTICK_PERIOD_MS);
      break;
    case LED_BREATHING:
      for (int brightness = 0; brightness <= 255; brightness += 5) {
        ledcWrite(LED_PIN, brightness);
        vTaskDelay(30 / portTICK_PERIOD_MS);
      }
      for (int brightness = 255; brightness >= 0; brightness -= 5) {
        ledcWrite(LED_PIN, brightness);
        vTaskDelay(30 / portTICK_PERIOD_MS);
      }
      break;
    default:
      break;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Small delay to avoid watchdog reset
  }
  vTaskDelete(NULL); // Delete this task if it ever exits (it won't)
}

// void showInfoOnFace(){
//   display_clearDisplay();
//   facePrint("ChikoBot v1.0", 0, 0);
//   facePrint("www.chikodroid.com", 0, 10);
//   facePrint("", 0, 20);
//   facePrint("IP: 192.168.3.2", 0, 30);
//   float voltage = readBatteryVoltage();
  
//   facePrint("4.3 V", 0, 40);
// } 

void test(){
  Serial.println("Function button action");
}

void initilize_chikobot(void){
  Serial.begin(115200);
  Serial.println("Chiko Here!");
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleButtonInterrupt, CHANGE);
  // Check if woke from deep sleep
	if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
		Serial.println("Hello Again!");
	} else {
		Serial.println("Hello.");
	}
  // Function button pin setup
  pinMode(FUNCTION_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FUNCTION_BUTTON_PIN), handleFunctionButtonInterrupt, CHANGE);
  // Setup charge indicator pin
  pinMode(CHARGE_INDICATOR_PIN, INPUT_PULLUP);
    
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
    // Setup status LED task
    xTaskCreatePinnedToCore(
      statusLedTask,   // Function to implement the task
      "StatusLedTask", // Name of the task
      1000,             // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
    
    u8g2.begin();
    
    pageConfig tempConfig = ChikoPages.getPageConfig(0);

    sprintf(tempConfig.btnFunName,"Toggle");
    tempConfig.taskBarVisibility = false;
    tempConfig.btnFunClickAction = &test;

    ChikoPages.setPageConfig(0,tempConfig);
    
    ChikoPages.intilize(u8g2,4);
    /*

    initialize_Wifi(); 
    Serial.println("WiFi Initialized.");

    initialize_joints(&LeftLeg, &RightLeg, &LeftFoot, &RightFoot);
    Serial.println("Joints Initialized.");
    accelrometer.initialize();
    Serial.println("Joints and Accelerometer Initialized.");

    initialize_face();
    Serial.println("Face Initialized.");
    
    accelrometer.attachDoubleTapToAction(TOP, [](){
      Serial.println("Top face double-tapped.");
      // if(areJointsActive()){
      //   Serial.println("Disabling joints.");
      //   disable_joints();
      // } else {
      //   Serial.println("Enabling joints.");
      //   enable_joints();
      // }
      DisplayPage(FACE);

    });

    accelrometer.attachDoubleTapToAction(LEFT, [](){
      Serial.println("Left face double-tapped.");
      setFaceEmoji(LOOK_LEFT_BIG);
      delay(100);
      DisplayPage(LEFTPAGE);
    });

    accelrometer.attachDoubleTapToAction(RIGHT, [](){
      Serial.println("Right face double-tapped.");
      setFaceEmoji(LOOK_RIGHT_BIG);
      delay(100);
      DisplayPage(RIGHTPAGE);
    });
    Serial.println("ChikoBot Initialized!");

    */
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


