

/**
 * @file walk.cpp
 * @brief Main firmware entry point for ChikoBot walking example.
 *
 * This file implements a modular, event-driven walking routine for ChikoBot, 
 * a bipedal robot platform. The design leverages hardware abstraction for 
 * joints and sensors, and an action-based state machine for movement routines. 
 * The code is structured for clarity, maintainability, and extensibility, making 
 * it easy to add new behaviors or modify existing ones.
 *
 * Comprehensive Reasoning & Design Rationale:
 * - **Hardware Abstraction:** The use of `Joint` and `BMA250` classes abstracts 
 * away hardware details, allowing the walking logic to focus on high-level 
 * movement rather than low-level control.
 * - **Event-Driven Actions:** Walking is triggered and stopped by double-tap gestures,
 * making the robot interactive and responsive to user input. This decouples the 
 * walking logic from the main loop, enabling future expansion (e.g., more gestures, 
 * remote control).
 * - **Action State Machine:** The `action` object encapsulates the walking routine 
 * as a state machine with enter, loop, and exit routines. This modular approach 
 * allows for easy sequencing and reuse of behaviors.
 * - **Safety and Debugging:** The robot always returns to a neutral pose 
 * when starting or stopping walking, reducing the risk of falls or hardware stress. 
 * Serial output provides real-time feedback for debugging and monitoring.
 *
 * High-Level Flow:
 * 1. **Initialization:** Joints and accelerometer are initialized. Joint offsets are printed for calibration.
 * 2. **Event Binding:** Double-tap gestures (LEFT/RIGHT) are bound to start/stop walking actions.
 * 3. **Walking Action:** When triggered, the walking action executes a sequence of joint movements to simulate a walking gait, repeating for a set number of iterations.
 * 4. **Exit Routine:** On stop, the robot returns to a safe, neutral pose.
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


// Declare joint objects for the robot's limbs.
// Each Joint object represents a servo or actuator controlling a limb segment.
Joint LeftLeg, RightLeg, LeftFoot, RightFoot;

// Accelerometer object for gesture detection (double-tap events)
BMA250 accelrometer;

// Action object for walking routine.
// Encapsulates the walking state machine (enter, loop, exit routines)
action chikoWalkAction;

// Forward declarations for walking action routines
void walkEnterRoutine();   // Called once when walking starts
void walkLoopRoutine();    // Called repeatedly while walking
void walkExitRoutine();    // Called once when walking stops


/**
 * @brief Lambda function to start the walking action.
 * Called when a double-tap gesture is detected on the LEFT side.
 *
 * Reasoning: Using a lambda (function pointer) allows flexible event binding. The number of loop iterations (5) can be tuned for desired walking distance.
 */
void WalkStartLambdaFunction(void) {
  chikoWalkAction.begin(5); // Start walking action with 5 loop iterations
}

/**
 * @brief Lambda function to stop the walking action.
 * Called when a double-tap gesture is detected on the RIGHT side.
 *
 * Reasoning: Decouples the stop event from the main loop, allowing immediate and safe interruption of walking.
 */
void WalkEndLambdaFunction(void) {
  chikoWalkAction.stop(); // Stop the walking action
}


/**
 * @brief Arduino setup function: runs once at startup.
 * Initializes serial, sensors, joints, and walking action.
 *
 * Reasoning: All hardware and event bindings are performed here to ensure the robot is ready for user interaction immediately after boot.
 * The joint offsets are printed for calibration and troubleshooting.
 */
void setup() {
  Serial.begin(115200); // Start serial communication for debugging
  Serial.println("Chiko");

  // Initialize accelerometer (BMA250) for gesture detection
  accelrometer.initialize();
  // Optionally attach other gestures to actions here
  // accelrometer.attachDoubleTapToAction(TOP, topAction);

  // Initialize all robot joints (calibrates and sets up servos)
  initialize_joints(&LeftFoot, &LeftLeg, &RightFoot, &RightLeg);

  // Print joint offsets for debugging and calibration
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

  // Create the walking action and bind routines (state machine)
  chikoWalkAction.create(walkEnterRoutine, walkLoopRoutine, walkExitRoutine);

  // Attach double-tap gestures to start/stop walking
  // LEFT: Start walking, RIGHT: Stop walking
  accelrometer.attachDoubleTapToAction(LEFT, WalkStartLambdaFunction);
  accelrometer.attachDoubleTapToAction(RIGHT, WalkEndLambdaFunction);
}



/**
 * @brief Arduino loop function: runs repeatedly after setup.
 * Main control loop for ChikoBot. Add additional logic here if needed.
 *
 * Reasoning: The walking action is event-driven and managed by the action object, so the main loop remains empty unless additional features are added.
 */
void loop() {
  // Main control loop for ChikoBot
  // The walking action is event-driven; no code needed here unless adding more features.
}



/**
 * @brief Walking action entry routine.
 * Sets the robot to a neutral standing pose, then transitions to the initial walking pose.
 * Called once when the walking action begins.
 *
 * Reasoning: Ensures the robot always starts walking from a safe, known configuration. This reduces mechanical stress and improves repeatability.
 *
 * Step-by-step:
 * 1. Set all joints to zero (neutral pose).
 * 2. Move to the first step pose (prepares for walking).
 * 3. Adjust legs for initial gait.
 * 4. Return to neutral before entering the main loop.
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
 *
 * Reasoning: The gait is broken into discrete steps for clarity and maintainability. Each step is separated by a wait to ensure all joints reach their target before proceeding, preventing mechanical conflicts.
 *
 * Step-by-step:
 * 1. Move feet backward (simulate step back).
 * 2. Shift weight and move legs (simulate weight transfer).
 * 3. Return to neutral (balance).
 * 4. Move feet forward (simulate step forward).
 * 5. Shift weight and move legs (simulate weight transfer).
 * 6. Return to neutral (balance).
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
 *
 * Reasoning: Ensures the robot always stops in a safe, balanced configuration, reducing risk of falls or hardware damage.
 *
 * Step-by-step:
 * 1. Move feet backward to stop motion.
 * 2. Return legs to neutral (balance).
 * 3. Set all joints to zero (neutral pose).
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
