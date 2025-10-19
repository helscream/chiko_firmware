#include "chiko_logs.h"
#include <Arduino.h> // Required for Serial object and Serial.begin(), Serial.printf()
#include <stdarg.h>  // Required for va_list, va_start, va_end
#include <cstdio>    // Required for vsnprintf

// Uncomment the line below to enable logging.
// You can also define CHIKO_LOGS_ENABLE in platformio.ini under build_flags.
#define CHIKO_LOGS_ENABLE

void chikoLog(const char *tag, const char * format, ...) {
#ifdef CHIKO_LOGS_ENABLE
    // Check if Serial is initialized. If not, initialize it.
    // This ensures that log messages can be printed even if Serial.begin()
    // was not explicitly called in setup().
    if (!Serial) {
        Serial.begin(SERIAL_BUADRATE);
    }

    // A buffer to hold the formatted log message.
    // Start with a small buffer on the stack.
    char loc_buf[64];
    char * temp = loc_buf; // Pointer to the buffer
    va_list arg;            // Argument list for vsnprintf
    va_list copy;           // Copy of argument list for length calculation

    va_start(arg, format);  // Initialize arg list
    va_copy(copy, arg);     // Copy arg list for safe re-use

    // Try to format the string into the stack buffer.
    // vsnprintf returns the number of characters that would have been written
    // if the buffer had been large enough, excluding the null terminator.
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy); // Clean up the copy of the argument list

    // Handle error case for vsnprintf
    if(len < 0) {
        va_end(arg); // Clean up the original argument list
        return;
    }

    // If the message is too long for the stack buffer, allocate memory on the heap.
    if(len >= sizeof(loc_buf)) {
        temp = (char*) malloc(len+1); // Allocate exactly enough space + null terminator
        if(temp == NULL) {
            // If malloc fails, we can't print the full message, so just return.
            va_end(arg);
            return;
        }
        // Format the string again into the newly allocated heap buffer.
        vsnprintf(temp, len+1, format, arg);
    }
    va_end(arg); // Clean up the original argument list

    // Print the formatted message to the Serial monitor, prefixed with the tag.
    Serial.printf("[%s] %s\n", tag, temp);

    // If heap memory was allocated, free it.
    if(temp != loc_buf) {
        free(temp);
    }
#endif // CHIKO_LOGS_ENABLE
}