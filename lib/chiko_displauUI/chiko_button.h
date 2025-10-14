#ifndef __CHIKO_BUTTON__
#define __CHIKO_BUTTON__

#include <chiko_definations.h>
#include <U8g2lib.h>

class button{
    private:
    U8G2 _display;
    int _x_pos, _y_pos;
    uint shadow_offset = 2, _h_margin, _w_margin, _h_button, _w_button, _h_text, _w_text;
    const char *_title;
    bool _rounded=true, _shadow=false, _isSelected=false;
    UIorigin _origin = LEFTTOP;

    
    public:
    void create(U8G2& display, const char *title,UIorigin origin=LEFTTOP, int x_pos=0, int y_pos=0, uint h_margin=2, uint w_margin=4, bool rounded=true, bool shadow=false);
    void dropShadow(bool shadow=true, bool autoUpdateButton=true);
    void select(bool _isSelected=true, bool autoUpdateButton=true);
    void update(void);
    void setOrigin(UIorigin _origin = LEFTTOP,bool autoUpdateButton=true);
    void clear(void);
};

#endif