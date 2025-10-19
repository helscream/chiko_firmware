#ifndef __CHIKO_WIFI_H__
#define __CHIKO_WIFI_H__


#include <WiFi.h>
#include <time.h>
#include "chiko_definations.h"
#include "chiko_logs.h"
#include <U8g2lib.h>
#include "chiko_displayUI.h"

void initialize_Wifi(pages* _pages);
// void broadcastData(const char* data, uint16_t port = 4210);
bool isConnectedToInternet();
bool getTimeFromNTP(struct tm* timeinfo, long gmtOffset_sec = 0, int daylightOffset_sec = 0);
void startWifiTask();
void killWifiTask();

#endif // __CHIKO_WIFI_H__



