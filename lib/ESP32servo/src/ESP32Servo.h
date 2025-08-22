#ifndef ESP32SERVO_H
#define ESP32SERVO_H

#include <Arduino.h>

class ESP32Servo {
public:
    ESP32Servo();

    void attach(uint8_t pin, uint8_t channel);
    void write(int angle);
    void writeMicroseconds(uint16_t microseconds);
    int getAngle() const;

    static void setResolution(uint8_t resolutionBits);
    static void setFrequency(uint16_t frequencyHz);

private:
    uint8_t _pin;
    uint8_t _channel;
    int _angle;

    static uint8_t _resolutionBits;
    static uint16_t _frequencyHz;
};

#endif
