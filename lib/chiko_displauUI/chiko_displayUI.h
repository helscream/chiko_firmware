#ifndef ESP32_DISPLAY_UI_H
#define ESP32_DISPLAY_UI_H

#include <U8g2lib.h>
#include <chiko_wifi.h>
#include <BluetoothSerial.h>

class DisplayUI {
public:
    DisplayUI(U8G2& display, const String& deviceName);
    void begin();
    void updateDisplay();

private:
    U8G2& u8g2;
    String name;
    String getBatteryStatus();
    String getWiFiStatus();
    String getBluetoothStatus();
    String getUniqueID();
    void drawTopBar();
    void drawCenterInfo();
    void drawBottomButtons();
};

#endif
