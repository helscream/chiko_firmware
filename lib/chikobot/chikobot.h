#ifndef _CHIKOBOT_H_
#define _CHIKOBOT_H_



#define HOLD_TIME_MS      5000
#define DEBOUNCE_TIME_MS  50 

#include <Arduino.h>           // Core Arduino functionality
#include <chiko_joint.h>       // Custom joint control for ChikoBot
#include <chiko_BMA250.h>      // BMA250 accelerometer support
#include <chiko_action.h>      // Predefined actions for ChikoBot
//#include <chiko_face.h>        // ChikoBot facial expressions
#include <chiko_wifi.h>        // WiFi and networking utilities
#include <chiko_displayUI.h>
#include "chiko_LED.h"
#include "chiko_clock.h"
#include "chiko_battery.h"
#include <U8g2lib.h>
#include <esp_sleep.h>         // ESP32 deep sleep functionality
#include <esp_err.h>          // ESP32 error codes
#include <time.h>           // Time functions










void initilize_chikobot(void);

float readBatteryVoltage(void);


#endif // _CHIKOBOT_H_