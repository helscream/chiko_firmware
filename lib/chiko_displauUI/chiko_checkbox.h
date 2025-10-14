#ifndef __CHIKO_CHECKBOX__
#define __CHIKO_CHECKBOX__

#include <chiko_definations.h>
#include <U8g2lib.h>


class checkbox {
  private:
    // Display stored by value (mirrors your button)
    U8G2 _display;

    // Placement
    int _x_pos = 0, _y_pos = 0;
    UIorigin _origin = LEFTTOP;

    // Appearance / layout
    uint shadow_offset = 2;
    uint _h_margin = 1, _w_margin = 2;
    bool _rounded = true;
    bool _shadow  = false;

    // Content
    const char* _title = "";

    // States
    bool _isSelected = false;
    bool _checked = false;
    bool _indeterminate = false;

    // Measurements
    uint _w_text = 0, _h_text = 0;     // text
    uint _box_size = 0;                // checkbox square side
    uint _w_widget = 0, _h_widget = 0; // whole widget (box + text + paddings)

    // Internals
    void clear(void);
    void measure(void);

  public:
    // API mirrored from button
    void create(U8G2& display, const char *title,
                UIorigin origin=LEFTTOP, int x_pos=0, int y_pos=0,
                uint h_margin=1, uint w_margin=2,
                bool rounded=true, bool shadow=false);

    void dropShadow(bool shadow=true, bool autoUpdateButton=true);
    void setOrigin(UIorigin origin=LEFTTOP, bool autoUpdateButton=true);
    void select(bool isSelected=true, bool autoUpdateButton=true);
    void update(void);

    // Checkbox-specific helpers
    void setChecked(bool checked=true, bool autoUpdateButton=true);
    void toggle(bool autoUpdateButton=true);
    void setIndeterminate(bool indeterminate=true, bool autoUpdateButton=true);

    // Convenience
    void setPosition(int x, int y, bool autoUpdateButton=true);
    void setTitle(const char* title, bool autoUpdateButton=true);

    // Getters
    bool isChecked() const { return _checked; }
    bool isIndeterminate() const { return _indeterminate; }
};


#endif