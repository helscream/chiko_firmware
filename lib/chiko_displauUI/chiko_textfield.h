#ifndef __CHIKO_TEXTFIELD__
#define __CHIKO_TEXTFIELD__

#include <chiko_definations.h>
#include <U8g2lib.h>



#pragma once

class textfield {
  public:
    // Create a full-width row with a label (left) and a field (right).
    // Only y_pos is required. Height is derived from the font + margins.
    void create(U8G2 &display,
                int y_pos,
                const char* label,
                const char* placeholder = nullptr,
                bool rounded = true,
                bool shadow = false,
                uint8_t h_margin = 2,
                uint8_t w_margin = 0,
                uint8_t gap = 0,               // space between label and field
                uint8_t outer_left = 0,        // left padding for the whole row
                uint8_t outer_right = 0);      // right padding for the whole row

    // Content & styling
    void setText(const char* text, bool autoUpdate = true);
    void setLabel(const char* label, bool autoUpdate = true);
    void setPlaceholder(const char* placeholder, bool autoUpdate = true);
    void setFont(const uint8_t* font, bool autoUpdate = true);
    void focus(bool focused = true, bool autoUpdate = true);
    void showCursor(bool visible = true, bool autoUpdate = true);
    void dropShadow(bool shadow = true, bool autoUpdate = true);
    void setMargins(uint8_t h_margin, uint8_t w_margin, bool autoUpdate = true);
    void setOuterPadding(uint8_t left, uint8_t right, bool autoUpdate = true);
    void setGap(uint8_t gap, bool autoUpdate = true);

    // Render
    void update();

    // Accessors
    const char* text() const { return _text; }
    const char* label() const { return _label ? _label : ""; }
    const char* placeholder() const { return _placeholder ? _placeholder : ""; }

  private:
    // Layout + drawing
    void ensureFontAndHeight();
    void layout();             // compute label width use and field rect
    void clearRow();
    void drawLabel();
    void drawFieldFrame();
    void drawFieldTextAndCaret();

  private:
    // Display & geometry
    U8G2      _display;
    int16_t   _y_pos = 0;
    uint16_t  _w_disp = 0;         // full display width
    uint16_t  _w_field = 0;        // computed field width (right side)
    int16_t   _x_field = 0;        // computed field x
    uint8_t   _h_field = 0;        // from font + margins

    // Styling
    uint8_t   _h_margin = 2, _w_margin = 3; // inside field content margins
    uint8_t   _outer_left = 0, _outer_right = 0; // outer paddings
    uint8_t   _gap = 6;           // space between label and field
    bool      _rounded = true, _shadow = false;

    // Content
    const char* _label = nullptr;
    const char* _placeholder = nullptr;
    char        _text[64] = {0};
    bool        _focused = false;
    bool        _cursorVisible = true;

    // Font
    const uint8_t* _font = u8g2_font_6x12_mf; // default to match your button example
    int16_t        _h_text = 0;                  // cached font height

    // Internal cache (for layout)
    uint16_t  _label_draw_px = 0; // how many pixels of label will be drawn

    // Misc
    static constexpr uint8_t shadow_offset = 2;
};


#endif