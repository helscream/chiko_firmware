#ifndef __CHIKO_WIFI_H__
#define __CHIKO_WIFI_H__


#include <WiFi.h>
#include <time.h>





void initialize_Wifi();
// void broadcastData(const char* data, uint16_t port = 4210);
bool isConnectedToInternet();
bool getTimeFromNTP(struct tm* timeinfo, long gmtOffset_sec = 0, int daylightOffset_sec = 0);


#endif // __CHIKO_WIFI_H__



