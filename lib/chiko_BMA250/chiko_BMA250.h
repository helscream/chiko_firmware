#ifndef BMA250_H
#define BMA250_H

#include <Arduino.h>
#include <Wire.h>



#define BMA250_INT_PIN            32

// Enum definitions for BMA250 configuration and events

/**
 * @enum BMA250Range
 * @brief Measurement range options for the BMA250 accelerometer.
 */
enum BMA250Range {
  RANGE_2G = 0x03,  // DEFAULT: ±2g
  RANGE_4G = 0x05,  // ±4g
  RANGE_8G = 0x08,  // ±8g
  RANGE_16G = 0x0C  // ±16g
};

/**
 * @enum BMA250Bandwidth
 * @brief Bandwidth (output data rate) options for the BMA250.
 */
enum BMA250Bandwidth {
  BW_7_81HZ = 0x08,
  BW_15_63HZ = 0x09,
  BW_31_25HZ = 0x0A,
  BW_62_5HZ = 0x0B,
  BW_125HZ = 0x0C,
  BW_250HZ = 0x0D,
  BW_500HZ = 0x0E,
  BW_1000HZ = 0x0F
};

/**
 * @enum BMA250InterruptType
 * @brief Interrupt types supported by the BMA250.
 */
enum BMA250InterruptType {
  INT_SLOPE_X = 0x01,      // Slope interrupt on X axis
  INT_SLOPE_Y = 0x02,      // Slope interrupt on Y axis
  INT_SLOPE_Z = 0x04,      // Slope interrupt on Z axis
  INT_DOUBLE_TAP = 0x10,   // Double tap interrupt
  INT_ORIENT = 0x40        // Orientation change interrupt
};

/**
 * @enum BMA250InterruptPin
 * @brief Interrupt pin selection for BMA250.
 */
enum BMA250InterruptPin {
  INT1 = 0x01, // Interrupt pin 1
  INT2 = 0x02  // Interrupt pin 2
};

/**
 * @enum BMA250RstLatch
 * @brief Interrupt reset/latch modes for BMA250.
 */
enum BMA250RstLatch {
  INT_MODE_8s  =  0x06,   // 8 seconds
  INT_MODE_4s  = 0x05,    // 4 seconds
  INT_MODE_1s  = 0x03,    // 1 second
  INT_MODE_50ms= 0x0E,    // 50 milliseconds
  INT_MODE_1ms= 0x0B,     // 1 millisecond
  INT_MODE_LATCHED = 0x0F // Latched mode
};

/**
 * @enum TapFace
 * @brief Faces of the sensor for tap detection.
 */
enum TapFace {
  TOP,
  BOTTOM,
  LEFT,
  RIGHT,
  BACK,
  FRONT,
  NONE
};



/**
 * @class BMA250
 * @brief Driver class for the BMA250 accelerometer sensor.
 *        Provides initialization, configuration, and data reading methods.
 */
class BMA250{
  private:
    /**
     * @brief Read a signed 16-bit value from two registers (MSB and LSB) for an axis.
     * @param msbReg Register address for the most significant byte.
     * @param lsbReg Register address for the least significant byte.
     * @return The combined signed 16-bit axis value.
     */
    int16_t readAxis(uint8_t msbReg, uint8_t lsbReg);
    
  public:
    /**
     * @brief Function pointers for actions to perform on double tap for each face.
     */
    void (*TopAction)()=NULL, (*BottomAction)()=NULL, (*LeftAction)()=NULL,
         (*RightAction)()=NULL, (*FrontAction)()=NULL, (*BackAction)()=NULL;

    /**
     * @brief Initialize the BMA250 sensor (I2C setup, configuration, etc).
     */
    void initialize(void);

    /**
     * @brief Read a register from the BMA250.
     * @param RegAddr Register address to read from.
     * @return Value read from the register.
     */
    uint8_t readRegister(uint8_t RegAddr);

    /**
     * @brief Write a value to a register on the BMA250.
     * @param RegAddr Register address to write to.
     * @param value Value to write.
     */
    void writeRegister(uint8_t RegAddr, uint8_t value);

    /**
     * @brief Read the X-axis acceleration value.
     * @return Acceleration in g or raw units (implementation dependent).
     */
    float readXaxis(void);

    /**
     * @brief Read the Y-axis acceleration value.
     * @return Acceleration in g or raw units (implementation dependent).
     */
    float readYaxis(void);

    /**
     * @brief Read the Z-axis acceleration value.
     * @return Acceleration in g or raw units (implementation dependent).
     */
    float readZaxis(void);

    /**
     * @brief Set the measurement range of the sensor.
     * @param range Range value (see BMA250Range enum).
     */
    void setRange(BMA250Range range);

    /**
     * @brief Set the bandwidth (output data rate) of the sensor.
     * @param bw Bandwidth value (see BMA250Bandwidth enum).
     */
    void setBandwidth(BMA250Bandwidth bw);

    /**
     * @brief Attach an action (function) to a double tap event on a specific face.
     * @param face The face to attach the action to (see TapFace enum).
     * @param action Function pointer to call on double tap.
     */
    void attachDoubleTapToAction(TapFace face, void (*action)());

    /**
     * @brief Get the face where the last double tap was detected.
     * @return The face (see TapFace enum).
     */
    TapFace getLastTapFace(void);

  /**
   * @brief Get the angle in degrees between the X and Y axes.
   * @return Angle in degrees.
   */
  float getAngleXY(void);

  /**
   * @brief Get the angle in degrees between the X and Z axes.
   * @return Angle in degrees.
   */
  float getAngleXZ(void);

  /**
   * @brief Get the angle in degrees between the Y and Z axes.
   * @return Angle in degrees.
   */
  float getAngleYZ(void);

  /**
   * @brief Get the pitch angle of the sensor (rotation around Y axis).
   * @return Pitch angle in degrees.
   */
  float getPitchAngle(void);

  /**
   * @brief Get the roll angle of the sensor (rotation around X axis).
   * @return Roll angle in degrees.
   */
  float getRollAngle(void);

  /**
   * @brief Get the temperature reading from the sensor (if supported).
   * @return Temperature in degrees Celsius.
   */
  float getTemperature(void);
};



#endif