#include <Arduino.h>
#include <chiko_face.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Chiko Face Emoji");
  initializes_eyes();
  delay(1000);
  eyes_wakeup();
}

void loop() {
  // put your main code here, to run repeatedly:
  // eyes_wakeup();
  // delay(5000);  

  // eyes_sleep();
  // delay(5000);

  // eyes_blink(12);
  // delay(5000);

  // eyes_happy();
  // delay(5000); 
  uint16_t refRate = 20; // ms  

  eyes_blink(12);
  delay(2000);
  eyes_blink(12);
  eyes_reset(true);
  delay(5000);

  eyes_saccade(1, 0); // right
  delay(refRate);
  eyes_reset(false);


  eyes_saccade(1, 1); // down-right  
  delay(refRate);
  eyes_reset(false);

  eyes_saccade(0, 1); // down
  delay(refRate);
  eyes_reset(false);  

  eyes_saccade(-1, 1); // down-left 
  delay(refRate);
  eyes_reset(false);

  eyes_saccade(-1, 0); // left
  delay(refRate);
  eyes_reset(false);

  eyes_saccade(-1, -1); // up-left
  delay(refRate);
  eyes_reset(false);

  eyes_saccade(0, -1); // up
  delay(refRate);
  eyes_reset(false);

  eyes_saccade(1, -1); // up-right
  delay(refRate);
  eyes_reset(false);

  // eyes_move_big(1); // big right
  // delay(5000);  

  // eyes_move_big(-1); // big left
  // delay(5000);


}