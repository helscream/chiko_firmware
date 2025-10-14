#include "chiko_displayUI.h"
#include <math.h> // for roundf if needed
#include <string.h>


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
    u8g2.setFont(u8g2_font_6x10_mf);
    u8g2.drawStr(0, 10, getBluetoothStatus().c_str());
    u8g2.drawStr(0, 20, getWiFiStatus().c_str());

    int batteryWidth = u8g2.getStrWidth(getBatteryStatus().c_str());
    u8g2.drawStr(u8g2.getDisplayWidth() - batteryWidth, 10, getBatteryStatus().c_str());
}

void DisplayUI::drawCenterInfo() {
    u8g2.setFont(u8g2_font_6x10_mf);
    int y = u8g2.getDisplayHeight() / 2 - 10;
    u8g2.drawStr(10, y, getUniqueID().c_str());

    String ssid = WiFi.SSID();
    if (ssid.length() > 0) {
        u8g2.drawStr(10, y + 15, ("SSID: " + ssid).c_str());
    }

    u8g2.drawStr(10, y + 30, ("Name: " + name).c_str());
}

void DisplayUI::drawBottomButtons() {
    u8g2.setFont(u8g2_font_6x10_mf);
    u8g2.drawStr(0, u8g2.getDisplayHeight() - 2, "Function");

    int resetWidth = u8g2.getStrWidth("Reset");
    u8g2.drawStr(u8g2.getDisplayWidth() - resetWidth, u8g2.getDisplayHeight() - 2, "Reset");
}






// --- Public API ---

// Pages API

void pageUpdateTask(void *param){
  pages *obj;
  obj = (pages*)param;
  while (1)
  {
    obj->updatePage();
    delay(10);
  }
  vTaskDelete(NULL);
}

void pages::intilize(U8G2 &display, uint8_t pageCount){
  _pageCount = pageCount;
  _display = display;
  _pageTaskBar.create(_display, _pageCount);

  xTaskCreatePinnedToCore(
      pageUpdateTask,     // Function to implement the task
      "PageUpdateTask",   // Name of the task
      10000,              // Stack size in words
      this,               // Task input parameter
      2,                  // Priority of the task
      NULL,               // Task handle
      1);                 // Core where the task should run

  _currentPage = (0);
}

void pages::setPageConfig(uint8_t pageNumber, pageConfig config){
  _pageConfigs[pageNumber] = config;
}

pageConfig pages::getPageConfig(uint8_t pageNumber){
  return _pageConfigs[pageNumber];
}

uint8_t pages::getSelectedPageNumber(){
  return _currentPage;
}

uint8_t pages::getPageCount(){
  return _pageCount;
}

void pages::gotoNextPage(){
  uint8_t currentPage = getSelectedPageNumber();
  if (currentPage++ > _pageCount){
    _currentPage = (0);
  }
  else{
    _currentPage = (currentPage++);
  }
}

void pages::gotoPreviousPage(){
  uint8_t currentPage = getSelectedPageNumber();
  if (currentPage-- < 0){
    _currentPage = (_pageCount-1);
  }
  else{
    _currentPage = (currentPage--);
  }
}


void pages::gotoPageNumber(uint8_t pageNumber){
  uint8_t currentPage = getSelectedPageNumber();
  if (pageNumber >= 0 || pageNumber < _pageCount){
    _currentPage = (pageNumber);
  }
  else{
    _currentPage = (0);
  }
}

void pages::gotoDefaultPage(){
  _currentPage = (0);
}

void pages::updatePage(bool forceUpdate){
  if (_previousPage != _currentPage || forceUpdate){
    _display.clearDisplay();
  
  if (!_pageConfigs[_currentPage].taskBarVisibility){
    _pageTaskBar.clear();
  }else{
    _pageTaskBar.select(_currentPage);
    _pageTaskBar.update();
  }

  if (!_pageConfigs[_currentPage].btnPwVisibility){
    _power.clear();
  }else{
    _power.create(_display,_pageConfigs[_currentPage].btnPwName,MIDBOTTOM,64,62);
    _power.update();
  }
  
  if (!_pageConfigs[_currentPage].btnRstVisibility){
    _reset.clear();
  }else{
    _reset.create(_display,_pageConfigs[_currentPage].btnRstName,RIGHTBOTTOM,126,62);
    _reset.update();
  }
  
  if (!_pageConfigs[_currentPage].btnFunVisibility){
    _fun.clear();
  }else{
    _fun.create(_display,_pageConfigs[_currentPage].btnFunName,LEFTBOTTOM,2,62);
    _fun.update();
  }
  }
  _previousPage = _currentPage;
}

void pages::btnFunctionAction(){
  if (_pageConfigs[_currentPage].btnFunClickAction != nullptr){
      chikoLog(LOG_TAG, "Executing Function button action linked to page # %d",_currentPage);
    _pageConfigs[_currentPage].btnFunClickAction();
  }else{
    chikoLog(LOG_TAG, "No Function assigned!");
  }
}


void pages::btnPowerAction(){
  if (_pageConfigs[_currentPage].btnPwClickAction != nullptr){
    chikoLog(LOG_TAG, "Executing Power button action linked to page # %d",_currentPage);
    _pageConfigs[_currentPage].btnPwClickAction();
  }else{
    chikoLog(LOG_TAG, "No Function assigned!");
  }
}

void pages::btnPowerSelect(bool select){
  if (_pageConfigs[_currentPage].btnPwVisibility == true){
    _power.select(select);
  }
}

void pages::btnFunctionSelect(bool select){
  if (_pageConfigs[_currentPage].btnFunVisibility == true){
    _fun.select(select);
  }
}
