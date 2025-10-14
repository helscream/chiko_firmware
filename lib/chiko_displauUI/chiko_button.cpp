#include "chiko_button.h"



#include <U8g2lib.h>

void button::create(U8G2& display, const char *title, UIorigin origin, int x_pos, int y_pos, uint h_margin, uint w_margin, bool rounded, bool shadow) {
  _display = display;
  _title = title;
  _x_pos = x_pos;
  _y_pos = y_pos;
  _h_margin = h_margin;
  _w_margin = w_margin;
  _shadow = shadow;
  _rounded = rounded;
  _origin = origin;

  // Set font before measuring text
  _display.setFont(u8g2_font_5x7_mf); // Change font as needed

  // Measure text dimensions
  _w_text = _display.getStrWidth(_title);
  _h_text = _display.getMaxCharHeight();

  // Calculate button dimensions
  _w_button = _w_text + 2 * _w_margin;
  _h_button = _h_text + 2 * _h_margin;

  setOrigin(_origin,false);
}

void button::dropShadow(bool shadow, bool autoUpdateButton){
    _shadow = shadow;
    if (autoUpdateButton){
        update();
    }
}

void button::setOrigin(UIorigin origin,bool autoUpdateButton){
    _origin = origin;
    switch (_origin)
    {
    case LEFTTOP:
        // Do nothing
        break;
    case LEFTMIDDLE:
        _y_pos = _y_pos - round((float)_h_button/2);
        break;
    case LEFTBOTTOM:
        _y_pos = _y_pos - _h_button;
        break;
    case MIDTOP:
        _x_pos = _x_pos - round((float)_w_button/2);
        break;
    case MIDDLE:
        _x_pos = _x_pos - round((float)_w_button/2);
        _y_pos = _y_pos - round((float)_h_button/2);
        break;
    case MIDBOTTOM:
        _x_pos = _x_pos - round((float)_w_button/2);
        _y_pos = _y_pos - _h_button;
        break;
    case RIGHTTOP:
        _x_pos = _x_pos - _w_button;
        break;
    case RIGHTMIDDLE:
        _x_pos = _x_pos - _w_button;
        _y_pos = _y_pos - round((float)_h_button/2);
        break;
    case RIGHTBOTTOM:
        _x_pos = _x_pos - _w_button;
        _y_pos = _y_pos - _h_button;
        break;
    default:
        break;
    }
    if (autoUpdateButton){
        update();
    }
}

void button::select(bool isSelected, bool autoUpdateButton){
    _isSelected = isSelected;
    if (autoUpdateButton){
        update();
    }
}

void button::update(void){
  clear();

  // Optional drop shadow
  if (_shadow) {
    _display.setColorIndex(1); // Draw Shadow
  }else{
    _display.setColorIndex(0); // Clear Shadow
  }

  
  if (_rounded) {
        _display.drawRBox(_x_pos + shadow_offset, _y_pos + shadow_offset, _w_button, _h_button, 4);
        // Clear place for button
        _display.setColorIndex(0);
        _display.drawRBox(_x_pos, _y_pos, _w_button, _h_button, 4); // 4 = corner radius
        _display.setColorIndex(1);
    } else {
        _display.drawBox(_x_pos + shadow_offset, _y_pos + shadow_offset, _w_button, _h_button);
        // Clear place for button
        _display.setColorIndex(0);
        _display.drawBox(_x_pos, _y_pos, _w_button, _h_button);
        _display.setColorIndex(1);
    }

  // Draw button shape
  if (_rounded) {
    if(_isSelected){
        _display.drawRBox(_x_pos, _y_pos, _w_button, _h_button, 4); // 4 = corner radius
    }else{
        _display.drawRFrame(_x_pos, _y_pos, _w_button, _h_button, 4); // 4 = corner radius
    }
  } else {
    if(_isSelected){
        _display.drawBox(_x_pos, _y_pos, _w_button, _h_button);
    }else{
        _display.drawFrame(_x_pos, _y_pos, _w_button, _h_button);
    }
  }

  // Draw centered text inside the button
  uint text_x = _x_pos + _w_margin;
  uint text_y = _y_pos + _h_margin + _h_text - 1; // Adjust for baseline
  if(_isSelected){
    _display.setColorIndex(0);
  }else{
    _display.setColorIndex(1);
  }
  _display.drawStr(text_x, text_y, _title);
  _display.setColorIndex(1);
  _display.sendBuffer();
}

void button::clear(){
    _display.setColorIndex(0);
    _display.drawBox(_x_pos, _y_pos, _w_button+shadow_offset, _h_button+shadow_offset);
    _display.setColorIndex(1);
    _display.sendBuffer();
}

