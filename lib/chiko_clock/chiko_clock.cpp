#include "chiko_clock.h"

const char LOG_TAG[] = "SYSTEM_CLOCK";

const long gmtOffset_sec = 0; // Example: 0 for UTC
const int daylightOffset_sec = 0; // Example: 0 if no daylight saving

// Global variables for timekeeping
static time_t timeAtSync;
static unsigned long millisAtSync;
static bool isTimeSynchronized = false;

text ClockHour, ClockMin, ClockColon, ClockInfo;

void clockTask(void* param){
    pages* _pages;
    _pages = (pages*) param;
    U8G2 _display = _pages->getDisplayHandel();
    ClockInfo.create(_display,"",MIDTOP, 64, 14);
    ClockHour.create(_display, "", RIGHTTOP, 55, 24,u8g2_font_courB18_tn);
    ClockColon.create(_display, "", MIDTOP, 64, 24,u8g2_font_courB18_tn);
    ClockMin.create(_display, "", LEFTTOP, 73, 24,u8g2_font_courB18_tn);

    if (_pages->getSelectedPageNumber() == CLOCK_PN){
        ClockInfo.clear();
        ClockInfo.setText("Clock Info!");
    }
    

    struct tm timeinfo;

    chikoLog(LOG_TAG,"Waiting for internet connection.!");
    while(!isConnectedToInternet()){
        for(int x=0;x<=5;x++){
            if (_pages->getSelectedPageNumber() == CLOCK_PN){
                ClockInfo.clear();
                ClockInfo.setText("Waiting for Internet!");
            }
            delay(1000);
        }
    }
    chikoLog(LOG_TAG,"Internet connected!");


    chikoLog(LOG_TAG,"Getting time information from NTP server!");
    while(!getTimeFromNTP(&timeinfo, GMT_OFFSET_SEC, DAY_LIGHT_OFFSET_SEC)){
        for(int x=0;x<=30;x++){
            if (_pages->getSelectedPageNumber() == CLOCK_PN){
                ClockInfo.clear();
                ClockInfo.setText("Connecting 2 NTP Server");
            }
            delay(1000);
        }
    }
    if (_pages->getSelectedPageNumber() == CLOCK_PN){
        ClockInfo.clear();
        ClockInfo.setText("Time acquired!");
    }
    

    chikoLog(LOG_TAG,"Time synchronized from NTP server!");
    
    setSystemTime(&timeinfo);

  

    while(true) {
        if (getSystemTime(&timeinfo)) {
            char timeBuffer[9]; // HH:MM:SS\0
            strftime(timeBuffer, sizeof(timeBuffer), "%H:%M:%S", &timeinfo);
            char dateBuffer[28]; // YYYY-MM-DD\0
            strftime(dateBuffer, sizeof(dateBuffer), "%A, %d %B %Y", &timeinfo);
            chikoLog(LOG_TAG,"%s %s",dateBuffer,timeBuffer);
            
            if (_pages->getSelectedPageNumber() == CLOCK_PN) {
                char hourStr[3];
                strftime(hourStr, sizeof(hourStr), "%H", &timeinfo);
                ClockHour.setText(hourStr);

                char minStr[3];
                strftime(minStr, sizeof(minStr), "%M", &timeinfo);
                ClockMin.setText(minStr);

                ClockInfo.clear();
                ClockInfo.setText(dateBuffer);

                if (timeinfo.tm_sec % 2) {
                    ClockColon.setText(":");
                } else {
                    ClockColon.setText(" ");
                }
            }
        }
        delay(1000); // Update every second
    }
}

void initialize_SystemClock(pages *_pages){
    xTaskCreatePinnedToCore(
      clockTask,   // Function to implement the task
      "SytemClockTask", // Name of the task
      20000,             // Stack size in words
      _pages,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
}


/**
 * @brief Sets the base time after a successful NTP synchronization.
 * This function should be called once after getTimeFromNTP() succeeds.
 * @param syncedTimeinfo A pointer to the struct tm containing the time from the NTP server.
 */
void setSystemTime(struct tm* syncedTimeinfo) {
  timeAtSync = mktime(syncedTimeinfo);
  millisAtSync = millis();
  isTimeSynchronized = true;
}

/**
 * @brief Gets the current system time, calculated from the last sync time.
 * @param currentTimeinfo A pointer to a struct tm to be filled with the current time.
 * @return true if the time has been synchronized and the current time is available, false otherwise.
 */
bool getSystemTime(struct tm* currentTimeinfo) {
  if (!isTimeSynchronized) {
    return false;
  }

  unsigned long elapsedSeconds = (millis() - millisAtSync) / 1000;
  time_t currentTime = timeAtSync + elapsedSeconds;

  // Use localtime_r for thread safety, which is important in a multi-tasking environment like FreeRTOS.
  localtime_r(&currentTime, currentTimeinfo);

  return true;
}