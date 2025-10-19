#ifndef ESP32_DISPLAY_UI_H
#define ESP32_DISPLAY_UI_H

#include <chiko_definations.h>
#include <chiko_button.h>
#include <chiko_checkbox.h>
#include <chiko_textfield.h>
#include <chiko_text.h>
#include <chiko_taskbar.h>
#include <chiko_logs.h>

#include <U8g2lib.h>
#include <chiko_face.h>
#include <stdint.h>
#include <stddef.h>



class pages {
    private:
        taskbar _pageTaskBar;
        uint8_t _pageCount, _currentPage = 0 , _previousPage=99;
        U8G2 _display;
        button _fun,_reset,_power;
        checkbox _jointCheckBox;
        textfield _textField;
        text _title;
        
        pageConfig _pageConfigs[NUMBER_OF_PAGES];

        eyesConfig _eyesScanDirection = LOOK_STRIGHT;
        void drawFace();
        void jointCalibrationPage_setup(uint8_t PN);
        void jointCalibrationPage_update(uint8_t PN);
    public:
    

    void intilize(U8G2 &diplay);

    pageConfig getPageConfig(uint8_t pageNumber);
    void setPageConfig(uint8_t pageNumber, pageConfig config);

    void gotoNextPage();
    void gotoPreviousPage();
    uint8_t getSelectedPageNumber();
    uint8_t getPageCount();
    void gotoPageNumber(uint8_t pageNumber);
    void gotoDefaultPage();
    
    void pageSetup();
    void printPageInfo();
    
    void updatePage(bool forceUpdate=false);

    void btnFunctionAction();
    void btnPowerAction();

    void btnFunctionSelect(bool select=true);
    void btnPowerSelect(bool select=true);

    void setEyesScanDirection(eyesConfig ESD);
    eyesConfig getEyesScanDirection();
    
    U8G2 getDisplayHandel();
};


#endif
