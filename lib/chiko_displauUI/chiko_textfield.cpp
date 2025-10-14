#include "chiko_textfield.h"


void textfield::create(U8G2 &display,
                       int y_pos,
                       const char* label,
                       const char* placeholder,
                       bool rounded,
                       bool shadow,
                       uint8_t h_margin,
                       uint8_t w_margin,
                       uint8_t gap,
                       uint8_t outer_left,
                       uint8_t outer_right)
{
  _display = display;
  _y_pos = y_pos;
  _label = label;
  _placeholder = placeholder;
  _rounded = rounded;
  _shadow = shadow;
  _h_margin = h_margin;
  _w_margin = w_margin;
  _gap = gap;
  _outer_left = outer_left;
  _outer_right = outer_right;

  _w_disp = _display.getDisplayWidth();
  ensureFontAndHeight();
  layout();
}

void textfield::setText(const char* text, bool autoUpdate) {
  if (!text) text = "";
  strncpy(_text, text, sizeof(_text) - 1);
  _text[sizeof(_text) - 1] = '\0';
  if (autoUpdate) update();
}

void textfield::setLabel(const char* label, bool autoUpdate) {
  _label = label;
  layout();
  if (autoUpdate) update();
}

void textfield::setPlaceholder(const char* placeholder, bool autoUpdate) {
  _placeholder = placeholder;
  if (autoUpdate) update();
}

void textfield::setFont(const uint8_t* font, bool autoUpdate) {
  if (font) _font = font;
  ensureFontAndHeight();
  layout();
  if (autoUpdate) update();
}

void textfield::focus(bool focused, bool autoUpdate) {
  _focused = focused;
  if (autoUpdate) update();
}

void textfield::showCursor(bool visible, bool autoUpdate) {
  _cursorVisible = visible;
  if (autoUpdate) update();
}

void textfield::dropShadow(bool shadow, bool autoUpdate) {
  _shadow = shadow;
  if (autoUpdate) update();
}

void textfield::setMargins(uint8_t h_margin, uint8_t w_margin, bool autoUpdate) {
  _h_margin = h_margin;
  _w_margin = w_margin;
  ensureFontAndHeight();
  layout();
  if (autoUpdate) update();
}

void textfield::setOuterPadding(uint8_t left, uint8_t right, bool autoUpdate) {
  _outer_left = left;
  _outer_right = right;
  layout();
  if (autoUpdate) update();
}

void textfield::setGap(uint8_t gap, bool autoUpdate) {
  _gap = gap;
  layout();
  if (autoUpdate) update();
}

void textfield::update() {
  // Recompute in case font/label changed externally
  ensureFontAndHeight();
  layout();
  clearRow();
  drawLabel();
  drawFieldFrame();
  drawFieldTextAndCaret();
  _display.sendBuffer();
}

// --- Private helpers ---

void textfield::ensureFontAndHeight() {
  if (_font) _display.setFont(_font);
  _h_text = _display.getMaxCharHeight();
  _h_field = (uint8_t)(_h_text + 2 * _h_margin);
}

void textfield::layout() {
  // Ensure display width
  _w_disp = _display.getDisplayWidth();

  // Minimum field width to keep it usable
  const uint16_t MIN_FIELD_W = 24;

  // Full row inner span available for: [outer_left] LABEL [gap] FIELD [outer_right]
  int16_t x = _outer_left;

  // Measure full label width (can be 0)
  uint16_t label_full_px = 0;
  if (_label && *_label) {
    label_full_px = _display.getStrWidth(_label);
  }

  // Max label pixels allowed to still keep a minimal field
  uint16_t max_label_px = 0;
  if (_w_disp > (_outer_left + _outer_right + _gap + MIN_FIELD_W)) {
    max_label_px = _w_disp - _outer_left - _outer_right - _gap - MIN_FIELD_W;
  } else {
    max_label_px = 0;
  }

  // Actual label pixels to draw
  _label_draw_px = (label_full_px > max_label_px) ? max_label_px : label_full_px;

  // Field position & size (rest of the row)
  _x_field = x + (int16_t)_label_draw_px + (int16_t)_gap;
  int32_t remaining = (int32_t)_w_disp - (int32_t)_outer_right - (int32_t)_x_field;
  _w_field = (remaining > 0) ? (uint16_t)remaining : 0;

  // Guard field width minimum
  if (_w_field < MIN_FIELD_W) _w_field = MIN_FIELD_W;
}

void textfield::clearRow() {
  // Clear entire row area incl. potential shadow space on field
  // _display.setColorIndex(0);
  // _display.drawBox(0, _y_pos, _w_disp, (uint16_t)_h_field + shadow_offset);
  // _display.setColorIndex(1);
}

void textfield::drawLabel() {
  if (!(_label && *_label)) return;

  // Draw up to _label_draw_px from left
  int16_t tx = _outer_left;
  int16_t ty = _y_pos + _h_margin + _h_text - 1;

  int16_t drawn = 0;
  for (const char* p = _label; *p; ++p) {
    char t[2] = { *p, 0 };
    int16_t cw = _display.getStrWidth(t);
    if (drawn + cw > (int16_t)_label_draw_px) break;
    _display.drawStr(tx + drawn, ty, t);
    drawn += cw;
  }
}

void textfield::drawFieldFrame() {
  // Optional shadow under the field rect
  if (_shadow) {
    if (_rounded)
      _display.drawRBox(_x_field + shadow_offset, _y_pos + shadow_offset, _w_field, _h_field, 4);
    else
      _display.drawBox(_x_field + shadow_offset, _y_pos + shadow_offset, _w_field, _h_field);
  }

  // Clear field region
  _display.setColorIndex(0);
  if (_rounded)
    _display.drawRBox(_x_field, _y_pos, _w_field, _h_field, 4);
  else
    _display.drawBox(_x_field, _y_pos, _w_field, _h_field);
  _display.setColorIndex(1);

  // // Frame
  // if (_rounded)
  //   _display.drawRFrame(_x_field, _y_pos, _w_field, _h_field, 4);
  // else
  //   _display.drawFrame(_x_field, _y_pos, _w_field, _h_field);

  // Focus ring
  if (_focused && _w_field > 2 && _h_field > 2) {
    if (_rounded)
      _display.drawRFrame(_x_field + 1, _y_pos + 1, _w_field - 2, _h_field - 2, 3);
    else
      _display.drawFrame(_x_field + 1, _y_pos + 1, _w_field - 2, _h_field - 2);
  }
}

void textfield::drawFieldTextAndCaret() {
  const int16_t inner_w = (int16_t)_w_field - 2 * (int16_t)_w_margin;
  if (inner_w <= 0) return;

  const int16_t tx = _x_field + _w_margin;
  const int16_t ty = _y_pos + _h_margin + _h_text - 1;

  const char* s = (_text[0] != '\0') ? _text : _placeholder;
  int16_t x = tx;

  if (s && *s) {
    // Draw only what fits (no scrolling in this simplified version)
    for (const char* p = s; *p; ++p) {
      char t[2] = { *p, 0 };
      int16_t cw = _display.getStrWidth(t);
      if (x + cw > tx + inner_w) break;
      _display.drawStr(x, ty, t);
      x += cw;
    }
  }

  // Caret at the end of drawn text
  if (_focused && _cursorVisible) {
    int16_t caret_x = x;
    if (caret_x > tx + inner_w) caret_x = tx + inner_w;
    int16_t top = _y_pos + 2;
    int16_t height = (_h_field >= 4) ? (_h_field - 4) : _h_field;
    _display.drawVLine(caret_x, top, height);
  }
}

