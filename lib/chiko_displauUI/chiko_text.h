#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include <chiko_definations.h>


class text{
public:
  void create(U8G2 display, const char *placeholder, UIorigin origin,
              int  x_orign, int  y_orign, const uint8_t* font = u8g2_font_5x7_mf,
              uint h_margin = 0, uint w_margin = 0);

  void setText(const char *text, bool autoUpdate = true);
  void update();
  void clear();
  void setFont(const uint8_t* font, bool autoUpdate = true);

private:
  U8G2 _display;
  UIorigin _origin;
  int _x_pos, _y_pos, _x_orign, _y_orign;
  uint _w_margin, _h_margin;
  uint _w_field, _h_field;
  uint _h_text;
  bool  _isFocused;
  String _text, _placeholder;
   const uint8_t* _font = u8g2_font_5x7_mf; // default to match your button example

  void applyOrigin();
};
