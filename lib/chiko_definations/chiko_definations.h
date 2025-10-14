#ifndef __CHIKO_DEFINATIONS__
#define __CHIKO_DEFINATIONS__

#define CHIKO_LOGS_ENABLE

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

enum PageInfo{
    FACE_PAGE,
    JOINTS_PAGE,
    WIFI_PAGE,
    BLUETOOTH_PAGE,
    ALARM_PAGE
};

struct pageConfig{
        char btnFunName[10] = "Enter";
        char btnPwName[10]  = "Power";
        char btnRstName[10] = "Reset";
        bool btnFunVisibility = true;
        bool btnPwVisibility = true;
        bool btnRstVisibility = true;
        bool taskBarVisibility = true;
        PageInfo _info = FACE_PAGE;
        void (*btnPwClickAction)() = nullptr;
        void (*btnFunClickAction)() = nullptr;
    };



#endif