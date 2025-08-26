

/**
 * @file walk.cpp
 * @brief Main firmware entry point for ChikoBot walking example.
 *
 * This file demonstrates the walking routine for ChikoBot, a bipedal robot. It initializes the robot's joints and accelerometer,
 * sets up event-driven walking actions triggered by double-tap gestures, and defines the walking sequence using a stateful action object.
 *
 * Key Features:
 * - Uses custom Joint and BMA250 classes for hardware abstraction.
 * - Implements a modular action system for walking, with enter, loop, and exit routines.
 * - Allows starting and stopping the walking action via double-tap gestures on the accelerometer (LEFT/RIGHT).
 * - Provides detailed serial output for debugging and monitoring.
 *
 * Usage:
 * 1. Upload to ChikoBot hardware.
 * 2. Double-tap the robot (LEFT) to start walking, double-tap (RIGHT) to stop.
 * 3. Observe joint angles and action state via serial monitor.
 */

#include <Arduino.h>           // Core Arduino functionality
#include <chiko_joint.h>       // Custom joint control for ChikoBot
#include <chiko_BMA250.h>      // BMA250 accelerometer support
#include <chiko_action.h>      // Predefined actions for ChikoBot

// Declare joint objects for the robot's limbs
Joint LeftLeg, RightLeg, LeftFoot, RightFoot;
BMA250 accelrometer; // Accelerometer object for gesture detection

// Action object for walking routine
action chikoWalkAction;

// Forward declarations for walking action routines
void walkEnterRoutine();
void walkLoopRoutine();
void walkExitRoutine();

/**
 * @brief Lambda function to start the walking action.
 * Called when a double-tap gesture is detected on the LEFT side.
 */
void WalkStartLambdaFunction(void) {
  chikoWalkAction.begin(5); // Start walking action with 5 loop iterations
}

/**
 * @brief Lambda function to stop the walking action.
 * Called when a double-tap gesture is detected on the RIGHT side.
 */
void WalkEndLambdaFunction(void) {
  chikoWalkAction.stop(); // Stop the walking action
}

/**
 * @brief Arduino setup function: runs once at startup.
 * Initializes serial, sensors, joints, and walking action.
 */
void setup() {
  Serial.begin(115200);
  Serial.println("Chiko");

  // Initialize accelerometer (BMA250)
  accelrometer.initialize();
  // Optionally attach other gestures to actions here
  // accelrometer.attachDoubleTapToAction(TOP, topAction);

  // Initialize all robot joints
  initialize_joints(&LeftFoot, &LeftLeg, &RightFoot, &RightLeg);

  // Print joint offsets for debugging
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

  // Create the walking action and bind routines
  chikoWalkAction.create(walkEnterRoutine, walkLoopRoutine, walkExitRoutine);

  // Attach double-tap gestures to start/stop walking
  accelrometer.attachDoubleTapToAction(LEFT, WalkStartLambdaFunction);
  accelrometer.attachDoubleTapToAction(RIGHT, WalkEndLambdaFunction);
}


/**
 * @brief Arduino loop function: runs repeatedly after setup.
 * Main control loop for ChikoBot. Add additional logic here if needed.
 */
void loop() {
  // Main control loop for ChikoBot
  // The walking action is event-driven; no code needed here unless adding more features.
}


/**
 * @brief Walking action entry routine.
 * Sets the robot to a neutral standing pose, then transitions to the initial walking pose.
 * Called once when the walking action begins.
 */
void walkEnterRoutine(void) {
  // Set all joints to zero (neutral pose)
  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  RightLeg.setAngle(0, 50);
  LeftLeg.setAngle(0, 50);
  waitTillAllJointsAvailable();

  // Move to first step pose
  RightFoot.setAngle(40, 100);
  LeftFoot.setAngle(20, 50);
  waitTillAllJointsAvailable();

  // Prepare for walking
  RightFoot.setAngle(20, 100);
  RightLeg.setAngle(15, 50);
  LeftLeg.setAngle(15, 50);
  waitTillAllJointsAvailable();

  // Return to neutral
  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  waitTillAllJointsAvailable();
}

/**
 * @brief Walking action loop routine.
 * Repeats the walking gait sequence for each iteration.
 * Called repeatedly while the walking action is active.
 */
void walkLoopRoutine(void) {
  Serial.print("Walk Loop Iteration #: ");
  Serial.println(chikoWalkAction.LoopItrations);

  // Step 1: Move feet backward
  RightFoot.setAngle(-20, 50);
  LeftFoot.setAngle(-40, 100);
  waitTillAllJointsAvailable();

  // Step 2: Shift weight and move legs
  LeftFoot.setAngle(-20, 100);
  RightLeg.setAngle(-15, 50);
  LeftLeg.setAngle(-15, 50);
  waitTillAllJointsAvailable();

  // Step 3: Return to neutral
  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  waitTillAllJointsAvailable();

  // Step 4: Move feet forward
  RightFoot.setAngle(40, 100);
  LeftFoot.setAngle(20, 50);
  waitTillAllJointsAvailable();

  // Step 5: Shift weight and move legs
  RightFoot.setAngle(20, 100);
  RightLeg.setAngle(15, 50);
  LeftLeg.setAngle(15, 50);
  waitTillAllJointsAvailable();

  // Step 6: Return to neutral
  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  waitTillAllJointsAvailable();
}

/**
 * @brief Walking action exit routine.
 * Returns the robot to a safe, neutral pose when walking ends.
 * Called once when the walking action is stopped.
 */
void walkExitRoutine(void) {
  // Move feet backward to stop
  RightFoot.setAngle(-20, 50);
  LeftFoot.setAngle(-40, 100);
  waitTillAllJointsAvailable();

  // Return legs to neutral
  LeftFoot.setAngle(-20, 100);
  RightLeg.setAngle(0, 50);
  LeftLeg.setAngle(0, 50);
  waitTillAllJointsAvailable();

  // Set all joints to zero (neutral pose)
  RightFoot.setAngle(0, 50);
  LeftFoot.setAngle(0, 50);
  waitTillAllJointsAvailable();
}
