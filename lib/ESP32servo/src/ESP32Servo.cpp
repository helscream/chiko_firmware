#include "ESP32Servo.h"

// Initialize static variables
uint8_t ESP32Servo::_resolutionBits = 12;      // Default resolution: 12 bits
uint16_t ESP32Servo::_frequencyHz = 50;        // Default servo frequency: 50Hz

ESP32Servo::ESP32Servo() {
    _pin = 255;      // Unused pin
    _channel = 255;  // Unused channel
    _angle = 0;
}

void ESP32Servo::setResolution(uint8_t resolutionBits) {
    _resolutionBits = resolutionBits;
}

void ESP32Servo::setFrequency(uint16_t frequencyHz) {
    _frequencyHz = frequencyHz;
}

void ESP32Servo::attach(uint8_t pin, uint8_t channel) {
    _pin = pin;
    _channel = channel;

    // Initialize PWM with the specified resolution and frequency
    ledcSetup(_channel, _frequencyHz, _resolutionBits);
    ledcAttachPin(_pin, _channel);
}

void ESP32Servo::writeMicroseconds(uint16_t microseconds) {
    // Check if the servo was attached
    if (_pin == 255 || _channel == 255) return;

    // Convert microseconds to duty cycle value for 16-bit resolution
    // Total PWM period = 20 ms = 20000 µs
    // Duty = (microseconds / 20000) * 2^16
    uint32_t maxDuty = (1 << _resolutionBits);  // 65536 for 16-bit
    uint32_t duty = (uint32_t)microseconds * maxDuty / 20000;

    ledcWrite(_channel, duty);
}

void ESP32Servo::write(int angle) {
    // Check if the servo is attached
    if (_pin == 255 || _channel == 255) return;

    angle = constrain(angle, 0, 180);

    // Typical servo pulse: 0.5 ms (0°) to 2.5 ms (180°)
    const uint16_t pulseMin = 500;   // Minimum pulse width in microseconds
    const uint16_t pulseMax = 2500;  // Maximum pulse width in microseconds

    // Calculate PWM period in microseconds based on configured frequency
    uint32_t period = 1000000UL / _frequencyHz;

    // Map angle to pulse width
    uint32_t pulseWidth = map(angle, 0, 180, pulseMin, pulseMax);

    // Calculate duty cycle based on resolution and period
    uint32_t maxDuty = (1UL << _resolutionBits);
    uint32_t duty = (pulseWidth * maxDuty) / period;

    ledcWrite(_channel, duty);
    _angle = angle;
}

int ESP32Servo::getAngle() const {
    return _angle;
}
