# ESP32Servo

A lightweight C++ library to control servo motors with PWM using ESP32's built-in `ledc` module.  
This library allows you to control multiple servo motors with customizable resolution and frequency.

## 📦 Features

- Supports multiple servos (each on its own PWM channel)
- Customizable PWM frequency (default: 50 Hz)
- Customizable resolution (default: 12 bits)
- Control via microseconds (`writeMicroseconds(1500)`) or angle (`write(90)`)

## 📁 Project Structure

```text
ESP32Servo/
├── src/
│   ├── ESP32Servo.h
│   └── ESP32Servo.cpp
├── examples/
│   └── sweep/
│       └── sweep.ino
├── README.md
├── library.properties
```

## 🚀 Installation

Copy the `ESP32Servo` folder into your Arduino `libraries` directory or install it as a PlatformIO library.

## 🧰 Usage

### Include the library

```cpp
#include <ESP32Servo.h>
```

### Example: Controlling a single servo

```cpp
#include <ESP32Servo.h>

ESP32Servo myServo;

void setup() {
  Serial.begin(115200);
  ESP32Servo::setFrequency(50);     // Set frequency to 50Hz (typical for servos)
  ESP32Servo::setResolution(12);    // Set resolution to 12 bits

  myServo.attach(13, 0);            // Attach servo to pin 13, channel 0
  myServo.writeMicroseconds(1500);  // Center the servo
}

void loop() {
  myServo.writeMicroseconds(1000);  // Move to one end
  delay(1000);
  myServo.writeMicroseconds(2000);  // Move to the other end
  delay(1000);
}
```

### Example: Sweep (see `examples/sweep/sweep.ino`)

```cpp
#include <ESP32Servo.h>

ESP32Servo servo;

void setup() {
  Serial.begin(115200);
  servo.attach(13, 0);
  servo.setFrequency(50);
  servo.setResolution(12);
  servo.write(0);
  delay(1000);
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 10) {
    servo.write(angle);
    delay(500);
  }
  for (int angle = 180; angle >= 0; angle -= 10) {
    servo.write(angle);
    delay(500);
  }
}
```

### Controlling Multiple Servos

Just create multiple instances of `ESP32Servo`, each with its own pin and channel.

## 🛠️ API Reference

| Method                  | Description                                       |
| ----------------------- | ------------------------------------------------- |
| `attach(pin, channel)`  | Attach servo to GPIO pin and PWM channel          |
| `write(angle)`          | Set servo position by angle (0–180°)              |
| `writeMicroseconds(us)` | Send pulse width in microseconds (e.g. 500–2500)  |
| `setFrequency(freqHz)`  | Set global PWM frequency                          |
| `setResolution(bits)`   | Set global PWM resolution in bits (e.g. 8–16)     |
| `getAngle()`            | Get last written angle                            |

## 🧪 Tested On

- ESP32 DEVKIT DOIT V1
- Arduino IDE & PlatformIO

## 📜 License

MIT License
