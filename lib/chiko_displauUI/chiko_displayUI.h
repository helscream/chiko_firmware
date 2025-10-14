#ifndef ESP32_DISPLAY_UI_H
#define ESP32_DISPLAY_UI_H


#define LOG_TAG     "PAGES"

#include <chiko_definations.h>
#include <chiko_button.h>
#include <chiko_checkbox.h>
#include <chiko_textfield.h>
#include <chiko_taskbar.h>
#include <chiko_logs.h>

#include <U8g2lib.h>
// #include <chiko_face.h>
#include <stdint.h>
#include <stddef.h>
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



class pages {
    private:
        taskbar _pageTaskBar;
        uint8_t _pageCount, _currentPage = 0 , _previousPage=99;
        U8G2 _display;
        button _fun,_reset,_power;
        pageConfig _pageConfigs[10];
    public:
    

    void intilize(U8G2 &diplay, uint8_t pageCount);

    pageConfig getPageConfig(uint8_t pageNumber);
    void setPageConfig(uint8_t pageNumber, pageConfig config);

    void gotoNextPage();
    void gotoPreviousPage();
    uint8_t getSelectedPageNumber();
    uint8_t getPageCount();
    void gotoPageNumber(uint8_t pageNumber);
    void gotoDefaultPage();

    void updatePage(bool forceUpdate=false);

    void btnFunctionAction();
    void btnPowerAction();

    void btnFunctionSelect(bool select=true);
    void btnPowerSelect(bool select=true);

};


#endif
