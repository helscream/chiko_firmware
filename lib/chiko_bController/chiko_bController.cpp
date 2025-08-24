#include "chiko_bController.h"
#include <RTOS.h>

// Store up to the max number of connected controllers


void (*onControllerConnect)() = NULL, (*onControllerDisconnect)() = NULL, (*ControllerActions)() = NULL;

TaskHandle_t ControllerReadTaskHandle;


static void ControllerReadTask(void* param) {
  while (1) {

    // Check current connection state
    bool nowConnected = controller.isConnected();

    // --- onConnect edge ---
    if (nowConnected && !wasConnected) {

      // Short hello rumble: medium main motors, light triggers, ~200 ms
      sendVibration(/*left*/ 180, /*right*/ 180, /*LT*/ 50, /*RT*/ 50, /*ms*/ 200, /*cycles*/ 1);

      onControllerConnect();
    }
    // --- onDisconnect edge ---
    else if (!nowConnected && wasConnected) {
      onControllerDisconnect();
    }

    // When connected, read the latest controls
    if (nowConnected) {
      controller.readControls(lastControls);  // fills struct with current state
      ControllerActions();
    }


    // Remember state for the next iteration
    wasConnected = nowConnected;

    // Small sleep; BLE-Gamepad-Client polling doesn’t require tight loops
    delay(5);
  }
}

void initialize_bController(void (*onConnect)(), void (*onDisconnect)(), void (*readController)()) {

  // Start BLE and begin scanning for gamepads (Xbox supported)
  controller.begin();

  Serial.println("BLE-Gamepad-Client started. "
                 "Put the Xbox controller into pairing mode to connect.");

  onControllerConnect = onConnect;
  onControllerDisconnect = onDisconnect;
  ControllerActions = readController;

  xTaskCreate(ControllerReadTask, "Controller update", 5000, NULL, 3, &ControllerReadTaskHandle);
}



void printControllerInfo() {
  // Example: print sticks & triggers at ~10Hz
  static uint32_t lastPrint = 0;
  uint32_t now = millis();
  if (now - lastPrint > 100) {
    lastPrint = now;
    Serial.printf("LX:%+.2f LY:%+.2f  RX:%+.2f RY:%+.2f  "
                  "LT:%.2f RT:%.2f  A:%d B:%d X:%d Y:%d  LB:%d RB:%d\n",
                  lastControls.leftStickX, lastControls.leftStickY,
                  lastControls.rightStickX, lastControls.rightStickY,
                  lastControls.leftTrigger, lastControls.rightTrigger,
                  lastControls.buttonA, lastControls.buttonB,
                  lastControls.buttonX, lastControls.buttonY,
                  lastControls.leftBumper, lastControls.rightBumper);
  }
}


// ---- Vibration helper -------------------------------------------------------
// Sends a rumble pulse. Intensities are 0..255 (0 = off, 255 = max).
// durationMs is capped internally by the lib around 2550 ms.
void sendVibration(uint8_t leftMain, uint8_t rightMain,
                          uint8_t leftTrigger, uint8_t rightTrigger,
                          uint16_t durationMs, uint8_t cycles) {
  if (!controller.isConnected()) return;

  BLEVibrationsCommand cmd{};
  // Common fields exposed by the library:
  // - leftMotor, rightMotor: main rumble motors
  // - leftTriggerMotor, rightTriggerMotor: trigger rumble (Series X|S)
  // - durationMs: pulse length (max ~2550 ms)
  // - cycles: repeat count
  cmd.leftMotor = leftMain;
  cmd.rightMotor = rightMain;
  cmd.leftTriggerMotor = leftTrigger;
  cmd.rightTriggerMotor = rightTrigger;
  cmd.durationMs = durationMs;
  cmd.cycles = cycles;

  // Library call to send the vibration command to the connected pad.
  // Depending on library version, this may be named `sendVibrations(...)` or `vibrate(...)`.
  // If your IDE flags an unknown method, open BLEVibrationsCommand.h / BLEController.h
  // in your installed library for the exact name and adjust here.
  controller.writeVibrations(cmd);
}


// --- Utilities ---------------------------------------------------------------

float dz(float v) {
  return (fabsf(v) < DEADZONE) ? 0.0f : v;
}
