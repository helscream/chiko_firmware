#include <ESP32Servo.h>

// Create a servo object
ESP32Servo servo;

void setup() {
  Serial.begin(115200);
  
  // Attach the servo to GPIO 13 using channel 0
  servo.attach(13, 0);
  
  // Optional: Set frequency and resolution
  servo.setFrequency(50);     // Standard servo frequency (Hz)
  servo.setResolution(12);    // PWM resolution (bit), typical 12 bits for ESP32

  servo.write(0);
  delay(1000); // Wait for the servo to reach the initial position
  
}

void loop() {
  // Sweep the servo from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle += 10) {
    servo.write(angle);
    delay(500);
  }

  // Sweep back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle -= 10) {
    servo.write(angle);
    delay(500);
  }
}
