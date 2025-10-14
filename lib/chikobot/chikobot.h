#ifndef _CHIKOBOT_H_
#define _CHIKOBOT_H_

#define LOG_LOCAL_LEVEL   ESP_LOG_INFO

#define HOLD_TIME_MS      5000
#define DEBOUNCE_TIME_MS  50 

#include <Arduino.h>           // Core Arduino functionality
#include <chiko_joint.h>       // Custom joint control for ChikoBot
#include <chiko_BMA250.h>      // BMA250 accelerometer support
#include <chiko_action.h>      // Predefined actions for ChikoBot
//#include <chiko_face.h>        // ChikoBot facial expressions
#include <chiko_wifi.h>        // WiFi and networking utilities
#include <chiko_displayUI.h>
#include <U8g2lib.h>
#include <esp_sleep.h>         // ESP32 deep sleep functionality
#include <esp_err.h>          // ESP32 error codes
#include <time.h>           // Time functions




enum LEDStates {
  LED_OFF,
  LED_ON_SOLID,
  LED_DOUBLE_BLINK_FADE,
  LED_SINGLE_BLINK_FADE,
  LED_BREATHING
};




void initilize_chikobot(void);

float readBatteryVoltage(void);


#endif // _CHIKOBOT_H_