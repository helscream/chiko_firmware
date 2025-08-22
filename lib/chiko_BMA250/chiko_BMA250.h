#ifndef BMA250_H
#define BMA250_H

#include <Arduino.h>
#include <Wire.h>


#define BMA250_INT_PIN            32

// Definations
  // -----------------------------------------
  enum BMA250Range {
    RANGE_2G = 0x03,  // DEFAULT
    RANGE_4G = 0x05,
    RANGE_8G = 0x08,
    RANGE_16G = 0x0C
  };

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

  enum BMA250InterruptType {
    INT_SLOPE_X = 0x01,
    INT_SLOPE_Y = 0x02,
    INT_SLOPE_Z = 0x04,
    INT_DOUBLE_TAP = 0x10,
    INT_ORIENT = 0x40
  };

  enum BMA250InterruptPin {
    INT1 = 0x01,
    INT2 = 0x02
  };

  enum BMA250RstLatch {
  INT_MODE_8s  =  0x06,
  INT_MODE_4s  = 0x05,
  INT_MODE_1s  = 0x03,
  INT_MODE_50ms= 0x0E,
  INT_MODE_1ms= 0x0B,
  INT_MODE_LATCHED = 0x0F
  };

  enum TapFace {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    BACK,
    FRONT,
    NONE
  };


class BMA250{
  private:    
    int16_t readAxis(uint8_t msbReg, uint8_t lsbReg);
    
  public:
    void (*TopAction)()=NULL, (*BottomAction)()=NULL, (*LeftAction)()=NULL,
         (*RightAction)()=NULL, (*FrontAction)()=NULL, (*BackAction)()=NULL;
    void intilize(void);
    uint8_t readRegister(uint8_t RegAddr);
    void writeRegister(uint8_t RegAddr, uint8_t value);
    float readXaxis(void);
    float readYaxis(void);
    float readZaxis(void);
    void setRange(BMA250Range range);
    void setBandwidth(BMA250Bandwidth bw);
    void attachDoubleTapToAction(TapFace face, void (*action)());
    TapFace getLastTapFace(void);
};



#endif