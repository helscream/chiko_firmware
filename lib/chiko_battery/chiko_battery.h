#ifndef __CHIKO_BATTERY__
#define __CHIKO_BATTERY__

#include "Arduino.h"
#include "chiko_definations.h"
#include "chiko_logs.h"
#include "chiko_displayUI.h"

float readBatteryVoltage(void);
void initilize_batteryMonitor(pages *_pages);
bool isBatteryCharging();
float getBatteryPercentage();


#endif