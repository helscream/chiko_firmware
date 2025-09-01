

#include <chikobot.h>
#include <chiko_wifi.h>

#define LED_PIN 16
#define BOST_CONVERTER_ENABLE_PIN 17
#define CHARGE_INDICATOR_PIN 36
#define STANDBY_PIN 39


void setup() {
  Serial.begin(115200);
  Serial.println("Hello Chiko");
  delay(1000); // Wait for serial monitor to connect

  // Initialize ChikoBot components
	initilize_chikobot();

  
  // pinMode(LED_PIN, OUTPUT);
    // pinMode(BOST_CONVERTER_ENABLE_PIN, INPUT_PULLUP);
    // pinMode(CHARGE_INDICATOR_PIN, INPUT_PULLUP);
    // pinMode(STANDBY_PIN, INPUT_PULLUP);

    // digitalWrite(BOST_CONVERTER_ENABLE_PIN, HIGH); // Enable boost converter
    // digitalWrite(LED_PIN, HIGH); // Turn on LED to indicate power on
    
}

void loop() {

    // Spinner animation: cycles through | / - \ characters
  //   static int spinnerIndex = 0;
  //   const char spinnerChars[] = {'|', '/', '-', '\\'};
  //   Serial.print("\r"); // Return to start of line
  //   Serial.print(spinnerChars[spinnerIndex]);
  // char voltageStr[8];
  // snprintf(voltageStr, sizeof(voltageStr), "%.2f V", readBatteryVoltage());
  // facePrintMiddle(voltageStr, true, 18); // Display battery voltage on face
  //   spinnerIndex = (spinnerIndex + 1) % 4; // Move to next spinner character
    // if (spinnerIndex == 0) {
    //   facePrint('\n'); // Clear face display after full cycle
    // }
    delay(100); // Delay to control spinner speed

  // Serial.println("ADC pin 35:" + String(analogRead(35)));
  // Serial.println("ADC pin 34:" + String(analogRead(34)));
  // Serial.println("ADC pin 39:" + String(analogRead(39)));
  // Serial.println("ADC pin 36:" + String(analogRead(36)));
  // Serial.println("\t --- ");
  // broadcastData("12355");
  // delay(1000);
  static int counter = 0;
  counter = random(0, 100);
  if (counter == 50)
  {
    eyes_blink(12);
  }else if (counter == 25)
  {
    eyes_move_right_big();
  }else if (counter == 75)
  {
    eyes_move_left_big();
  }
    
  eyes_reset(true);
  
}
