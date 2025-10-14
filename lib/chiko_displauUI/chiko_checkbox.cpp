#include "chiko_checkbox.h"



void checkbox::measure() {
  // Font must be set before this; create() does it.
  _w_text = _display.getStrWidth(_title ? _title : "");
  _h_text = _display.getMaxCharHeight();

  _box_size = _h_text;                           // square like the text height
  _h_widget = _h_text + 2 * _h_margin;           // same as button height
  _w_widget = _w_margin + _box_size + _w_margin  // left pad + box + gap
              + _w_text + _w_margin;             // text + right pad
}

void checkbox::create(U8G2& display, const char *title,
                      UIorigin origin, int x_pos, int y_pos,
                      uint h_margin, uint w_margin,
                      bool rounded, bool shadow) {
  _display = display;       // by-value copy like your button
  _title = title ? title : "";
  _x_pos = x_pos;
  _y_pos = y_pos;
  _h_margin = h_margin;
  _w_margin = w_margin;
  _shadow = shadow;
  _rounded = rounded;
  _origin = origin;

  // Match your font choice (so sizes align with the button)
  _display.setFont(u8g2_font_6x12_mf);

  measure();
  setOrigin(_origin, false);
  update();
}

void checkbox::dropShadow(bool shadow, bool autoUpdateButton) {
  _shadow = shadow;
  if (autoUpdateButton) update();
}

void checkbox::setOrigin(UIorigin origin, bool autoUpdateButton) {
  _origin = origin;

  // Mirror your button's destructive positioning logic
  switch (_origin)
  {
    case LEFTTOP:
      // no change
      break;
    case LEFTMIDDLE:
      _y_pos = _y_pos - round((float)_h_widget/2.0f);
      break;
    case LEFTBOTTOM:
      _y_pos = _y_pos - _h_widget;
      break;
    case MIDTOP:
      _x_pos = _x_pos - round((float)_w_widget/2.0f);
      break;
    case MIDDLE:
      _x_pos = _x_pos - round((float)_w_widget/2.0f);
      _y_pos = _y_pos - round((float)_h_widget/2.0f);
      break;
    case MIDBOTTOM:
      _x_pos = _x_pos - round((float)_w_widget/2.0f);
      _y_pos = _y_pos - _h_widget;
      break;
    case RIGHTTOP:
      _x_pos = _x_pos - _w_widget;
      break;
    case RIGHTMIDDLE:
      _x_pos = _x_pos - _w_widget;
      _y_pos = _y_pos - round((float)_h_widget/2.0f);
      break;
    case RIGHTBOTTOM:
      _x_pos = _x_pos - _w_widget;
      _y_pos = _y_pos - _h_widget;
      break;
    default:
      break;
  }

  if (autoUpdateButton) update();
}

void checkbox::select(bool isSelected, bool autoUpdateButton) {
  _isSelected = isSelected;
  if (autoUpdateButton) update();
}

void checkbox::update(void) {
  clear();

  // // --- Shadow background (same pattern as button) ---
  // if (_shadow) {
  //   _display.setColorIndex(1); // draw shadow
  // } else {
  //   _display.setColorIndex(0); // erase shadow
  // }

  // if (_rounded) {
  //   _display.drawRBox(_x_pos + shadow_offset, _y_pos + shadow_offset, _w_widget, _h_widget, 4);
  //   // clear widget area
  //   _display.setColorIndex(0);
  //   _display.drawRBox(_x_pos, _y_pos, _w_widget, _h_widget, 4);
  //   _display.setColorIndex(1);
  // } else {
  //   _display.drawBox(_x_pos + shadow_offset, _y_pos + shadow_offset, _w_widget, _h_widget);
  //   // clear widget area
  //   _display.setColorIndex(0);
  //   _display.drawBox(_x_pos, _y_pos, _w_widget, _h_widget);
  //   _display.setColorIndex(1);
  // }

  // // --- Widget container (to mirror button look) ---
  // if (_rounded) {
  //   if (_isSelected) {
  //     _display.drawRBox(_x_pos, _y_pos, _w_widget, _h_widget, 4);
  //   } else {
  //     _display.drawRFrame(_x_pos, _y_pos, _w_widget, _h_widget, 4);
  //   }
  // } else {
  //   if (_isSelected) {
  //     _display.drawBox(_x_pos, _y_pos, _w_widget, _h_widget);
  //   } else {
  //     _display.drawFrame(_x_pos, _y_pos, _w_widget, _h_widget);
  //   }
  // }

  // --- Checkbox square geometry ---
  int box_x = _x_pos + _w_margin;
  int box_y = _y_pos + (int)(_h_widget - _box_size)/2;

  // Border color: contrast with selection fill (white bg when selected)
  uint8_t borderColor = _isSelected ? 0 : 1;
  _display.setColorIndex(borderColor);

  if (_rounded) {
    _display.drawRFrame(box_x, box_y, _box_size, _box_size, 3);
  } else {
    _display.drawFrame(box_x, box_y, _box_size, _box_size);
  }

  // --- Check / Indeterminate mark ---
  // Foreground color: contrast with background (white on black, black on white)
  uint8_t markColor = _isSelected ? 0 : 1;
  _display.setColorIndex(markColor);

  if (_indeterminate) {
    // small centered bar
    int pad = (int)(_box_size * 0.2f);
    int barH = (int)max(1.0f, _box_size * 0.18f);
    int barX = box_x + pad;
    int barY = box_y + (_box_size - barH)/2;
    _display.drawBox(barX, barY, _box_size - 2*pad, barH);
  } else if (_checked) {
    // classic two-segment check
    int x1 = box_x + (int)(_box_size * 0.20f);
    int y1 = box_y + (int)(_box_size * 0.55f);
    int x2 = box_x + (int)(_box_size * 0.40f);
    int y2 = box_y + (int)(_box_size * 0.75f);
    int x3 = box_x + (int)(_box_size * 0.80f);
    int y3 = box_y + (int)(_box_size * 0.30f);
    _display.drawLine(x1, y1, x2, y2);
    _display.drawLine(x2, y2, x3, y3);
  }

  // --- Label text ---
  uint text_x = box_x + _box_size + _w_margin;
  uint text_y = _y_pos + _h_margin + _h_text - 1; // same baseline style as your button

  // Invert text on selection (same as button)
  _display.setColorIndex(_isSelected ? 0 : 1);
  _display.drawStr(text_x, text_y, _title);

  // Reset & flush
  _display.setColorIndex(1);
  _display.sendBuffer();
}

void checkbox::clear() {
  _display.setColorIndex(0);
  _display.drawBox(_x_pos, _y_pos, _w_widget + shadow_offset, _h_widget + shadow_offset);
  _display.setColorIndex(1);
  _display.sendBuffer();
}

void checkbox::setChecked(bool checked, bool autoUpdateButton) {
  _checked = checked;
  if (_checked) _indeterminate = false; // exclusive states
  if (autoUpdateButton) update();
}

void checkbox::toggle(bool autoUpdateButton) {
  _checked = !_checked;
  if (_checked) _indeterminate = false;
  if (autoUpdateButton) update();
}

void checkbox::setIndeterminate(bool indeterminate, bool autoUpdateButton) {
  _indeterminate = indeterminate;
  if (_indeterminate) _checked = false;
  if (autoUpdateButton) update();
}

void checkbox::setPosition(int x, int y, bool autoUpdateButton) {
  _x_pos = x;
  _y_pos = y;
  if (autoUpdateButton) update();
}

void checkbox::setTitle(const char* title, bool autoUpdateButton) {
  _title = title ? title : "";
  measure();
  if (autoUpdateButton) update();
}
