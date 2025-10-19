#ifndef __CHIKO_LOGS__
#define __CHIKO_LOGS__

#include <chiko_definations.h>

// Define the baud rate for serial communication.
// This will be used to initialize Serial if it's not already started.
#define SERIAL_BUADRATE     115200

/**
 * @brief Custom logging function similar to ESP_LOGI.
 *        This function prints formatted messages to the Serial monitor.
 *        It automatically initializes Serial if it hasn't been started yet,
 *        using the SERIAL_BUADRATE defined above.
 *
 * @param tag A string literal tag (e.g., "WIFI", "MAIN", "SENSOR") to categorize the log message.
 *            This tag will be printed before the actual message, like "[TAG] Your message".
 * @param format The format string for the log message (printf-style).
 * @param ... Variable arguments for the format string, matching the specifiers in 'format'.
 *
 * @note To enable or disable logging, you can define or undefine `CHIKO_LOGS_ENABLE`
 *       in your `platformio.ini` or a global header file.
 *       Example usage:
 *       chikoLog("MAIN", "Application started, version: %s", "1.0.0");
 *       chikoLog("WIFI", "Connected to %s, IP: %s", ssid, WiFi.localIP().toString().c_str());
 */
void chikoLog(const char *tag, const char * format, ...);


#endif