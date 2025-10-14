#include "chiko_taskbar.h"
#include <math.h>    // round
#include <stdio.h>   // snprintf

// ---------------- Private helpers ----------------

void taskbar::setFont() {
  // Same font as your button example; adjust as needed
  _display.setFont(u8g2_font_5x7_mf);
}

void taskbar::clear(void) {
  _display.setColorIndex(0);
  _display.drawBox(_x_pos, _y_pos, _w_bar + shadow_offset, _h_bar + shadow_offset);
  _display.setColorIndex(1);
  _display.sendBuffer();
}

void taskbar::applyOrigin(bool autoUpdate) {
  // Full width bar; origin adjusts (x,y) anchor point
  switch (_origin) {
    case LEFTTOP:       /* keep x,y */ break;
    case LEFTMIDDLE:    _y_pos = _y_pos - (int16_t)round((float)_h_bar/2); break;
    case LEFTBOTTOM:    _y_pos = _y_pos - _h_bar; break;

    case MIDTOP:        _x_pos = _x_pos - (int16_t)round((float)_w_bar/2); break;
    case MIDDLE:        _x_pos = _x_pos - (int16_t)round((float)_w_bar/2);
                        _y_pos = _y_pos - (int16_t)round((float)_h_bar/2); break;
    case MIDBOTTOM:     _x_pos = _x_pos - (int16_t)round((float)_w_bar/2);
                        _y_pos = _y_pos - _h_bar; break;

    case RIGHTTOP:      _x_pos = _x_pos - (int16_t)_w_bar; break;
    case RIGHTMIDDLE:   _x_pos = _x_pos - (int16_t)_w_bar;
                        _y_pos = _y_pos - (int16_t)round((float)_h_bar/2); break;
    case RIGHTBOTTOM:   _x_pos = _x_pos - (int16_t)_w_bar;
                        _y_pos = _y_pos - _h_bar; break;
    default: break;
  }
  if (autoUpdate) update();
}

void taskbar::layoutSegment(uint8_t index, int16_t &x, uint16_t &w) const {
  if (_count == 0) { x = _x_pos; w = _w_bar; return; }
  uint16_t base = _w_bar / _count;
  uint16_t rem  = _w_bar % _count;

  // Distribute remainder pixels to the leftmost segments for perfect fit
  w = base + (index < rem ? 1 : 0);
  x = _x_pos + (int16_t)(index * base) + (int16_t)(index < rem ? index : rem);
}

void taskbar::drawSeparators(void) {
  if (_count <= 1) return;
  for (uint8_t i = 0; i < _count - 1; ++i) {
    int16_t xi; uint16_t wi;
    layoutSegment(i, xi, wi);
    int16_t sep_x = xi + (int16_t)wi; // right edge of segment
    _display.drawVLine(sep_x, _y_pos + 1, _h_bar - 2);
  }
}

void taskbar::drawSelection(void) {
  if (_selected < 0 || _selected >= (int8_t)_count) return;

  int16_t xi; uint16_t wi;
  layoutSegment((uint8_t)_selected, xi, wi);

  // Inset to keep frame/separators visible
  int16_t fx = xi + 1;
  int16_t fy = _y_pos + 1;
  uint16_t fw = wi > 2 ? wi - 2 : wi;
  uint16_t fh = _h_bar > 2 ? _h_bar - 2 : _h_bar;

  if (_rounded) {
    _display.drawRBox(fx, fy, fw, fh, 3);
  } else {
    _display.drawBox(fx, fy, fw, fh);
  }
}

int8_t taskbar::getSelectedIndex(){
  return _selected;
}

void taskbar::drawTitles(void) {
  if (_count == 0) return;
  setFont();
  int16_t h_text = _display.getMaxCharHeight();

  for (uint8_t i = 0; i < _count; ++i) {
    const char *label = nullptr;
    char numbuf[5]; // up to 4 digits

    if (_auto_numbers) {
      uint16_t n = (uint16_t)i + 1;
      snprintf(numbuf, sizeof(numbuf), "%u", n);
      label = numbuf;
    } else if (_titles) {
      label = _titles[i];
    }

    int16_t xi; uint16_t wi;
    layoutSegment(i, xi, wi);

    if (label && *label) {
      int16_t w_text = _display.getStrWidth(label);
      int16_t tx = xi + (int16_t)((wi - w_text) / 2);
      // Baseline near vertical center
      int16_t ty = _y_pos + (int16_t)((_h_bar - h_text) / 2) + h_text - 1;

      // Invert text over selected background
      if ((int8_t)i == _selected) {
        _display.setColorIndex(0);
      } else {
        _display.setColorIndex(1);
      }
      _display.drawStr(tx, ty, label);
      _display.setColorIndex(1);
    }
  }
}

// ---------------- Public API ----------------

void taskbar::create(U8G2 &display,
                     const char *titles[], uint8_t count,
                     UIorigin origin, int x_pos, int y_pos,
                     uint8_t bar_height, bool rounded, bool shadow)
{
  _display = display;
  _titles = titles;
  _count = count;
  _origin = origin;
  _x_pos = x_pos;
  _y_pos = y_pos;
  _h_bar = bar_height;
  _rounded = rounded;
  _shadow = shadow;
  _auto_numbers = false;

  _w_bar = _display.getDisplayWidth(); // Use entire screen width
  applyOrigin(false);
}

void taskbar::create(U8G2 &display,
                     uint8_t count,
                     UIorigin origin, int x_pos, int y_pos,
                     uint8_t bar_height, bool rounded, bool shadow)
{
  _display = display;
  _titles = nullptr;
  _count = count;
  _origin = origin;
  _x_pos = x_pos;
  _y_pos = y_pos;
  _h_bar = bar_height;
  _rounded = rounded;
  _shadow = shadow;
  _auto_numbers = true;

  _w_bar = _display.getDisplayWidth(); // Use entire screen width
  applyOrigin(false);
}

void taskbar::setTitles(const char *titles[], uint8_t count, bool autoUpdate) {
  _titles = titles;
  _count = count;
  _auto_numbers = (titles == nullptr);
  if (autoUpdate) update();
}

void taskbar::setOrigin(UIorigin origin, bool autoUpdate) {
  _origin = origin;
  applyOrigin(false);
  if (autoUpdate) update();
}

void taskbar::dropShadow(bool shadow, bool autoUpdate) {
  _shadow = shadow;
  if (autoUpdate) update();
}

void taskbar::select(int8_t index, bool autoUpdate) {
  if (index < -1) index = -1;
  if (_count == 0) index = -1;
  if (index >= (int8_t)_count) index = (int8_t)_count - 1;
  _selected = index;
  if (autoUpdate) update();
}

void taskbar::update(void) {
  // Clear previous area (including possible shadow space)
  clear();

  // Optional drop shadow
  if (_shadow) {
    _display.setColorIndex(1);
    if (_rounded) {
      _display.drawRBox(_x_pos + shadow_offset, _y_pos + shadow_offset, _w_bar, _h_bar, 4);
    } else {
      _display.drawBox(_x_pos + shadow_offset, _y_pos + shadow_offset, _w_bar, _h_bar);
    }
  }

  // Clear the main bar area before drawing the frame (so it "pokes through" shadow)
  _display.setColorIndex(0);
  if (_rounded) {
    _display.drawRBox(_x_pos, _y_pos, _w_bar, _h_bar, 4);
  } else {
    _display.drawBox(_x_pos, _y_pos, _w_bar, _h_bar);
  }
  _display.setColorIndex(1);

  // Bar frame
  if (_rounded) {
    _display.drawRFrame(_x_pos, _y_pos, _w_bar, _h_bar, 4);
  } else {
    _display.drawFrame(_x_pos, _y_pos, _w_bar, _h_bar);
  }

  // Fill selection first so separators and frame stay visible
  drawSelection();

  // Draw separators
  drawSeparators();

  // Draw labels
  drawTitles();

  // Flush buffer
  _display.sendBuffer();
}



