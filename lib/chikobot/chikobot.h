#ifndef _CHIKOBOT_H_
#define _CHIKOBOT_H_


#include <Arduino.h>           // Core Arduino functionality
#include <chiko_joint.h>       // Custom joint control for ChikoBot
#include <chiko_BMA250.h>      // BMA250 accelerometer support
#include <chiko_action.h>      // Predefined actions for ChikoBot
#include <chiko_face.h>        // ChikoBot facial expressions
#include <esp_sleep.h>         // ESP32 deep sleep functionality
#include <esp_err.h>          // ESP32 error codes


#define HOLD_TIME_MS 5000

void initilize_chikobot(void);


#endif // _CHIKOBOT_H_