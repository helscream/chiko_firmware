
/**
 * @file chiko_BMA250.cpp
 * @brief Implementation of BMA250 accelerometer interface for ChikoBot.
 *
 * This file provides functions to initialize, configure, and read data from the BMA250 accelerometer sensor.
 * It also handles double-tap interrupts and allows attaching custom actions to tap events on different faces.
 */

#include <chiko_BMA250.h>

// I2C address and register definitions for BMA250
#define BMA250_I2C_ADDR           0x18
// Register addresses
#define BMA250_REG_CHIP_ID        0x00
#define BMA250_REG_ACC_X_LSB      0x02
#define BMA250_REG_ACC_X_MSB      0x03
#define BMA250_REG_ACC_Y_LSB      0x04
#define BMA250_REG_ACC_Y_MSB      0x05
#define BMA250_REG_ACC_Z_LSB      0x06
#define BMA250_REG_ACC_Z_MSB      0x07
#define BMA250_REG_INT_STATUS_0   0x09
#define BMA250_REG_INT_STATUS_2   0x0B
#define BMA250_REG_RANGE          0x0F
#define BMA250_REG_BW             0x10
#define BMA250_REG_POWER_MODE     0x11
#define BMA250_REG_BGW_SOFTRESET  0x14
#define BMA250_REG_INT_EN_0       0x16
#define BMA250_REG_INT_MAP_0      0x19
#define BMA250_REG_INT_MAP_1      0x1A
#define BMA250_REG_INT_SRC        0x1E
#define BMA250_REG_INT_OUT_CTRL   0x20
#define BMA250_REG_INT_RST_LATCH  0x21
#define BMA250_REG_INT_0          0x22
#define BMA250_REG_INT_1          0x23
#define BMA250_REG_INT_2          0x24
#define BMA250_REG_INT_3          0x25
#define BMA250_REG_INT_4          0x26
#define BMA250_REG_INT_5          0x27
#define BMA250_REG_INT_6          0x28        // Slope or Any motion detection
#define BMA250_REG_INT_7          0x29
#define BMA250_REG_INT_8          0x2A       // Tap duration configuration
#define BMA250_REG_INT_9          0x2B       // Tap threshold configuration
#define BMA250_REG_ORIENT_CONF    0x2F


// Global state for tap detection
TapFace LastTap = NONE; ///< Last detected tap face
bool FLAG_DoubleTap = false; ///< Flag set by interrupt when double tap is detected


/**
 * @brief Interrupt service routine for double tap event.
 * Sets the double tap flag to true.
 */
void BMA250_INIT_EVENT(void){
  FLAG_DoubleTap = true;
}

/**
 * @brief FreeRTOS task to handle double tap events and call user actions.
 * @param param Pointer to BMA250 object.
 */
void DoubleTapTask(void* param){
  BMA250 *obj = (BMA250*) param;
  Serial.println("BMA250 DoupleTap Task Started! ");
  uint8_t intrupt_direction = 0;
  while(1){
    if (FLAG_DoubleTap){
      FLAG_DoubleTap = false;
      // Determine which face was tapped based on interrupt direction
      switch (intrupt_direction) {
        case 0b11000000:
          LastTap = TOP;
          if (obj->TopAction != NULL){
            obj->TopAction();
          }
          Serial.println("DoubleTap: Top");
          break;
        
        case 0b10100000:
          LastTap = FRONT;
          if (obj->FrontAction != NULL){
            obj->FrontAction();
          }
          Serial.println("DoubleTap: Front");
          break;
        
        case 0b00010000:
          LastTap = LEFT;
          if (obj->LeftAction != NULL){
            obj->LeftAction();
          }
          Serial.println("DoubleTap: Left");
          break;
        
        case 0b10010000:
          LastTap = RIGHT;
          if (obj->RightAction != NULL){
            obj->RightAction();
          }
          Serial.println("DoubleTap: Right");
          break;
        
        case 0b01000000:
          LastTap = BOTTOM;
          if (obj->BottomAction != NULL){
            obj->BottomAction();
          }
          Serial.println("DoubleTap: Bottom");
          break;

        case 0b00100000:
          LastTap = BACK;
          if (obj->BackAction != NULL){
            obj->BackAction();
          }
          Serial.println("DoubleTap: Back");
          break;

        default:
          LastTap = NONE;
          Serial.print("DoubleTap: ??, INT_STATUS_2 = ");
          Serial.println(intrupt_direction);
          break;
      }
    }
    intrupt_direction = obj->readRegister(BMA250_REG_INT_STATUS_2);
    delay(50);
  }
}

/**
 * @brief Set the measurement range of the BMA250 sensor.
 * @param range The desired range (e.g., ±2g, ±4g, etc.)
 */
void BMA250::setRange(BMA250Range range){
  writeRegister(BMA250_REG_RANGE,(uint8_t)range);
}

/**
 * @brief Read a single byte from a BMA250 register.
 * @param RegAddr Register address to read from.
 * @return Value read from the register, or 0xFF if failed.
 */
uint8_t BMA250::readRegister(uint8_t RegAddr){
  Wire.beginTransmission(BMA250_I2C_ADDR);
  Wire.write(RegAddr);
  Wire.endTransmission(false);
  Wire.requestFrom(BMA250_I2C_ADDR, 1);
  if (Wire.available()) {
    return Wire.read();
  }
  return 0xFF;  // Return invalid value if read fails
}

/**
 * @brief Write a single byte to a BMA250 register.
 * @param RegAddr Register address to write to.
 * @param value Value to write.
 */
void BMA250::writeRegister(uint8_t RegAddr, uint8_t value){
  Wire.beginTransmission(BMA250_I2C_ADDR);
  Wire.write(RegAddr);
  Wire.write(value);
  Wire.endTransmission();
}

/**
 * @brief Set the bandwidth (output data rate) of the BMA250 sensor.
 * @param bw Desired bandwidth setting.
 */
void BMA250::setBandwidth(BMA250Bandwidth bw){
  // Register 0X10 (PMU_BW)
  // BITs   7:5 reserved, 4:0 bw<4:0>
  writeRegister(BMA250_REG_BW,(uint8_t)bw);
}

/**
 * @brief Read a 10-bit signed value from the specified axis registers.
 * @param msbReg Register address for MSB.
 * @param lsbReg Register address for LSB.
 * @return Signed 10-bit raw value.
 */
int16_t BMA250::readAxis(uint8_t msbReg, uint8_t lsbReg) {
  uint8_t lsb = readRegister(lsbReg); // The LSB must always be read first to keep the integrity of data
  uint8_t msb = readRegister(msbReg);
  // Combine MSB and LSB (10-bit value)
  int16_t raw = ((int16_t)msb << 2) | (lsb >> 6);
  // Convert to signed 10-bit
  if (raw > 511) raw -= 1024;
  return raw;
}

/**
 * @brief Read acceleration in g for the X axis.
 * @return Acceleration in g.
 */
float BMA250::readXaxis(void){
  int16_t x = readAxis(0x03, 0x02); // X axis
  // Convert raw to g-force (assuming ±2g range, 256 LSB/g)
  float g = x / 256.0;
  return g;
}

/**
 * @brief Read acceleration in g for the Y axis.
 * @return Acceleration in g.
 */
float BMA250::readYaxis(void){
  int16_t y = readAxis(0x05, 0x04); // Y axis
  // Convert raw to g-force (assuming ±2g range, 256 LSB/g)
  float g = y / 256.0;
  return g;
}

/**
 * @brief Read acceleration in g for the Z axis.
 * @return Acceleration in g.
 */
float BMA250::readZaxis(void){
  int16_t z = readAxis(0x07, 0x06); // Z axis
  // Convert raw to g-force (assuming ±2g range, 256 LSB/g)
  float g = z / 256.0;
  return g;
}

/**
 * @brief Attach a user-defined action to a double tap event on a specific face.
 * @param face The face to attach the action to (TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK).
 * @param action Function pointer to the action to execute.
 */
void BMA250::attachDoubleTapToAction(TapFace face, void (*action)()){
  switch (face) {
  case TOP:
    TopAction = action;
    break;
  case BOTTOM:
    BottomAction = action;
    break;
  case LEFT:
    LeftAction = action;
    break;
  case RIGHT:
    RightAction = action;
    break;
  case FRONT:
    FrontAction = action;
    break;
  case BACK:
    BackAction = action;
    break;
  default:
    break;
  }
}
/**
 * @brief Initialize the BMA250 sensor, configure interrupts, and start the double tap task.
 */
void BMA250::initialize(void) { 
  pinMode(BMA250_INT_PIN, INPUT); // BMA250 is configured at active high
  attachInterrupt(BMA250_INT_PIN, BMA250_INIT_EVENT, FALLING);

  // Initialize sensor
  Wire.begin(); // SDA = GPIO21, SCL = GPIO22 by default on ESP32
  
  writeRegister(BMA250_REG_BGW_SOFTRESET, 0xB6); // Soft reset
  // Reading Chip ID:
  Serial.print("Chip ID: ");
  Serial.println(readRegister(BMA250_REG_CHIP_ID));

  // Set sensor to normal mode
  writeRegister(BMA250_REG_POWER_MODE, 0x00);
  Serial.print("Power Mode: ");
  Serial.println(readRegister(BMA250_REG_POWER_MODE));

  // Set Range of the sensor
  setRange(RANGE_2G);

  // Set Bandwidth
  setBandwidth(BW_125HZ);

  // Set INT latch reset timer
  writeRegister(BMA250_REG_INT_RST_LATCH,INT_MODE_1ms);

  // Set interrupt threshold for any motion
  writeRegister(BMA250_REG_INT_6,0x04);

  // Set tap configuration
  writeRegister(BMA250_REG_INT_8,B10000110);

  // Set tap thresholds
  writeRegister(BMA250_REG_INT_9,B10001000);
  
  // Configure INT1 output pin
  writeRegister(BMA250_REG_INT_OUT_CTRL,B00000001);

  // Configure source of data to double tap interrupt engine
  writeRegister(BMA250_REG_INT_SRC,B00000000); // INT mapped for double tap

  // Enable double tap interrupt
  writeRegister(BMA250_REG_INT_EN_0,B00010000); // INT mapped for double tap

  // Map the interrupt to INT1
  writeRegister(BMA250_REG_INT_MAP_0, B00010000); // Mapped for double tap
  
  // Start the double tap task
  xTaskCreate(DoubleTapTask,"Double Tap Task",2048,this,1,NULL);
}
  


/**
 * @brief Get the angle in degrees between the X and Y axes.
 * @return Angle in degrees.
 */
float BMA250::getAngleXY(void){
  float x = readXaxis();
  float y = readYaxis();
  return atan2(y, x) * 180.0 / PI;
}
/**
 * @brief Get the angle in degrees between the X and Z axes.
 * @return Angle in degrees.
 */
float BMA250::getAngleXZ(void){
  float x = readXaxis();
  float z = readZaxis();
  return atan2(z, x) * 180.0 / PI;
}
/**
 * @brief Get the angle in degrees between the Y and Z axes.
 * @return Angle in degrees.
 */
float BMA250::getAngleYZ(void){
  float y = readYaxis();
  float z = readZaxis();
  return atan2(z, y) * 180.0 / PI;
}


float BMA250::getPitchAngle(void){
  return getAngleYZ() - 90;
}

float BMA250::getRollAngle(void){
  return getAngleXZ() - 90;
  }

float BMA250::getTemperature(void){
  // Temperature register is at 0x08, value in degrees Celsius = (value * 0.5) + 23
  uint8_t temp_raw = readRegister(0x08);
  return (temp_raw * 0.5) + 23;
}

/**
 * @brief Get the last detected tap face.
 * @return The last TapFace value (TOP, BOTTOM, LEFT, RIGHT, FRONT, BACK, NONE).
 */
TapFace BMA250::getLastTapFace(void){
  return LastTap;
}