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


class Joint{
    private:
        ESP32Servo JointServo;  // create servo object to control a servo
        TaskHandle_t JointSweepTaskHandle = NULL;
    public:
        float JointOffset = 0;   // Measured offset of the joint
        float JointAngle = 0; // Inital Angle
        float JointAngleSetPoint = 0;
        float JointSpeed = DEFAULT_JOINT_SPEED;
        float JointBaseSpeed = DEFAULT_JOINT_SPEED;
        bool enableSweep = false;
        bool isJointBusy = false;
        void ServoWrite(float angle);

        /*
        Set the Joint to zero position
        */
        void setToZero(void);

        /*
        Initilize the Joint
        */
        void init_joint(int JointPort, float speed = DEFAULT_JOINT_SPEED, float offset=0);

        /*
            Make a fastest moment of the joint by making a step change in the joint angle
        */
        void stepAngle (DIRECTION direction);

        /*
            Read the current joint angle
        */
        int getAngle();

        /*
        Toggle ON/OFF the joint sweep from 90 <-> -90 degrees
        */
        void toggleJointSweep(void);

        /*
        Start joint sweep 90 <-> -90 degrees
        */
        void startJointSweep(bool bindKey=false);

        /*
        Move joint to the given angle at a given speed
        */
        void setAngle(float angle,int percentageSpeed=20, bool enable=true);

        /*
        Set joint speed
        */
        void setSpeed(float speed);

        void setBaseSpeed(float BaseSpeed);

        float getBaseSpeed(void);

        float getSpeed(void);

        void attachInput(float JoyStickInput);

        void attachInput(bool ButtonInput);

};


void initialize_joints(Joint *LeftFootJoint, Joint *LeftLegJoint, Joint *RightFootJoint, Joint *RightLegJoint);

void enable_joints(void);

void disable_joints(void);

void jointCalibrationsRoutine(void);

bool getJointStatus(Joint joint);

bool allJointsStatus(void);

bool leftJointStatus(void);

bool rightJointStatus(void);

void setAllJointsSpeed(float speed); 

void setAllJointsBaseSpeed(float BaseSpeed); 

void waitTillAllJointsAvailable(void);


#endif
