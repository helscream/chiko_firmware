#include "chiko_logs.h"
#include <Arduino.h>
#include <stdarg.h>
#include <cstdio> // Required for vsnprintf

void chikoLog(const char *tag, const char * format, ...) {
#ifdef CHIKO_LOGS_ENABLE
    if (!Serial) { // Check if Serial is initialized
        Serial.begin(SERIAL_BUADRATE);
    }

    char loc_buf[64];
    char * temp = loc_buf;
    va_list arg;
    va_list copy;
    va_start(arg, format);
    va_copy(copy, arg);
    int len = vsnprintf(temp, sizeof(loc_buf), format, copy);
    va_end(copy);
    if(len < 0) {
        va_end(arg);
        return;
    }
    if(len >= sizeof(loc_buf)) {
        temp = (char*) malloc(len+1);
        if(temp == NULL) {
            va_end(arg);
            return;
        }\
        vsnprintf(temp, len+1, format, arg);
    }
    va_end(arg);
    Serial.printf("[%s] %s\n", tag, temp); // Print with tag
    if(temp != loc_buf) {
        free(temp);
    }
#endif
}
