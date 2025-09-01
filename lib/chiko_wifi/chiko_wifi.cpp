#include "chiko_wifi.h"

// Replace with your network credentials
const char* ssid = "Bubblegum";
const char* password = "1234567890";

void WifiConnectionTask(void* parameter) {
    WiFi.mode(WIFI_STA);
  WiFi.setHostname("CHIKO");
  Serial.println("Scanning for available networks...");
  int n = WiFi.scanNetworks();
  bool ssidFound = false;
  for (int i = 0; i < n; ++i) {
    if (WiFi.SSID(i) == ssid) {
      ssidFound = true;
      break;
    }
  }
  if (ssidFound) {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("SSID not found. Starting Access Point mode.");
    WiFi.mode(WIFI_AP);
    WiFi.softAP("CHIKO_AP", "chiko_password");
    Serial.print("Access Point started. IP address: ");
    Serial.println(WiFi.softAPIP());
  }
  while(!isConnectedToInternet()) {
    Serial.println("Waiting for internet connection...");
    delay(5000);
  }
    Serial.println("Connected to the internet!");
    struct tm timeinfo;
    // Set your timezone offset and daylight offset as needed
    long gmtOffset_sec = 0; // Example: 0 for UTC
    int daylightOffset_sec = 0; // Example: 0 if no daylight saving
    if(getTimeFromNTP(&timeinfo, gmtOffset_sec, daylightOffset_sec)) {
      Serial.println("Time synchronized from NTP server:");
      Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    } else {
      Serial.println("Failed to synchronize time from NTP server.");
    }
  while (1) {
    delay(60000); // Check connection every 60 seconds
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected. Attempting reconnection...");
      WiFi.disconnect();
      WiFi.reconnect();
      int attempts = 0;
      while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        delay(1000);
        Serial.print(".");
        attempts++;
      }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nReconnected to WiFi!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
      } else {
        Serial.println("\nFailed to reconnect to WiFi.");
      }
    } else {
      Serial.println("WiFi still connected.");
    }   
  }
  vTaskDelete(NULL); // Delete this task when done
}

void initialize_Wifi() {
    xTaskCreatePinnedToCore(
      WifiConnectionTask,   // Function to implement the task
      "WifiConnectionTask", // Name of the task
      10000,             // Stack size in words
      NULL,             // Task input parameter
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
    Serial.println("Failed to obtain time from NTP server");
    return false;
  }
  return true;
}
