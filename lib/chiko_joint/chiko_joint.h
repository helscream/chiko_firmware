#ifndef __CHIKO_JOINT__
#define __CHIKO_JOINT__

#include <Arduino.h>
#include <ESP32Servo.h>
#include <RTOS.h>
#include <Preferences.h>

/*
 Model no: SG90
 Weight: 11g.
 Rotation Angle: 180°
 Torque: 1,8 kg/cm (11 Ncm) (at 4,8 V).
 Torque: 2,4 kg/cm (15 Ncm) (at 6 V).
 Speed: 0,12 sec/60° (at 4,8 V).
 Speed: 0,10 sec/60° (at 6 V).
 Linear response to PWM for easy ramping.
 Operating voltage: 4,8-6 V.
 Operating temperature: -10 to 50 °C.
 Current max: < 600 mA.
 Control system: PWM (Pulse Width Modulation)
 Pulse Frequency / Duty cycle: 50 Hz / 20 ms square wave
*/
/*
    Joint Tasks are only ment for joint setpoints
*/
#define JOINT_TASK_PRIORITY    2 

// Deafult speed of joint movement is set to 720°/second
// Smaller joints are made with MG90 Servos which are quite fast and can acheive these speeds
// while bigger joints are made of MG996r, which are slow. In the case of bigger joints the joint will
// move at its maximum speed which might be lower than 720°/Second
// This parameter only defines the maximum speed of the joint movemnt. There is no way to define the 
// minimum speed.
#define DEFAULT_JOINT_SPEED 300     

// Update rate of joint setpoint
#define JOINT_UPDATE_RATE    20 //[mS] 20 mS is 50 Hz


// Leg Configuration
/*
    Leg Update Rate
    Leg update must be slower (i.e. Integer Multiple of SB_JOINT_UPDATE_RATE) or
    equal to SB_JOINT_UPDATE_RATE
*/
#define LEG_UPDATE_RATE  JOINT_UPDATE_RATE

/*
    Leg maximum speed
*/
#define LEG_MAX_SPEED    4 //[Hz]

/*
    Leg Input joystick dead band
*/
#define LEG_INPUT_DEAD_BAND   (float)0.02 // 0 to 1

/*
    Joint Step angle for
*/
#define STEP_ANGLE          2

// Joint Servo Pins
#define LEFTLEG_PIN     13
#define RIGHTLEG_PIN    4
#define LEFTFOOT_PIN    14
#define RIGHTFOOT_PIN   5

#define SERVO_ENABLE_PIN 15


enum DIRECTION {
    POSITIVE,
    NEGATIVE
};

enum SelectedJoint {
  LEFTFOOT,
  RIGHTFOOT,
  LEFTLEG,
  RIGHTLEG,
  NONE_SELECTED
};


#define LF_OFFSET (float)0
#define LL_OFFSET (float)0
#define RF_OFFSET (float)0
#define RL_OFFSET (float)0



/**
 * @class Joint
 * @brief Represents a servo-controlled joint for the ChikoBot robot.
 *        Provides methods for initialization, movement, speed control, and input attachment.
 */
class Joint{
    private:
        ESP32Servo JointServo;  // Servo object to control the joint
        TaskHandle_t JointSweepTaskHandle = NULL; // RTOS task handle for sweeping motion
    public:
        float JointOffset = 0;   // Measured offset of the joint
        float JointAngle = 0;    // Current angle of the joint
        float JointAngleSetPoint = 0; // Target angle for the joint
        float JointSpeed = DEFAULT_JOINT_SPEED; // Current speed of the joint
        float JointBaseSpeed = DEFAULT_JOINT_SPEED; // Base speed for the joint
        bool enableSweep = false; // Whether sweep mode is enabled
        bool isJointBusy = false; // Whether the joint is currently moving

        /**
         * @brief Write a specific angle to the servo (low-level control).
         * @param angle Angle in degrees to set the servo to.
         */
        void ServoWrite(float angle);

        /**
         * @brief Set the joint to its zero (home) position.
         */
        void setToZero(void);

        /**
         * @brief Initialize the joint with a port, speed, and offset.
         * @param JointPort The pin number for the servo.
         * @param speed The speed to initialize the joint with (default: DEFAULT_JOINT_SPEED).
         * @param offset The offset to apply to the joint (default: 0).
         */
        void init_joint(int JointPort, float speed = DEFAULT_JOINT_SPEED, float offset=0);

        /**
         * @brief Move the joint by a step in the specified direction.
         * @param direction Direction to move (POSITIVE or NEGATIVE).
         */
        void stepAngle (DIRECTION direction);

        /**
         * @brief Get the current angle of the joint.
         * @return The current angle as an integer.
         */
        int getAngle();

        /**
         * @brief Toggle the sweep mode (oscillate between -90 and 90 degrees).
         */
        void toggleJointSweep(void);

        /**
         * @brief Start sweeping the joint between -90 and 90 degrees.
         * @param bindKey If true, bind sweep to a key event (default: false).
         */
        void startJointSweep(bool bindKey=false);

        /**
         * @brief Move the joint to a given angle at a specified speed.
         * @param angle Target angle in degrees.
         * @param percentageSpeed Speed as a percentage of max (default: 20).
         * @param enable If true, enable movement (default: true).
         */
        void setAngle(float angle,int percentageSpeed=20, bool enable=true);

        /**
         * @brief Set the speed of the joint.
         * @param speed Speed value to set.
         */
        void setSpeed(float speed);

        /**
         * @brief Set the base speed of the joint.
         * @param BaseSpeed The base speed value.
         */
        void setBaseSpeed(float BaseSpeed);

        /**
         * @brief Get the base speed of the joint.
         * @return The base speed value.
         */
        float getBaseSpeed(void);

        /**
         * @brief Get the current speed of the joint.
         * @return The current speed value.
         */
        float getSpeed(void);

        /**
         * @brief Attach a joystick input to control the joint.
         * @param JoyStickInput The input value from the joystick.
         */
        void attachInput(float JoyStickInput);

        /**
         * @brief Attach a button input to control the joint.
         * @param ButtonInput The input value from the button.
         */
        void attachInput(bool ButtonInput);
};



/**
 * @brief Initialize all joints by passing pointers to each joint object.
 * @param LeftFootJoint Pointer to the left foot joint.
 * @param LeftLegJoint Pointer to the left leg joint.
 * @param RightFootJoint Pointer to the right foot joint.
 * @param RightLegJoint Pointer to the right leg joint.
 */
void initialize_joints(Joint *LeftFootJoint, Joint *LeftLegJoint, Joint *RightFootJoint, Joint *RightLegJoint);

/**
 * @brief Enable all joints (power on or activate servos).
 */
void enable_joints(void);

/**
 * @brief Disable all joints (power off or deactivate servos).
 */
void disable_joints(void);

/**
 * @brief Run the joint calibration routine for all joints.
 */
void jointCalibrationsRoutine(void);

/**
 * @brief Get the status of a specific joint.
 * @param joint The joint to check.
 * @return True if the joint is enabled/active, false otherwise.
 */
bool getJointStatus(Joint joint);

/**
 * @brief Check if all joints are enabled/active.
 * @return True if all joints are enabled, false otherwise.
 */
bool allJointsStatus(void);

/**
 * @brief Check if the left-side joints are enabled/active.
 * @return True if left joints are enabled, false otherwise.
 */
bool leftJointStatus(void);

/**
 * @brief Check if the right-side joints are enabled/active.
 * @return True if right joints are enabled, false otherwise.
 */
bool rightJointStatus(void);

/**
 * @brief Set the speed for all joints.
 * @param speed The speed value to set.
 */
void setAllJointsSpeed(float speed); 

/**
 * @brief Set the base speed for all joints.
 * @param BaseSpeed The base speed value to set.
 */
void setAllJointsBaseSpeed(float BaseSpeed); 

/**
 * @brief Block execution until all joints are available (not busy).
 */
void waitTillAllJointsAvailable(void);


#endif
