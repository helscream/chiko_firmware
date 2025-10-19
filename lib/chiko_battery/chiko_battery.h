#ifndef __CHIKO_BATTERY__
#define __CHIKO_BATTERY__

#include "Arduino.h"
#include "chiko_definations.h"
#include "chiko_logs.h"

float readBatteryVoltage(void);
void initilize_batteryMonitor();
bool isBatteryCharging();


#endif