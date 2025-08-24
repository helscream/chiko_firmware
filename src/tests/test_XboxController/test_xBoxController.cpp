#include <Arduino.h>
#include <BLEGamepadClient.h>

#define VIBRATION_DURATION_MS 500

BLEController chiko_controller;

void onControlsUpdate(BLEControlsEvent& e) {
  Serial.printf("lx: %.2f, ly: %.2f, rx: %.2f, ry: %.2f",
    e.leftStickX, e.leftStickY, e.rightStickX, e.rightStickY);
    Serial.printf("lt: %.2f, rt: %.2f", e.leftTrigger, e.rightTrigger);
    Serial.printf("a: %d, b: %d, x: %d, y: %d", e.buttonA, e.buttonB, e.buttonX, e.buttonY);
    Serial.printf("lb: %d, rb: %d", e.leftBumper, e.rightBumper);
    Serial.printf("dpad up: %d, down: %d, left: %d, right: %d",
      e.dpadUp, e.dpadDown, e.dpadLeft, e.dpadRight);
    Serial.printf("left stick button: %d, right stick button: %d\n",
        e.leftStickButton, e.rightStickButton);
    
    // Example: trigger a vibration when pressing the A button
    if (e.buttonA) {
        BLEVibrationsCommand cmd;
        cmd.leftMotor = 1.0f;  // 1.0f = max power for the motor
        cmd.rightMotor = 0.0f;
        cmd.leftTriggerMotor = 0.0f;  // 0.5f = half power for the motor
        cmd.rightTriggerMotor = 0.0f;
        cmd.durationMs = VIBRATION_DURATION_MS;
        chiko_controller.writeVibrations(cmd);
        }
        if (e.buttonB)
        {
        BLEVibrationsCommand cmd;
        cmd.leftMotor = 0.0f;  
        cmd.rightMotor = 1.0f; // 1.0f = max power for the motor
        cmd.leftTriggerMotor = 0.0f;  // 0.5f = half power for the motor
        cmd.rightTriggerMotor = 0.0f;
        cmd.durationMs = VIBRATION_DURATION_MS;
        chiko_controller.writeVibrations(cmd);
        }
        if (e.buttonX)
        {   
        BLEVibrationsCommand cmd;
        cmd.leftMotor = 0.0f;
        cmd.rightMotor = 0.0f;
        cmd.leftTriggerMotor = 1.0f;  // 1.0f = max power for the motor
        cmd.rightTriggerMotor = 0.0f;
        cmd.durationMs = VIBRATION_DURATION_MS;
        chiko_controller.writeVibrations(cmd);
        }
        if (e.buttonY)
        {
        BLEVibrationsCommand cmd;
        cmd.leftMotor = 0.0f;
        cmd.rightMotor = 0.0f;
        cmd.leftTriggerMotor = 0.0f;
        cmd.rightTriggerMotor = 1.0f;  // 1.0f = max power for the motor
        cmd.durationMs = VIBRATION_DURATION_MS;
        chiko_controller.writeVibrations(cmd);
        }
}

void onConnect(NimBLEAddress address) {
  Serial.printf("controller connected, address: %s\n", address.toString().c_str());
}

void onDisconnect(NimBLEAddress address) {
  Serial.printf("controller disconnected, address: %s\n", address.toString().c_str());
}



void setup(void) {
  Serial.begin(115200);
  Serial.println("Starting Chiko bController example");
  if (chiko_controller.begin())
  {
    Serial.println("BLE initialized");
  } else {
    Serial.println("Failed to initialize BLE");
    while (1) {
      delay(1000);
    }   
  }
    // Register the event callbacks
  chiko_controller.onConnect(onConnect);
  chiko_controller.onDisconnect(onDisconnect);
  chiko_controller.onControlsUpdate(onControlsUpdate);
}

void loop() {
  // Nothing needed here, everything is handled in the callbacks
//  BLEControlsEvent lastControls;
//  if(chiko_controller.isConnected())
//   chiko_controller.readControls(lastControls);
  delay(50);
}
