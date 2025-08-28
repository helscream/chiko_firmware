
#include <chikobot.h>


void setup() {
  Serial.begin(115200);
  Serial.println("Hello Chiko");
  delay(1000); // Wait for serial monitor to connect

  // Initialize ChikoBot components
	initilize_chikobot();
  
}

void loop() {

    // Spinner animation: cycles through | / - \ characters
    static int spinnerIndex = 0;
    const char spinnerChars[] = {'|', '/', '-', '\\'};
    Serial.print("\r"); // Return to start of line
    Serial.print(spinnerChars[spinnerIndex]);
    spinnerIndex = (spinnerIndex + 1) % 4; // Move to next spinner character
    delay(100); // Delay to control spinner speed
}
