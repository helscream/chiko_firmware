#include "chiko_wifi.h"

TaskHandle_t wifiConnectionTaskHandle;

const char LOG_TAG[] = "WIFI";

void WifiInfoPageTask(void *param){
  pages *_pages;
  _pages = (pages*)param;
  while (true)
  {
    if (_pages->getSelectedPageNumber() == WIFI_INFO_PN)
    {
      char buff[30];
      U8G2 _display = _pages->getDisplayHandel();
      pageConfig wifiPageConfig = _pages->getPageConfig(WIFI_INFO_PN);
      text wifiStatus, wifiIP, wifiConnType;

      _display.setDrawColor(0);
      _display.drawBox(0,14,128,32); //Clear 
      _display.setDrawColor(1);

      if (WiFi.getMode() == WIFI_STA){
        sprintf(buff,"Connection: Station Mode");
      }else if(WiFi.getMode() == WIFI_AP){
        sprintf(buff,"Connection: Access Point");
      }else{
        sprintf(buff,"Connection: Unknown");
      }
      wifiConnType.create(_display,buff,MIDTOP,64,14);

      if (WiFi.isConnected()){
        sprintf(buff,"%s (Connected)",wifi_SSID);
        
        wifiPageConfig.btnFunClickAction = [&wifiPageConfig,&_pages](){
          strcpy(wifiPageConfig.btnFunName, "Disable WiFi");
          _pages->setPageConfig(WIFI_INFO_PN,wifiPageConfig);
          _pages->updatePage(true);
          killWifiTask();
          WiFi.disconnect(true);
        };
      }else{
        sprintf(buff,"WiFi: Not Connected!");
        wifiPageConfig.btnFunClickAction = [&wifiPageConfig,&_pages](){
          strcpy(wifiPageConfig.btnFunName, "Enable WiFi");
          _pages->setPageConfig(WIFI_INFO_PN,wifiPageConfig);
          _pages->updatePage(true);
          startWifiTask();
        };
      }

      _pages->setPageConfig(WIFI_INFO_PN,wifiPageConfig);
      _pages->updatePage();

      wifiStatus.create(_display,buff,LEFTTOP,4,26);

      if (WiFi.isConnected() && WiFi.getMode() == WIFI_STA){
        sprintf(buff,"IP: %s",WiFi.localIP().toString());
      }else if(WiFi.getMode() == WIFI_AP){
        sprintf(buff,"IP: %s",WiFi.softAPIP().toString());
      }else{
        sprintf(buff,"IP: Not avaliable!");
      }
      wifiIP.create(_display,buff,LEFTTOP,4,38);
      _display.sendBuffer();
    delay(500);
  } 
}
}

void WifiConnectionTask(void* parameter) {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(CHIKO_NAME);
  chikoLog(LOG_TAG,"Scanning for available networks...");
  int n = WiFi.scanNetworks();
  bool ssidFound = false;
  for (int i = 0; i < n; ++i) {
    if (WiFi.SSID(i) == wifi_SSID) {
      ssidFound = true;
      break;
    }
  }
  if (ssidFound) {
    WiFi.begin(wifi_SSID, wifi_PASS);
    chikoLog(LOG_TAG,"Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    chikoLog(LOG_TAG,"WiFi connected!");
    chikoLog(LOG_TAG,"IP address: %s",WiFi.localIP().toString());
    
  } else {
    chikoLog(LOG_TAG,"SSID not found. Starting Access Point mode.");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("CHIKO_AP", "chiko_password");
    chikoLog(LOG_TAG,"Access Point started. IP address: %s",WiFi.softAPIP().toString());
  }
  while(!isConnectedToInternet()) {
    chikoLog(LOG_TAG,"Waiting for internet connection...");
    delay(5000);
  }
    chikoLog(LOG_TAG,"Connected to the internet!");
    
    
    
  while (1) {
    delay(60000); // Check connection every 60 seconds
    if (WiFi.status() != WL_CONNECTED) {
      chikoLog(LOG_TAG,"WiFi disconnected. Attempting reconnection...");
      WiFi.disconnect();
      WiFi.reconnect();
      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        delay(1000);
        Serial.print(".");
        attempts++;
      }
      if (WiFi.status() == WL_CONNECTED) {
        chikoLog(LOG_TAG,"Reconnected to WiFi!");
        chikoLog(LOG_TAG,"IP address: %s",WiFi.localIP().toString());

      } else {
        chikoLog(LOG_TAG,"Failed to reconnect to WiFi.");
      }
    } else {
      chikoLog(LOG_TAG,"WiFi still connected.");
    }   
  }
  vTaskDelete(NULL); // Delete this task when done
}

void startWifiTask(){
  xTaskCreatePinnedToCore(
      WifiConnectionTask,   // Function to implement the task
      "WifiConnectionTask", // Name of the task
      10000,             // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      &wifiConnectionTaskHandle, // Task handle
      1);               // Core where the task should run
}

void killWifiTask(){
  vTaskDelete(wifiConnectionTaskHandle);
}


void initialize_Wifi(pages *_pages) {
    
    startWifiTask();

    xTaskCreatePinnedToCore(
      WifiInfoPageTask,   // Function to implement the task
      "WifiInfoPageTask", // Name of the task
      5000,             // Stack size in words
      _pages,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
}

// void broadcastData(const char* data, uint16_t port) {
//   WiFiUDP udp;
//   IPAddress broadcastIp = ~WiFi.subnetMask() | WiFi.gatewayIP();
//   udp.beginPacket(broadcastIp, port);
//   udp.write((const uint8_t*)data, strlen(data));
//   udp.endPacket();
// }

// Checks if the device is connected to the internet by pinging a known server (e.g., 8.8.8.8)
bool isConnectedToInternet() {
  WiFiClient client;
  // Try to connect to Google DNS on port 53
  if (client.connect("8.8.8.8", 53)) {
    client.stop();
    return true;
  }
  return false;
}

// Gets the current time from an NTP server and fills the provided struct tm with the result.
// Returns true if successful, false otherwise.
bool getTimeFromNTP(struct tm* timeinfo, long gmtOffset_sec, int daylightOffset_sec) {
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
  if (!getLocalTime(timeinfo, 10000)) { // 10 second timeout
    chikoLog(LOG_TAG,"Failed to obtain time from NTP server");
    return false;
  }
  return true;
}
