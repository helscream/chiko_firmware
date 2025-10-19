#include "chiko_LED.h"


LEDStates _currentLEDState = LED_BREATHING;


void statusLedTask(void* parameter) {
  ledcAttachChannel(LED_PIN, 5000, 8, 7); // 5 kHz PWM, 8-bit resolution
  while (true) {
    switch (_currentLEDState)
    {
    case LED_OFF:
      /* code */
      ledcWrite(LED_PIN, 0);
      break;
    case LED_ON_SOLID:
      ledcWrite(LED_PIN, 255);
      break;
    case LED_DOUBLE_BLINK_FADE:
      ledcWrite(LED_PIN, 255);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ledcWrite(LED_PIN, 0);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ledcWrite(LED_PIN, 255);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ledcWrite(LED_PIN, 0);
      vTaskDelay(700 / portTICK_PERIOD_MS);
      break;
    case LED_SINGLE_BLINK_FADE:
      ledcWrite(LED_PIN, 255);
      vTaskDelay(100 / portTICK_PERIOD_MS);
      ledcWrite(LED_PIN, 0);
      vTaskDelay(900 / portTICK_PERIOD_MS);
      break;
    case LED_BREATHING:
      for (int brightness = 0; brightness <= 255; brightness += 5) {
        ledcWrite(LED_PIN, brightness);
        vTaskDelay(30 / portTICK_PERIOD_MS);
      }
      for (int brightness = 255; brightness >= 0; brightness -= 5) {
        ledcWrite(LED_PIN, brightness);
        vTaskDelay(30 / portTICK_PERIOD_MS);
      }
      break;
    default:
      break;
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Small delay to avoid watchdog reset
  }
  vTaskDelete(NULL); // Delete this task if it ever exits (it won't)
}


void initilize_LED(){
        // Setup status LED task
    xTaskCreatePinnedToCore(
      statusLedTask,   // Function to implement the task
      "StatusLedTask", // Name of the task
      1000,             // Stack size in words
      NULL,             // Task input parameter
      1,                // Priority of the task
      NULL,             // Task handle
      1);               // Core where the task should run
}

void setLEDState(LEDStates currentLEDState){
    _currentLEDState = currentLEDState;
}

LEDStates getLEDState(){
    return _currentLEDState;
}