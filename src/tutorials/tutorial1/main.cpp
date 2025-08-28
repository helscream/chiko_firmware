
// This sketch demonstrates how to retrieve and display detailed ESP32 chip information
// using the Arduino framework. It prints out various hardware and firmware details
// to the serial monitor, and then runs a simple spinner animation in the main loop.


#include <Arduino.h>            // Core Arduino functions
#include <esp_chip_info.h>      // ESP32 chip information structure and function



// The setup() function runs once at startup. It initializes serial communication
// and prints detailed information about the ESP32 chip and environment.
void setup() {
  // Initialize serial communication at 115200 baud rate for debugging output
  Serial.begin(115200);
  Serial.println("Hello Chiko"); // Greeting message
  delay(1000); // Wait for serial monitor to connect

  // Structure to hold chip information
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info); // Populate chip_info with current chip data

  // Print basic chip features: core count, WiFi, Bluetooth, BLE
  Serial.printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
          chip_info.cores,
          (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
          (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  // Print silicon revision
  Serial.printf("silicon revision %d, ", chip_info.revision);

  // Print flash size and type (embedded or external)
  Serial.printf("%dMB %s flash\n", ESP.getFlashChipSize() / (1024 * 1024),
          (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  // Print minimum free heap size (useful for memory debugging)
  Serial.printf("Minimum free heap size: %d bytes\n", ESP.getMinFreeHeap());

  // Print additional chip and environment details
  Serial.printf("Chip Model: %s\n", ESP.getChipModel());
  Serial.printf("Chip Cores: %d\n", ESP.getChipCores());
  Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());
  Serial.printf("Core version: %s\n", ESP.getCoreVersion());
  Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Flash Chip Size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
  Serial.printf("Flash Chip Speed: %d MHz\n", ESP.getFlashChipSpeed() / 1000000);
  // Print flash chip mode (QIO, QOUT, DIO, DOUT, or UNKNOWN)
  Serial.printf("Flash Chip Mode: %s\n",
    (ESP.getFlashChipMode() == FM_QIO) ? "QIO" :
    (ESP.getFlashChipMode() == FM_QOUT) ? "QOUT" :
    (ESP.getFlashChipMode() == FM_DIO) ? "DIO" :
    (ESP.getFlashChipMode() == FM_DOUT) ? "DOUT" : "UNKNOWN");
  Serial.printf("Sketch Size: %d bytes\n", ESP.getSketchSize());  // Current sketch size
  Serial.printf("Free Sketch Space: %d bytes\n", ESP.getFreeSketchSpace()); // Available space for new sketch
  Serial.printf("Sketch MD5: %s\n", ESP.getSketchMD5().c_str()); // MD5 hash of the sketch for integrity check
}

// The loop() function runs repeatedly after setup().
// Here, it displays a simple spinner animation in the serial monitor
// by cycling through four characters. This demonstrates how to use
// carriage return (\r) to overwrite the same line in the serial output.
void loop() {
  // Print spinner animation to indicate the program is running
  Serial.print("\r-");   // Print '-' and return to line start
  delay(500);            // Wait 500 ms
  Serial.print("\r\\");  // Print '\' (escaped) and return
  delay(500);
  Serial.print("\r|");   // Print '|' and return
  delay(500);
  Serial.print("\r/");   // Print '/' and return
  delay(500);
  // The animation repeats indefinitely
}