
// Main firmware entry point for ChikoBot

#include <Arduino.h>           // Core Arduino functionality
#include <chiko_joint.h>       // Custom joint control for ChikoBot
#include <chiko_BMA250.h>      // BMA250 accelerometer support
#include <chiko_action.h>    // Predefined actions for ChikoBot


// Declare joint objects for the robot's limbs
Joint LeftLeg, RightLeg, LeftFoot, RightFoot;
BMA250 accelrometer;

// Action object for walking routine
action chikoWalkAction;
void walkEnterRoutine();
void walkLoopRoutine();
void walkExitRoutine();

/*Walk Action Lambda functions to interface with Events*/
void WalkStartLambdaFunction(void) {
  chikoWalkAction.begin(5);
}
void WalkEndLambdaFunction(void) {
  chikoWalkAction.stop();
}


// Arduino setup function: runs once at startup
void setup() {
   Serial.begin(115200);
  Serial.println("Chiko");
  
  accelrometer.intilize();
  //accelrometer.attachDoubleTapToAction(TOP, topAction);

  initialize_joints(&LeftFoot, &LeftLeg, &RightFoot, &RightLeg);

  Serial.println("Chiko Joints initialized!");
  Serial.println("Joint offsets are:");
  Serial.print("Left Leg\t=\t");
  Serial.print(LeftLeg.JointOffset);
  Serial.print(" Degrees \nLeft Foot \t=\t");
  Serial.print(LeftFoot.JointOffset);
  Serial.print(" Degrees \nRight Leg \t=\t");
  Serial.print(RightLeg.JointOffset);
  Serial.print(" Degrees \nRight Foot \t=\t");
  Serial.print(RightFoot.JointOffset);
  Serial.println(" Degrees");

  chikoWalkAction.create(walkEnterRoutine, walkLoopRoutine, walkExitRoutine);
  
  accelrometer.attachDoubleTapToAction(LEFT, WalkStartLambdaFunction);
  accelrometer.attachDoubleTapToAction(RIGHT, WalkEndLambdaFunction);
}

// Arduino loop function: runs repeatedly after setup
void loop() {
  // Main control loop for ChikoBot
  // put your main code here, to run repeatedly:
}



/*Walk Action Routines*/
void walkEnterRoutine(void) {
  // Setting the joints to zero
  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  RightLeg.setAngle(0, 50);
  LeftLeg.setAngle(0, 50);
  waitTillAllJointsAvailable();

  RightFoot.setAngle(40, 100);
  LeftFoot.setAngle(20, 50);
  waitTillAllJointsAvailable();

  RightFoot.setAngle(20, 100);
  RightLeg.setAngle(15, 50);
  LeftLeg.setAngle(15, 50);
  waitTillAllJointsAvailable();

  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  waitTillAllJointsAvailable();
}

void walkLoopRoutine(void) {
  Serial.print("Walk Loop Iteration #: ");
  Serial.println(chikoWalkAction.LoopItrations);
  RightFoot.setAngle(-20, 50);
  LeftFoot.setAngle(-40, 100);
  waitTillAllJointsAvailable();

  LeftFoot.setAngle(-20, 100);
  RightLeg.setAngle(-15, 50);
  LeftLeg.setAngle(-15, 50);
  waitTillAllJointsAvailable();

  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  waitTillAllJointsAvailable();

  RightFoot.setAngle(40, 100);
  LeftFoot.setAngle(20, 50);
  waitTillAllJointsAvailable();

  RightFoot.setAngle(20, 100);
  RightLeg.setAngle(15, 50);
  LeftLeg.setAngle(15, 50);
  waitTillAllJointsAvailable();

  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  waitTillAllJointsAvailable();

}

void walkExitRoutine(void) {
  RightFoot.setAngle(-20, 50);
  LeftFoot.setAngle(-40, 100);
  waitTillAllJointsAvailable();

  LeftFoot.setAngle(-20, 100);
  RightLeg.setAngle(0, 50);
  LeftLeg.setAngle(0, 50);
  waitTillAllJointsAvailable();

  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  waitTillAllJointsAvailable();
}


