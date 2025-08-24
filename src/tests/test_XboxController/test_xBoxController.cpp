// #include <Arduino.h>
// #include <BLEGamepadClient.h>

// #define VIBRATION_DURATION_MS 500

// BLEController chiko_controller;

// void onControlsUpdate(BLEControlsEvent& e) {
//   Serial.printf("lx: %.2f, ly: %.2f, rx: %.2f, ry: %.2f",
//     e.leftStickX, e.leftStickY, e.rightStickX, e.rightStickY);
//     Serial.printf("lt: %.2f, rt: %.2f", e.leftTrigger, e.rightTrigger);
//     Serial.printf("a: %d, b: %d, x: %d, y: %d", e.buttonA, e.buttonB, e.buttonX, e.buttonY);
//     Serial.printf("lb: %d, rb: %d", e.leftBumper, e.rightBumper);
//     Serial.printf("dpad up: %d, down: %d, left: %d, right: %d",
//       e.dpadUp, e.dpadDown, e.dpadLeft, e.dpadRight);
//     Serial.printf("left stick button: %d, right stick button: %d\n",
//         e.leftStickButton, e.rightStickButton);
    
//     // Example: trigger a vibration when pressing the A button
//     if (e.buttonA) {
//         BLEVibrationsCommand cmd;
//         cmd.leftMotor = 1.0f;  // 1.0f = max power for the motor
//         cmd.rightMotor = 0.0f;
//         cmd.leftTriggerMotor = 0.0f;  // 0.5f = half power for the motor
//         cmd.rightTriggerMotor = 0.0f;
//         cmd.durationMs = VIBRATION_DURATION_MS;
//         chiko_controller.writeVibrations(cmd);
//         }
//         if (e.buttonB)
//         {
//         BLEVibrationsCommand cmd;
//         cmd.leftMotor = 0.0f;  
//         cmd.rightMotor = 1.0f; // 1.0f = max power for the motor
//         cmd.leftTriggerMotor = 0.0f;  // 0.5f = half power for the motor
//         cmd.rightTriggerMotor = 0.0f;
//         cmd.durationMs = VIBRATION_DURATION_MS;
//         chiko_controller.writeVibrations(cmd);
//         }
//         if (e.buttonX)
//         {   
//         BLEVibrationsCommand cmd;
//         cmd.leftMotor = 0.0f;
//         cmd.rightMotor = 0.0f;
//         cmd.leftTriggerMotor = 1.0f;  // 1.0f = max power for the motor
//         cmd.rightTriggerMotor = 0.0f;
//         cmd.durationMs = VIBRATION_DURATION_MS;
//         chiko_controller.writeVibrations(cmd);
//         }
//         if (e.buttonY)
//         {
//         BLEVibrationsCommand cmd;
//         cmd.leftMotor = 0.0f;
//         cmd.rightMotor = 0.0f;
//         cmd.leftTriggerMotor = 0.0f;
//         cmd.rightTriggerMotor = 1.0f;  // 1.0f = max power for the motor
//         cmd.durationMs = VIBRATION_DURATION_MS;
//         chiko_controller.writeVibrations(cmd);
//         }
// }

// void onConnect(NimBLEAddress address) {
//   Serial.printf("controller connected, address: %s\n", address.toString().c_str());
// }

// void onDisconnect(NimBLEAddress address) {
//   Serial.printf("controller disconnected, address: %s\n", address.toString().c_str());
// }



// void setup(void) {
//   Serial.begin(115200);
//   Serial.println("Starting Chiko bController example");
//   if (chiko_controller.begin())
//   {
//     Serial.println("BLE initialized");
//   } else {
//     Serial.println("Failed to initialize BLE");
//     while (1) {
//       delay(1000);
//     }   
//   }
//     // Register the event callbacks
//   chiko_controller.onConnect(onConnect);
//   chiko_controller.onDisconnect(onDisconnect);
//   chiko_controller.onControlsUpdate(onControlsUpdate);
// }

// void loop() {
//   // Nothing needed here, everything is handled in the callbacks
// //  BLEControlsEvent lastControls;
// //  if(chiko_controller.isConnected())
// //   chiko_controller.readControls(lastControls);
//   delay(50);
// }

#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_hidh.h"

static const char *TAG = "XboxHID";

// Callback for HID events
void hidh_event_callback(esp_hidh_event_t event, esp_hidh_event_data_t *param) {
    switch (event) {
        case ESP_HIDH_OPEN_EVENT:
            ESP_LOGI(TAG, "Controller connected");
            break;

        case ESP_HIDH_INPUT_EVENT: {
            ESP_LOGI(TAG, "Input report received");

            const uint8_t *data = param->input.data;
            const int len = param->input.length;

            // Example: Print raw input data
            ESP_LOG_BUFFER_HEX(TAG, data, len);

            // Example: Check if A button is pressed (depends on report format)
            if (len > 2 && (data[2] & 0x10)) { // Bitmask for A button
                ESP_LOGI(TAG, "A button pressed!");

                // Send vibration feedback
                uint8_t rumble_report[] = {
                    0x09,       // Report ID (may vary)
                    0x00,       // Sequence
                    0x00, 0x00, // Left motor (low freq)
                    0xFF, 0xFF, // Right motor (high freq)
                    0x00, 0x00, 0x00, 0x00 // Additional bytes (depends on controller)
                };

                esp_err_t ret = esp_hidh_dev_output_set(param->input.dev, rumble_report, sizeof(rumble_report));
                if (ret == ESP_OK) {
                    ESP_LOGI(TAG, "Vibration sent");
                } else {
                    ESP_LOGE(TAG, "Failed to send vibration: %s", esp_err_to_name(ret));
                }
            }
            break;
        }

        case ESP_HIDH_CLOSE_EVENT:
            ESP_LOGI(TAG, "Controller disconnected");
            break;

        default:
            ESP_LOGI(TAG, "Unhandled HID event: %d", event);
            break;
    }
}

void setup(void) {
    esp_err_t ret;

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth controller init failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BTDM);
    if (ret) {
        ESP_LOGE(TAG, "Bluetooth controller enable failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid init failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_enable();
    if (ret) {
        ESP_LOGE(TAG, "Bluedroid enable failed: %s", esp_err_to_name(ret));
        return;
    }

    ret = esp_hidh_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "HID host init failed: %s", esp_err_to_name(ret));
        return;
    }

    esp_hidh_register_callback(hidh_event_callback);

    ESP_LOGI(TAG, "HID host initialized. Turn on your Xbox controller...");
}


void loop() {
    // Nothing needed here, everything is handled in the callbacks
}