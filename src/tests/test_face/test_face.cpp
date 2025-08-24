#include <Arduino.h>
#include <chiko_face.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Chiko Face Emoji");
  initFaceEmoji();
  testFaceEmoji();
}

void loop() {
  // put your main code here, to run repeatedly:
  

}