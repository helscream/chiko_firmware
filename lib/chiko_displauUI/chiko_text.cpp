#include "chiko_text.h"

void text::create(U8G2 display, const char *placeholder, UIorigin origin,
                        int  x_orign, int  y_orign, const uint8_t* font, uint h_margin, uint w_margin) {
  _display = display;
  _placeholder = placeholder ? String(placeholder) : "";
  _origin = origin;
   _x_orign =  x_orign;
   _y_orign =  y_orign;
  _h_margin = h_margin;
  _w_margin = w_margin;
  _font = font;
  _isFocused = false;

  _display.setFont(_font);
  _h_text = _display.getMaxCharHeight();
  _w_field = _display.getStrWidth(_placeholder.c_str());

  _h_field = _h_text + 2 * _h_margin;

  applyOrigin();
  update();
}

void text::setFont(const uint8_t* font, bool autoUpdate) {
  if (font) _font = font;
  
if (autoUpdate) update();
}

void text::setText(const char *text, bool autoUpdate) {
  _text = text ? String(text) : "";
  _display.setFont(_font);
  _h_text = _display.getMaxCharHeight();
  _w_field = _display.getStrWidth(_text.c_str());
  _h_field = _h_text + 2 * _h_margin;

  applyOrigin();
  if (autoUpdate) update();
}


void text::update() {  
  String toDraw = _text.length() > 0 ? _text : _placeholder;
  clear();

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
  _x_pos = _x_orign;
  _y_pos = _y_orign;
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