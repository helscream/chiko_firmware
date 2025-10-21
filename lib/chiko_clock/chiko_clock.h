#ifndef __CHIKO_CLOCK__
#define __CHIKO_CLOCK__

#include <Arduino.h>
#include <time.h>
#include "chiko_wifi.h"
#include "chiko_displayUI.h"

void initialize_SystemClock(pages *_pages);

/**
 * @brief Sets the base time after a successful NTP synchronization.
 * This function should be called once after getTimeFromNTP() succeeds.
 * @param syncedTimeinfo A pointer to the struct tm containing the time from the NTP server.
 */
void setSystemTime(struct tm* syncedTimeinfo);

/**
 * @brief Gets the current system time, calculated from the last sync time.
 * @param currentTimeinfo A pointer to a struct tm to be filled with the current time.
 * @return true if the time has been synchronized and the current time is available, false otherwise.
 */
bool getSystemTime(struct tm* currentTimeinfo);

#endif