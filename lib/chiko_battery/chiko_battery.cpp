#include "chiko_battery.h"


const char LOG_TAG[] = "BATT_MONITOR";
bool isCharging = false;

bool isBatteryCharging(){
    return isCharging;
}

// Reads analog voltage on pin 35 and returns the average of 10 samples (in volts)
float readBatteryVoltage(void) {
  long sum = 0;
  for (int i = 0; i < BATTERY_AVERAGE_SAMPLES; i++) {
    int raw = analogRead(BATTERY_VOLTAGE_PIN);
    // Simple compensation for ESP32 ADC nonlinearity (empirical correction)
    float corrected = raw * 1.1325 - 60; // Adjust these values as needed for your board
    if (corrected < 0) corrected = 0;
    sum += corrected;
    // Serial.print(raw); // Debug: print each sample
    delay(1); // Small delay for stability
  }
  float avg = sum / (float)BATTERY_AVERAGE_SAMPLES;
  // ESP32 ADC is 12-bit (0-4095), reference voltage is typically 3.3V
  float voltage = avg * (3.3 / 4095.0) * (float)VOLTAGE_DIVIDER_RATIO;
  return voltage;
}

float getBatteryPercentage(){
    float voltage = readBatteryVoltage();
    float percentage = ((voltage - (float)BATTERY_MIN_VOLTAGE) / ((float)BATTERY_MAX_VOLTAGE - (float)BATTERY_MIN_VOLTAGE)) * (float)100.0;

    // Clamp the percentage between 0 and 100
    if (percentage > 100.0) {
        percentage = 100.0;
    } else if (percentage < 0.0) {
        percentage = 0.0;
    }
    return percentage;
}


void batteryMonitorTask(void* parameter) {
  pages *_pages;
  _pages = (pages*)parameter;
  chikoLog(LOG_TAG,"Battery monitoring started.");
  pinMode(CHARGE_INDICATOR_PIN, INPUT_PULLUP);
  while (true) {
    float voltage = readBatteryVoltage();
    // Optionally, check charging status
    if (digitalRead(CHARGE_INDICATOR_PIN) == LOW) {
        isCharging = true;
        chikoLog(LOG_TAG,"Charging... Battery Voltage: %2f V",voltage);
    } else {
        isCharging = false;
        chikoLog(LOG_TAG,"Not Charging... Battery Voltage: %2f V",voltage);
    }

    if (voltage < BATTERY_MIN_VOLTAGE) {
      chikoLog(LOG_TAG,"Warning: Low Battery!");
      // Optionally, you could add code here to put the robot into a low-power state
    }
    if (_pages->getSelectedPageNumber() == BATTERY_INFO_PN){
        uint8_t _x = 15, _y = 15, _h = 33, _w = 76, _r = 6;
        U8G2 _display = _pages->getDisplayHandel();
        _display.setDrawColor(0);
        _display.drawBox(0,_y,_display.getWidth(),_h);
        _display.setDrawColor(1);
        _display.drawRFrame(_x, _y, _w, _h, _r);
        _display.drawRBox(_x+_w-1, 24, 7, 15, 2);
        _display.drawRBox(_x, _y, round(_w*getBatteryPercentage()/100), _h, _r);
        if (isCharging){
            _display.drawLine(_x + _w + 10, _y + 19, _x + _w + 20, _y + 4);
            _display.drawLine(_x + _w + 10, _y + 19, _x + _w + 17, _y + 18);
            _display.drawLine(_x + _w + 20, _y + 4, _x + _w + 17, _y + 14);
            _display.drawLine(_x + _w + 17, _y + 14, _x + _w + 23, _y + 13);
            _display.drawLine(_x + _w + 17, _y + 18, _x + _w + 13, _y + 29);
            _display.drawLine(_x + _w + 24, _y + 13, _x + _w + 14, _y + 28);
        }
        textfield batteryInfo;
        char temp[10];
        sprintf(temp,"%.2f V",readBatteryVoltage());
        batteryInfo.create(_display,_h+_y,"Battery Voltage: ",temp);
        batteryInfo.setMargins(5,2);
        _display.sendBuffer();
    }
    

    vTaskDelay(BATTERY_CHECK_INTERVAL_MS / portTICK_PERIOD_MS); // Wait before next check
  }
}

void initilize_batteryMonitor(pages *_pages){
// Setup battery monitoring
    analogReadResolution(12); // Set ADC resolution to 12 bits (0-4095)
    analogSetPinAttenuation(BATTERY_VOLTAGE_PIN,ADC_0db); // Set attenuation for full-scale voltage (0-3.3V)
    chikoLog(LOG_TAG,"Starting battery monitor...");
    xTaskCreatePinnedToCore(
      batteryMonitorTask,   // Function to implement the task
      "BatteryMonitorTask", // Name of the task
      5000,             // Stack size in words
      _pages,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
    }