
#include "chikobot.h"

#include "chiko_definations.h"

const char LOG_TAG[] = "CHIKO";

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
  chikoLog(LOG_TAG,"Disabling joints for deep sleep.");
  // disable_joints();
  chikoLog(LOG_TAG,"Going to deep sleep...");
  delay(100); // Short delay to ensure serial message is sent
  esp_sleep_enable_ext0_wakeup((gpio_num_t)POWER_BUTTON_PIN, HIGH); // Wake up when pin is HIGH
  esp_deep_sleep_start();
}

void goToLightSleep() {
  chikoLog(LOG_TAG,"Going to light sleep...");
  esp_sleep_enable_ext0_wakeup((gpio_num_t)POWER_BUTTON_PIN, HIGH); // 1 = wake on HIGH
  esp_light_sleep_start();
  chikoLog(LOG_TAG,"Woke up from light sleep.");
  // setFaceEmoji(NORMAL);
}

void PowerButtonPressTask(void* parameter) {
  // If button is held LOW, check how long
  unsigned long buttonLowStart = millis();
  while (digitalRead(POWER_BUTTON_PIN) == HIGH) {
      if (millis() - buttonLowStart >= HOLD_TIME_MS) {
        chikoLog(LOG_TAG,"Ready to sleep ... You can release the button.");
        // setFaceEmoji(SLEEPY);
        while(digitalRead(POWER_BUTTON_PIN) == HIGH) {
          // Wait for button release to avoid immediate wake-up
          delay(100);
        }
        chikoLog(LOG_TAG,"ZZZzzzz...!");
        // DisplayPage(OFF);
        goToDeepSleep();
      }
      delay(10); // Before deleting task, give time for face emoji to update
    }
    ChikoPages.btnPowerAction();
    vTaskDelete(NULL); // Delete this task when button is released
}



void IRAM_ATTR handleButtonInterrupt() {
	// Called when button state changes
	if (digitalRead(POWER_BUTTON_PIN) == HIGH) {
    // Button pressed
    ChikoPages.btnPowerSelect(true);
    xTaskCreatePinnedToCore(
      PowerButtonPressTask,   // Function to implement the task
      "PowerButtonPressTask", // Name of the task
      10000,             // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
	}else{
    ChikoPages.btnPowerSelect(false);
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
  chikoLog(LOG_TAG,"Function button action");
}

void initilize_chikobot(void){
  Serial.begin(115200);
  chikoLog(LOG_TAG,"Chiko Here!");
  pinMode(POWER_BUTTON_PIN, INPUT_PULLDOWN);
	attachInterrupt(digitalPinToInterrupt(POWER_BUTTON_PIN), handleButtonInterrupt, CHANGE);
  // Check if woke from deep sleep
	if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0) {
		chikoLog(LOG_TAG,"Hello Again!");
	} else {
		chikoLog(LOG_TAG,"Hello.");
	}
  // Function button pin setup
  pinMode(FUNCTION_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FUNCTION_BUTTON_PIN), handleFunctionButtonInterrupt, CHANGE);
  // Setup charge indicator pin
  pinMode(CHARGE_INDICATOR_PIN, INPUT_PULLUP);
  
  initilize_LED();
  initilize_batteryMonitor();
  
  initialize_joints(&LeftLeg, &RightLeg, &LeftFoot, &RightFoot);
    chikoLog(LOG_TAG,"Joints Initialized.");

    u8g2.begin();
    
    ChikoPages.intilize(u8g2);
    ChikoPages.updatePage();
    

    accelrometer.initialize();
    
    accelrometer.attachDoubleTapToAction(TOP, [](){
      chikoLog(LOG_TAG,"Top face double-tapped.");
      if (ChikoPages.getSelectedPageNumber() == 0){
        ChikoPages.setEyesScanDirection(LOOK_UP);
        delay(EYE_SCAN_DELAY);
      }
      ChikoPages.gotoDefaultPage();
    });

    accelrometer.attachDoubleTapToAction(BOTTOM, [](){
      chikoLog(LOG_TAG,"Top face double-tapped.");
      if (ChikoPages.getSelectedPageNumber() == 0){
        ChikoPages.setEyesScanDirection(LOOK_DOWN);
        delay(EYE_SCAN_DELAY);
      }
      ChikoPages.gotoDefaultPage();
    });

    accelrometer.attachDoubleTapToAction(LEFT, [](){
      chikoLog(LOG_TAG,"Left face double-tapped.");
      if (ChikoPages.getSelectedPageNumber() == 0){
        ChikoPages.setEyesScanDirection(LOOK_LEFT);
        delay(EYE_SCAN_DELAY);
      }
      ChikoPages.gotoNextPage();
    });

    accelrometer.attachDoubleTapToAction(RIGHT, [](){
      chikoLog(LOG_TAG,"Right face double-tapped.");
      if (ChikoPages.getSelectedPageNumber() == 0){
        ChikoPages.setEyesScanDirection(LOOK_RIGHT);
        delay(EYE_SCAN_DELAY);
      }
      ChikoPages.gotoPreviousPage();
    });

    

    chikoLog(LOG_TAG,"ChikoBot Initializtion completed!");

    /*

    initialize_Wifi(); 
    Serial.println("WiFi Initialized.");

    initialize_joints(&LeftLeg, &RightLeg, &LeftFoot, &RightFoot);
    Serial.println("Joints Initialized.");
    
    Serial.println("Joints and Accelerometer Initialized.");

    initialize_face();
    Serial.println("Face Initialized.");
    
    

    */
}





