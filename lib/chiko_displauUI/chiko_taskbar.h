#ifndef __CHIKO_TASKBAR__
#define __CHIKO_TASKBAR__

#include <chiko_definations.h>
#include <U8g2lib.h>

#pragma once
class taskbar {
  private:
    U8G2 _display;

    int16_t   _x_pos = 0, _y_pos = 0;
    uint16_t  _w_bar = 0;          // Full bar width (uses entire screen width)
    uint8_t   _h_bar = 9;         // Bar height
    const char **_titles = nullptr;
    uint8_t   _count = 0;
    int8_t    _selected = -1;      // -1 = no selection
    bool      _rounded = true;
    bool      _shadow  = false;
    bool      _auto_numbers = false;
    UIorigin  _origin = LEFTTOP;
    const uint8_t shadow_offset = 2;

    // --- Helpers ---

    void layoutSegment(uint8_t index, int16_t &x, uint16_t &w) const;
    void drawSeparators(void);
    void drawSelection(void);
    void drawTitles(void);
    void setFont(); // Sets the font used for titles
    void applyOrigin(bool autoUpdate);

  public:
    
    int8_t getSelectedIndex();  
    // Create with explicit titles
    void create(U8G2 &display,
                const char *titles[], uint8_t count,
                UIorigin origin = LEFTTOP,
                int x_pos = 0, int y_pos = 0,
                uint8_t bar_height = 9,
                bool rounded = true, bool shadow = false);

    // Create with just a count (auto-number titles "1","2","3"...)
    void create(U8G2 &display,
                uint8_t count,
                UIorigin origin = LEFTTOP,
                int x_pos = 0, int y_pos = 0,
                uint8_t bar_height = 9,
                bool rounded = true, bool shadow = false);

    void setTitles(const char *titles[], uint8_t count, bool autoUpdate = true);
    void setOrigin(UIorigin origin = LEFTTOP, bool autoUpdate = true);
    void dropShadow(bool shadow = true, bool autoUpdate = true);
    void select(int8_t index = -1, bool autoUpdate = true);
    void update(void);
    void clear(void);
};


#endif