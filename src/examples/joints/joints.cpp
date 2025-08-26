#include <Arduino.h>
#include <chiko_joint.h>

// Declare joint objects for the robot's limbs
Joint LeftLeg, RightLeg, LeftFoot, RightFoot;

void setup() {
    // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Test Chiko");
  Serial.println("Testing Left Leg Joint");
  
  //accelrometer.attachDoubleTapToAction(TOP, topAction);

  initialize_joints(&LeftFoot, &LeftLeg, &RightFoot, &RightLeg);
}

void loop() {
    LeftLeg.setAngle(45, 50);
    delay(1000);
    LeftLeg.setAngle(0, 50);
    delay(1000);
    LeftLeg.setAngle(-45, 50);
    delay(1000);
    // put your main code here, to run repeatedly:
}