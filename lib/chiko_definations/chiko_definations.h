#ifndef __CHIKO_DEFINATIONS__
#define __CHIKO_DEFINATIONS__

#define CHIKO_LOGS_ENABLE
#include <functional>


#define LED_PIN                 17
#define CHARGE_INDICATOR_PIN    16

#define POWER_BUTTON_PIN        33
#define FUNCTION_BUTTON_PIN     0

// Battery monitoring constants
#define BATTERY_VOLTAGE_PIN     34 // Pin to read battery voltage (ADC1_6)
#define VOLTAGE_DIVIDER_RATIO   2.0 // Assuming a 1:1 voltage divider (adjust if different)
#define BATTERY_LOW_VOLTAGE     3.3 // Voltage threshold for low battery (in volts)
#define BATTERY_CHECK_INTERVAL_MS 1000 // Check battery every 1 seconds
#define BATTERY_AVERAGE_SAMPLES 10

#define EYE_SCAN_DELAY          500

#define NUMBER_OF_PAGES         6

#define FACE_PN                 0

#define JOINT_ENABLE_PN         1

#define LEFTLEG_CALIB_PN        2
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
    LOOK_DOWN
};

struct pageConfig{
        char btnFunName[10] = "Enter";
        char btnPwName[10]  = "Power";
        char btnRstName[10] = "Reset";
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