#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include <chiko_definations.h>


class text{
public:
  void create(U8G2& display, const char *placeholder, UIorigin origin,
              int x_pos, int y_pos, uint h_margin = 0, uint w_margin = 0,
              bool rounded = true, bool shadow = false);

  void setText(const char *text, bool autoUpdate = true);
  void update();
  void clear();

private:
  U8G2 _display;
  UIorigin _origin;
  int _x_pos, _y_pos;
  uint _w_margin, _h_margin;
  uint _w_field, _h_field;
  uint _h_text;
  bool _rounded, _shadow, _isFocused;
  String _text, _placeholder;

  void applyOrigin();
};
