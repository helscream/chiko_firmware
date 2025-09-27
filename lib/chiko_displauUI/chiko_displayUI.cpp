#include "chiko_displayUI.h"

DisplayUI::DisplayUI(U8G2& display, const String& deviceName)
    : u8g2(display), name(deviceName) {}

void DisplayUI::begin() {
    u8g2.begin();
}

void DisplayUI::updateDisplay() {
    u8g2.clearBuffer();
    drawTopBar();
    drawCenterInfo();
    drawBottomButtons();
    u8g2.sendBuffer();
}

String DisplayUI::getBatteryStatus() {
    // Simulated battery percentage
    return "Bat: 85%";
}

String DisplayUI::getWiFiStatus() {
    return isConnectedToInternet() ? "WiFi: ✔" : "WiFi: ✖";
}

String DisplayUI::getBluetoothStatus() {
    // return BluetoothSerial::isReady() ? "BT: ✔" : "BT: ✖";
    return "BT: ✖";
}

String DisplayUI::getUniqueID() {
    uint64_t chipid = ESP.getEfuseMac();
    char id[20];
    sprintf(id, "ID: %04X%04X", (uint16_t)(chipid >> 32), (uint16_t)chipid);
    return String(id);
}

void DisplayUI::drawTopBar() {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, 10, getBluetoothStatus().c_str());
    u8g2.drawStr(0, 20, getWiFiStatus().c_str());

    int batteryWidth = u8g2.getStrWidth(getBatteryStatus().c_str());
    u8g2.drawStr(u8g2.getDisplayWidth() - batteryWidth, 10, getBatteryStatus().c_str());
}

void DisplayUI::drawCenterInfo() {
    u8g2.setFont(u8g2_font_6x13_tf);
    int y = u8g2.getDisplayHeight() / 2 - 10;
    u8g2.drawStr(10, y, getUniqueID().c_str());

    String ssid = WiFi.SSID();
    if (ssid.length() > 0) {
        u8g2.drawStr(10, y + 15, ("SSID: " + ssid).c_str());
    }

    u8g2.drawStr(10, y + 30, ("Name: " + name).c_str());
}

void DisplayUI::drawBottomButtons() {
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(0, u8g2.getDisplayHeight() - 2, "Function");

    int resetWidth = u8g2.getStrWidth("Reset");
    u8g2.drawStr(u8g2.getDisplayWidth() - resetWidth, u8g2.getDisplayHeight() - 2, "Reset");
}
