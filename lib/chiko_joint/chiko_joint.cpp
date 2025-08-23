#include "chiko_joint.h"
#include "config.h"
#include "chiko_keyboard.h"


SelectedJoint SJ = NONE_SELECTED;
Joint *LFJ, *LLJ, *RFJ, *RLJ;
Preferences jointOffsets;


void loadJointsOffsets(void) {
  jointOffsets.begin("offsets", true);  //opening the Joint offset seetings as readonly mode
  LFJ->JointOffset = jointOffsets.getFloat("LF_OFFSET", 0);
  LLJ->JointOffset = jointOffsets.getFloat("LL_OFFSET", 0);
  RFJ->JointOffset = jointOffsets.getFloat("RF_OFFSET", 0);
  RLJ->JointOffset = jointOffsets.getFloat("RL_OFFSET", 0);
  jointOffsets.end();
}

void saveJointOffsets(void) {
  jointOffsets.begin("offsets", false);  //opening the Joint offset seetings as read write mode
  jointOffsets.putFloat("LF_OFFSET", LFJ->JointOffset);
  jointOffsets.putFloat("LL_OFFSET", LLJ->JointOffset);
  jointOffsets.putFloat("RF_OFFSET", RFJ->JointOffset);
  jointOffsets.putFloat("RL_OFFSET", RLJ->JointOffset);
  jointOffsets.end();
}

void jointCalibrationsRoutine(void) {
  Serial.println("Joint Offset calibration!");
  Serial.println("1. Left Foot");
  Serial.println("2. Left Leg");
  Serial.println("3. Right Foot");
  Serial.println("4. Right Leg");
  Serial.print("Select joint (1-4) to calibrate or press BACKSPACE to save and exit: ");
  bool bContinuebLoop = 1;
  while (bContinuebLoop) {
    Joint *ActiveJoint;
    switch (SJ) {
      case LEFTFOOT:
        ActiveJoint = LFJ;
        break;

      case RIGHTFOOT:
        ActiveJoint = RFJ;
        break;

      case LEFTLEG:
        ActiveJoint = LLJ;
        break;

      case RIGHTLEG:
        ActiveJoint = RLJ;
        break;

      default:
        break;
    }

    if (SJ == NONE_SELECTED) {
      switch (getKeyPressed()) {
        case ONE:
          Serial.println("Left Foot selected!");
          Serial.println("BACKSPACE: Return to joint selection.");
          Serial.println("+ to increase the offset & - to decrese the offset.");
          SJ = LEFTFOOT;
          break;
        case TWO:
          Serial.println("Left Leg selected!");
          Serial.println("BACKSPACE: Return to joint selection.");
          Serial.println("+ to increase the offset & - to decrese the offset.");
          SJ = LEFTLEG;
          break;
        case THREE:
          Serial.println("Right Foot selected!");
          Serial.println("BACKSPACE: Return to joint selection.");
          Serial.println("+ to increase the offset & - to decrese the offset.");
          SJ = RIGHTFOOT;
          break;
        case FOUR:
          Serial.println("Right Leg selected!");
          Serial.println("BACKSPACE: Return to joint selection.");
          Serial.println("+ to increase the offset & - to decrese the offset.");
          SJ = RIGHTLEG;
          break;
        case BACKSPACE:
          Serial.println("Exitting joint Offset calibration!");
          saveJointOffsets();
          bContinuebLoop = 0;
          break;
        default:
          SJ = NONE_SELECTED;
          break;
      }
    } else {
      switch (getKeyPressed()) {
        case BACKSPACE:
          Serial.println("Joint Offset calibration!");
          Serial.println("1. Left Foot");
          Serial.println("2. Left Leg");
          Serial.println("3. Right Foot");
          Serial.println("4. Right Leg");
          Serial.print("Select joint (1-4) to calibrate or press BACKSPACE to save and exit: ");
          SJ = NONE_SELECTED;
          break;
        case PLUS:
          //ActiveJoint->setAngle(ActiveJoint->getAngle() + 1);
          ActiveJoint->JointOffset = ActiveJoint->JointOffset + 1;
          Serial.print("Offset: ");
          // Serial.print(ActiveJoint->getAngle());
          Serial.print(ActiveJoint->JointOffset);
          Serial.println(" Degree");
          break;

        case MINUS:
          //ActiveJoint->setAngle(ActiveJoint->getAngle() - 1);
          ActiveJoint->JointOffset = ActiveJoint->JointOffset - 1;
          Serial.print("Offset: ");
          //Serial.print(ActiveJoint->getAngle());
          Serial.print(ActiveJoint->JointOffset);
          Serial.println(" Degree");
          break;

        default:
          break;
      }
    }
  }
}

void initialize_joints(Joint *LeftFootJoint, Joint *LeftLegJoint, Joint *RightFootJoint, Joint *RightLegJoint) {

  Serial.begin(115200);
  pinMode(SERVO_ENABLE_PIN, OUTPUT);
  // Initilizing Joints

  LLJ = LeftLegJoint;
  LFJ = LeftFootJoint;
  RLJ = RightLegJoint;
  RFJ = RightFootJoint;

  LFJ->init_joint(LEFTFOOT_PIN, 100, LF_OFFSET);
  LLJ->init_joint(LEFTLEG_PIN, 100, LL_OFFSET);
  RFJ->init_joint(RIGHTFOOT_PIN, 100, RF_OFFSET);
  RLJ->init_joint(RIGHTLEG_PIN, 100, RL_OFFSET);
  loadJointsOffsets();
  enable_joints();
  // Setting the joints to zero
  RLJ->setToZero();
  RFJ->setToZero();
  LLJ->setToZero();
  LFJ->setToZero();

}

void waitTillAllJointsAvailable(void){
  while (allJointsStatus()) {
    delay(JOINT_UPDATE_RATE);
  }
}

bool getJointStatus(Joint *joint) {
  return joint->isJointBusy;
}

bool allJointsStatus(void) {
  return (getJointStatus(LLJ) | getJointStatus(LFJ) | getJointStatus(RLJ) | getJointStatus(RFJ));
}

bool leftJointsStatus(void) {
  return (getJointStatus(LLJ) | getJointStatus(LFJ));
}

bool rightJointsStatus(void) {
  return (getJointStatus(RLJ) | getJointStatus(RFJ));
}

void enable_joints(void) {
  digitalWrite(SERVO_ENABLE_PIN, HIGH);
}

void disable_joints(void) {
  digitalWrite(SERVO_ENABLE_PIN, LOW);
}


void setAllJointsSpeed(float speed){
  LLJ->setSpeed(speed);
  LFJ->setSpeed(speed);
  RLJ->setSpeed(speed);
  RFJ->setSpeed(speed);
}

void setAllJointsBaseSpeed(float BaseSpeed){
  LLJ->setBaseSpeed(BaseSpeed);
  LFJ->setBaseSpeed(BaseSpeed);
  RLJ->setBaseSpeed(BaseSpeed);
  RFJ->setBaseSpeed(BaseSpeed);
}



void static processJointMovement(void *param) {
  while (1) {
    Joint *obj;
    obj = (Joint *)param;
    float JA = obj->JointAngle;
    float JASP = obj->JointAngleSetPoint;
    float JASPEED = obj->JointSpeed;

    if (JA != JASP) {
      obj->isJointBusy = true;
    } else {
      obj->isJointBusy = false;
    }

    if (JASP > JA) {
      JA = JA + JASPEED * JOINT_UPDATE_RATE / (float)1000;
      if (JA > JASP) {
        JA = JASP;
      }
    } else {
      JA = JA - JASPEED * JOINT_UPDATE_RATE / (float)1000;
      if (JA < JASP) {
        JA = JASP;
      }
    }
    // Serial.printf("SP: %.2f,\t CA: %.2f,\t S: %.2f\n",JASP,JA,JASPEED);
    obj->ServoWrite(JA);
    delay(JOINT_UPDATE_RATE);
  }
}


void JointSweepTask(void *param) {
  Joint *obj;
  obj = (Joint *)param;
  while (1) {
    obj->setAngle(-90, obj->JointSpeed);
    delay(15 + round(1000 * 180 / (obj->JointSpeed)));  // waits 15ms for the servo to reach the position
    obj->setAngle(90, obj->JointSpeed);
    delay(15 + round(1000 * 180 / (obj->JointSpeed)));  // waits 15ms for the servo to reach the position
  }
}


void Joint::ServoWrite(float angle) {
  JointAngle = angle;
  JointServo.write(JointAngle + 90 + JointOffset);  // Servo Mid point
}
/*
    Set the Joint to zero position
    */
void Joint::setToZero(void) {
  JointAngle = 0;
  ServoWrite(JointAngle);
}

/*
    Initilize the Joint
    */
void Joint::init_joint(int JointPort, float speed, float offset) {
  JointServo.setPeriodHertz(50);            // standard 50 hz servo
  JointServo.attach(JointPort, 250, 2500);  // attaches the servo on pin 18 to the servo object
  ServoWrite(JointAngle);
  JointSpeed = speed;
  JointOffset = offset;
  xTaskCreate(processJointMovement, "Joint Movement Task", 5000, this, JOINT_TASK_PRIORITY, NULL);
}

/*
        Make a fastest moment of the joint by making a step change in the joint angle
    */
void Joint::stepAngle(DIRECTION direction) {
  if (direction == POSITIVE) {  // Positive Direction
    if (JointAngle + STEP_ANGLE > 90) {
      JointAngle = 90;
    } else {
      JointAngle = JointAngle + STEP_ANGLE;
    }
  } else {  // Negative Direction
    if (JointAngle - STEP_ANGLE < -90) {
      JointAngle = -90;
    } else {
      JointAngle = JointAngle - STEP_ANGLE;
    }
  }
  ServoWrite(JointAngle);
}

/*
        Read the current joint angle
    */
int Joint::getAngle() {
  return JointAngle;
}

/*
      Toggle ON/OFF the joint sweep from 90 <-> -90 degrees
      */
void Joint::toggleJointSweep(void) {
  enableSweep = !enableSweep;
  if (enableSweep) {
    // Serial.println("Starting Task");
    xTaskCreate(JointSweepTask, "Joint Sweep", 4096, this, JOINT_TASK_PRIORITY, &JointSweepTaskHandle);
  } else {
    vTaskDelete(JointSweepTaskHandle);
  }
}

/*
      Start joint sweep 90 <-> -90 degrees
      */
void Joint::startJointSweep(bool bindKey) {
  if (bindKey != enableSweep) {
    toggleJointSweep();
  }
}

/*
    Move joint to the given angle at a given speed
    */
void Joint::setAngle(float angle, int percentageSpeed, bool enable) {
  if (enable) {
    JointAngleSetPoint = angle;
    JointSpeed = ((float)percentageSpeed) * getBaseSpeed() / 100;
    isJointBusy = true ;
  }
}


/*
    Set joint speed
    */
void Joint::setSpeed(float speed) {
  JointSpeed = speed;
}

float Joint::getSpeed(void){
  return JointSpeed;
}

void Joint::setBaseSpeed(float BaseSpeed){
  JointBaseSpeed = BaseSpeed;
}

float Joint::getBaseSpeed(void){
  return JointBaseSpeed;
}

void Joint::attachInput(float JoyStickInput) {
  setAngle(JoyStickInput * 90, JointSpeed, true);
}

void Joint::attachInput(bool ButtonInput) {
  setAngle(ButtonInput * 90, JointSpeed, true);
}
