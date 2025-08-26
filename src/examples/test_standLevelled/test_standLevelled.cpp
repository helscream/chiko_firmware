#include <Arduino.h>
#include <chiko_bma250.h>
#include <chiko_joint.h>


BMA250 accelrometer;
// Declare joint objects for the robot's limbs
Joint LeftLeg, RightLeg, LeftFoot, RightFoot;

void setup() {
    // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Test Chiko");
  Serial.println("Testing Stand Levelled");
  accelrometer.initialize();
 initialize_joints(&LeftFoot, &LeftLeg, &RightFoot, &RightLeg);
 LeftFoot.setToZero();
 LeftLeg.setToZero();
    RightFoot.setToZero();
    RightLeg.setToZero();
    delay(2000); // Wait for 2 seconds to ensure everything is initialized
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

    // Simple proportional control to keep the robot level
    // PI controller for pitch angle
    static float roll_integral = 0.0f;
    float roll_setpoint = 0.0f; // Target is level
    float roll_error = roll_setpoint - accelrometer.getRollAngle();
    float Kp = 1.0f; // Proportional gain, tune as needed
    float Ki = 0.05f; // Integral gain, tune as needed

    static float roll_output = 0.0f; // Control output for pitch
    if (roll_output < 50.0f && roll_output > -50.0f){
        roll_integral += roll_error * 0.1f; // 0.1s loop time (100ms delay)
    } 
    
     roll_output = Kp * roll_error + Ki * roll_integral;

    if (accelrometer.getRollAngle() > 0) {
        // Tilted forward
        LeftFoot.setAngle(roll_output, 30);
        RightFoot.setAngle(-accelrometer.getRollAngle(), 30);
    } else if (accelrometer.getRollAngle() < 0) {
        // Tilted backward
        // LeftLeg.setAngle(-accelrometer.getPitchAngle(), 30);
        // RightLeg.setAngle(-accelrometer.getPitchAngle(), 30);
    } else {
        // Level
        LeftFoot.setAngle(0, 30);
        RightFoot.setAngle(0, 30);
    }
    


//    LeftFoot.setAngle(accelrometer.getRollAngle(), 30);
//    RightFoot.setAngle(accelrometer.getRollAngle(), 30);

    delay(100); // Adjust delay as needed for responsiveness
}