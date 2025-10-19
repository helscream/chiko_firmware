#ifndef __CHIKO_DEFINATIONS__
#define __CHIKO_DEFINATIONS__

#define CHIKO_LOGS_ENABLE

#define CHIKO_NAME      "CHIKO"


const char wifi_SSID[] = "Bubblegum";
const char wifi_PASS[] = "1234567890";




#define LED_PIN                 17
#define CHARGE_INDICATOR_PIN    16

#define POWER_BUTTON_PIN        33
#define FUNCTION_BUTTON_PIN     0

// Battery monitoring constants
#define BATTERY_VOLTAGE_PIN     34 // Pin to read battery voltage (ADC1_6)
#define VOLTAGE_DIVIDER_RATIO   2.0 // Assuming a 1:1 voltage divider (adjust if different)
#define BATTERY_MIN_VOLTAGE     3.3 // Voltage threshold for low battery (in volts)
#define BATTERY_MAX_VOLTAGE     4.2 // Voltage threshold for low battery (in volts)
#define BATTERY_CHECK_INTERVAL_MS 1000 // Check battery every 1 seconds
#define BATTERY_AVERAGE_SAMPLES 10

#define EYE_SCAN_DELAY          500

// Pages configuration
#define NUMBER_OF_PAGES         11

#define FACE_PN                 0

#define BATTERY_INFO_PN         1
#define CLOCK_PN                2
#define ALARM_INFO_PN           3
#define WIFI_INFO_PN            4
#define BLUETOOTH_INFO_PN       5

#define JOINT_ENABLE_PN         6

#define LEFTLEG_CALIB_PN        7
#define LEFTFOOT_CALIB_PN       LEFTLEG_CALIB_PN+1
#define RIGHTLEG_CALIB_PN       LEFTLEG_CALIB_PN+2
#define RIGHTFOOT_CALIB_PN      LEFTLEG_CALIB_PN+3



enum UIorigin{
    LEFTTOP,
    LEFTMIDDLE,
    LEFTBOTTOM,
    MIDTOP,
    MIDDLE,
    MIDBOTTOM,
    RIGHTTOP,
    RIGHTMIDDLE,
    RIGHTBOTTOM
};



enum LEDStates {
  LED_OFF,
  LED_ON_SOLID,
  LED_DOUBLE_BLINK_FADE,
  LED_SINGLE_BLINK_FADE,
  LED_BREATHING
};

enum eyesConfig {
    LOOK_STRIGHT,
    LOOK_LEFT,
    LOOK_RIGHT,
    LOOK_UP,
    LOOK_DOWN,
    SLEEPING
};

#include <functional>
struct pageConfig{
        char btnFunName[16] = "Enter";
        char btnPwName[16]  = "Power";
        char btnRstName[16] = "Reboot";
        bool btnFunVisibility = true;
        bool btnPwVisibility = true;
        bool btnRstVisibility = true;
        bool taskBarVisibility = true;
        std::function<void()> btnPwClickAction = nullptr;
        std::function<void()> btnFunClickAction = nullptr;
        void (*pageConfig)() = nullptr;
        void (*pageUpdate)() = nullptr;
        
    };

enum SelectedJoint {
        LEFTFOOT,
        RIGHTFOOT,
        LEFTLEG,
        RIGHTLEG,
        NONE_SELECTED
};





#endif