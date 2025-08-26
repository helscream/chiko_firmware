#include <Arduino.h>
#include <chiko_BMA250.h>

BMA250 accelrometer;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Chiko BMA250 Test");
  accelrometer.initialize();
}   

void loop() {
  // put your main code here, to run repeatedly:
  float x = accelrometer.readXaxis();
  float y = accelrometer.readYaxis();
  float z = accelrometer.readZaxis();

  Serial.print("Gs: X: ");
  Serial.print(x);
  Serial.print(" g, Y: ");
  Serial.print(y);
  Serial.print(" g, Z: ");
  Serial.print(z);
  Serial.print(" g");
    Serial.print("\tAngles: Pitch: ");
    Serial.print(accelrometer.getPitchAngle());
    Serial.print(" deg, Roll: ");
    Serial.print(accelrometer.getRollAngle());    
    Serial.print(" deg");
    Serial.print("\tTemp: ");
    Serial.print(accelrometer.getTemperature());
    Serial.println(" C");

  delay(1000);
}