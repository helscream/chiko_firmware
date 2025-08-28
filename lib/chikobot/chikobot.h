#ifndef _CHIKOBOT_H_
#define _CHIKOBOT_H_


#include <Arduino.h>           // Core Arduino functionality
#include <chiko_joint.h>       // Custom joint control for ChikoBot
#include <chiko_BMA250.h>      // BMA250 accelerometer support
#include <chiko_action.h>      // Predefined actions for ChikoBot
#include <chiko_face.h>        // ChikoBot facial expressions


#define HOLD_TIME_MS 5000

void initilize_chikobot(void);


#endif // _CHIKOBOT_H_