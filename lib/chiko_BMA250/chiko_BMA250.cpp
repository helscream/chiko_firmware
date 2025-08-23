#include <chiko_BMA250.h> 



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

TapFace LastTap = NONE; 
bool FLAG_DoubleTap = false;

void BMA250_INIT_EVENT(void){
  FLAG_DoubleTap = true;
}

void DoubleTapTask(void* param){
  BMA250 *obj;
  obj = (BMA250*) param;
   Serial.println("BMA250 DoupleTap Task Started! ");
  uint8_t intrupt_direction = 0;
  while(1){
    if (FLAG_DoubleTap){
      FLAG_DoubleTap = false;
      switch (intrupt_direction) {
        case B11000000:
          LastTap = TOP;
          if (obj->TopAction != NULL){
            obj->TopAction();
          }
          Serial.println("DoubleTap: Top");
          break;
        
        case B10100000:
          LastTap = FRONT;
          if (obj->FrontAction != NULL){
            obj->FrontAction();
          }
          Serial.println("DoubleTap: Front");
          break;
        
        case B00010000:
          LastTap = LEFT;
          if (obj->LeftAction != NULL){
            obj->LeftAction();
          }
          Serial.println("DoubleTap: Left");
          break;
        
        case B10010000:
          LastTap = RIGHT;
          if (obj->RightAction != NULL){
            obj->RightAction();
          }
          Serial.println("DoubleTap: Right");
          break;
        
        case B01000000:
          LastTap = BOTTOM;
          if (obj->BottomAction != NULL){
            obj->BottomAction();
          }
          Serial.println("DoubleTap: Bottom");
          break;

        case B00100000:
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

void BMA250::setRange(BMA250Range range){
  writeRegister(BMA250_REG_RANGE,(uint8_t)range);
}

// Reading register
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

// Write register
void BMA250::writeRegister(uint8_t RegAddr, uint8_t value){
  Wire.beginTransmission(BMA250_I2C_ADDR);
  Wire.write(RegAddr);
  Wire.write(value);
  Wire.endTransmission();
}

void BMA250::setBandwidth(BMA250Bandwidth bw){
// Register 0X10 (PMU_BW)
// BITs   7              6              5               4              3                 2                 1               0
//        reserve(7:5)                                   bw<4:0>         
  writeRegister(BMA250_REG_BW,(uint8_t)bw);
}

int16_t BMA250::readAxis(uint8_t msbReg, uint8_t lsbReg) {
  uint8_t lsb = readRegister(lsbReg); // The LSB must always be read first to keep the integrety of data
  uint8_t msb = readRegister(msbReg);
  // Combine MSB and LSB (10-bit value)
  int16_t raw = ((int16_t)msb << 2) | (lsb >> 6);
  // Convert to signed 10-bit
  if (raw > 511) raw -= 1024;
  return raw;
}

float BMA250::readXaxis(void){
  int16_t x = readAxis(0x03, 0x02); // X axis
  // Convert raw to g-force (assuming ±2g range, 256 LSB/g)
  float g = x / 256.0;
  return g;
}

float BMA250::readYaxis(void){
  int16_t y = readAxis(0x05, 0x04); // Y axis
  // Convert raw to g-force (assuming ±2g range, 256 LSB/g)
  float g = y / 256.0;
  return g;
}

float BMA250::readZaxis(void){
  int16_t z = readAxis(0x07, 0x06); // Z axis
  // Convert raw to g-force (assuming ±2g range, 256 LSB/g)
  float g = z / 256.0;
  return g;
}

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

//unit8_t tempReg = B00000000;
void BMA250::intilize(void) { 
  pinMode(BMA250_INT_PIN, INPUT); // BMA250 is configured at active high
  attachInterrupt(BMA250_INT_PIN, BMA250_INIT_EVENT, FALLING);

  // Initialize sensor
  Wire.begin(); // SDA = GPIO21, SCL = GPIO22 by default on ESP32
  
  writeRegister(BMA250_REG_BGW_SOFTRESET, 0xB6);
  // Reading Chip ID:
  Serial.print("Chip ID: ");
  Serial.println(readRegister(BMA250_REG_CHIP_ID));

  // Optional: Set sensor to normal mode (register 0x11, value 0x00)
  
  writeRegister(BMA250_REG_POWER_MODE, 0x00);

  Serial.print("Power Mode: ");
  Serial.println(readRegister(BMA250_REG_POWER_MODE));

  // Set Range of the sensor
  setRange(RANGE_2G);

  // Set Bandwidth
  setBandwidth(BW_125HZ);

  // Set INT latch reset timer;
  // Register 0x21 (INT_RST_LATCH)
  // BITs   7           6          5         4          3        2         1        0
  //        reset_int   Reserved(6:4)                       latch_int<3:0>
  writeRegister(BMA250_REG_INT_RST_LATCH,INT_MODE_1ms);

  // Setting put interupt threshold for any motion
  // Register 0x28 (INT_6)
  // BITs   7           6          5         4             3               2         1        0
  //         slope_th<7:4>                                     slope_th<3:0>
  writeRegister(BMA250_REG_INT_6,0x04);

  // Setting Tap configuration
  // Register 0x2A (INT_8)
  // BITs   7           6          5         4             3               2         1        0
  //        tap_quiet   tap_shock  reserved  reserved   reserved      tap_dur<2:0>
  //    tap_quite     |     tap_shock     |   tap_dur
  //    0b    30ms    |      0b   50ms    |   000b  50ms
  //    1b    20ms    |      1b   75ms    |   001b  100ms
  //                  |                   |   010b  150ms
  //                  |                   |   011b  200ms
  //                  |                   |   100b  250ms
  //                  |                   |   101b  375ms
  //                  |                   |   110b  500ms
  //                  |                   |   111b  700ms
  writeRegister(BMA250_REG_INT_8,B10000110);

  // Setting up tap thresholds
  // Register 0x2B (INT_9) 
  // BITs   7           6          5         4             3               2         1        0
  //        tap_samp<7:6>          reserved     tap_th<4:0> 
  //    tap_samp          |       tap_th  
  //   00b  2 samples     |   
  //   01b  4 samples     |
  //   10b  8 samples     |
  //   11b  16 samples    |
  writeRegister(BMA250_REG_INT_9,B10001000);
  
  // Configuring the INT1 outpin
  // Register 0X20 (INT_OUT_CTRL)
  // BITs   7           6          5         4        3             2         1             0
  //       Reserved (7:4)                            int2_od    int2_lvl   int1_od   int1_lvl
  writeRegister(BMA250_REG_INT_OUT_CTRL,B00000001);

  // Configuring the source of data to the double tap intrupt engine
  // Register 0x1E (INT_SRC)
  // BITs   7           6          5             4             3               2         1               0
  //         reserved (7:6)        int_src_data  int_src_tap   int_src_slo_no_mot        int_src_slope    int_src_low
  // 0 -> Filtered data,      1 -> unfiltered data
 writeRegister(BMA250_REG_INT_SRC,B00000000); // INT mapped for double tap



  // Enabling any motion detection on x, y, & z axes or for taps
  // Register 0x16 (INT_En_0)
  // BITs   7           6            5             4             3                 2                 1               0
  //        flat_en    orient_en     s_tap_en      d_tap_en      reserved          slope_en_z        slope_en_y      slope_en_x
  // writeRegister(BMA250_REG_INT_EN_0,B00000111); // INT mapped for any motion
  writeRegister(BMA250_REG_INT_EN_0,B00010000); // INT mapped for double tap


  // Mapping the interupt to INT1
  //  Register 0x19 (INT_MAP_0)
  // BITs   7           6            5             4             3                 2           1               0
  //        int1_flat   int1_orient  int1_s_tap     int1_d_tap   int1_slo_no_mot   int1_slope  int1_high        int1_low
  // writeRegister(BMA250_REG_INT_MAP_0, B00000100); // Mapped for any motion
  writeRegister(BMA250_REG_INT_MAP_0, B00010000); // Mapped for douple tap
  
  xTaskCreate(DoubleTapTask,"Double Tap Task",2048,this,1,NULL);
}

TapFace getLastTapFace(void){
  return LastTap;
}