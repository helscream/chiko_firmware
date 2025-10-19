#include "chiko_text.h"

void text::create(U8G2& display, const char *placeholder, UIorigin origin,
                        int x_pos, int y_pos, uint h_margin, uint w_margin,
                        bool rounded, bool shadow) {
  _display = display;
  _placeholder = placeholder ? String(placeholder) : "";
  _origin = origin;
  _x_pos = x_pos;
  _y_pos = y_pos;
  _h_margin = h_margin;
  _w_margin = w_margin;
  _rounded = rounded;
  _shadow = shadow;
  _isFocused = false;

  _display.setFont(u8g2_font_5x7_mf);
  _h_text = _display.getMaxCharHeight();
//   _w_field = width_pixels;
  _w_field = _display.getStrWidth(placeholder);
  _h_field = _h_text + 2 * _h_margin;

  applyOrigin();
  update();
}

void text::setText(const char *text, bool autoUpdate) {
  _text = text ? String(text) : "";
  _w_field =_display.getStrWidth(text);
  if (autoUpdate) update();
}


void text::update() {
  clear();

  if (_shadow) _display.drawBox(_x_pos + 2, _y_pos + 2, _w_field, _h_field);

  String toDraw = _text.length() > 0 ? _text : _placeholder;
  uint text_x = _x_pos + _w_margin;
  uint text_y = _y_pos + _h_margin + _h_text - 1;


  _display.drawStr(text_x, text_y, toDraw.c_str());
  _display.sendBuffer();
}

void text::clear() {
  _display.setColorIndex(0);
  _display.drawBox(_x_pos, _y_pos, _w_field + 2, _h_field + 2);
  _display.setColorIndex(1);
}

void text::applyOrigin() {
  switch (_origin) {
    case LEFTTOP: break;
    case LEFTMIDDLE: _y_pos -= _h_field / 2; break;
    case LEFTBOTTOM: _y_pos -= _h_field; break;
    case MIDTOP: _x_pos -= _w_field / 2; break;
    case MIDDLE: _x_pos -= _w_field / 2; _y_pos -= _h_field / 2; break;
    case MIDBOTTOM: _x_pos -= _w_field / 2; _y_pos -= _h_field; break;
    case RIGHTTOP: _x_pos -= _w_field; break;
    case RIGHTMIDDLE: _x_pos -= _w_field; _y_pos -= _h_field / 2; break;
    case RIGHTBOTTOM: _x_pos -= _w_field; _y_pos -= _h_field; break;
  }
}