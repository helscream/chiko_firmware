
/**
 * @file chiko_face.cpp
 * @brief Implements facial animation and display routines for ChikoBot using the u8g2 graphics library.
 *
 * This file provides functions to draw, animate, and control the robot's eyes and facial expressions,
 * including blinking, saccades, happy/sleepy eyes, and demo/command modes. It also manages a FreeRTOS task
 * for continuous animation and serial command handling.
 */

#include "chiko_face.h"
#include <string>
#include <deque>
#define MAX_LOG_LINES 6 // Number of lines to show (depends on font size and screen height)
static std::deque<std::string> message_log;


// Color definitions for display
int COLOR_WHITE = 1;
int COLOR_BLACK = 0;

// Demo mode state variables
int demo_mode = 0;                        // 1 = demo mode, 0 = command mode
static const int max_animation_index = 8; // Number of available animations
int current_animation_index = 0;          // Current animation index

// Display dimensions
static const int SCREEN_WIDTH = 128;
static const int SCREEN_HEIGHT = 64;

// Reference state for eyes (default positions and sizes)
int ref_left_eye = 32;
int ref_eye_height = 40;
int ref_eye_width = 40;
int ref_space_between_eye = 10;
int ref_corner_radius = 10;

// Current state of the eyes (positions, sizes, and corner radius)
int left_eye_height = ref_eye_height;
int left_eye_width = ref_eye_width;
int left_eye_x = ref_left_eye;
int left_eye_y = ref_left_eye;
int right_eye_x = ref_left_eye + ref_eye_width + ref_space_between_eye;
int right_eye_y = ref_left_eye;
int right_eye_height = ref_eye_height;
int right_eye_width = ref_eye_width;
int corner_radius = ref_corner_radius;



// u8g2 display object for SSD1309 128x64 OLED (hardware SPI)
U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/U8X8_PIN_NONE, /* dc=*/26, /* reset=*/25);
static bool u8g2_initialized = false;

// --- Message log for scrolling messages ---

/**
 * @brief Prints a message on the display, scrolling old messages up as new ones arrive.
 * @param message The message to print.
 * @param font_size The font size to use (8, 10, 12, etc.).
 * @param clear If true, clears the display before printing (default: true).
 */
// Prints a message to the display, handling newlines and scrolling.
// - message: The string to print. '\n' triggers a new line and scrolls up if needed.
// - font_size: Font size to use (8, 10, 12, 14, 18, 24).
// - clear: If true, clears the display before printing.
void facePrint(const std::string &message, uint8_t font_size, bool clear) {
  if (!u8g2_initialized) return;
  static std::string line_buffer = ""; // Buffer for the current line being built
  size_t start = 0;
  // Process the message, splitting on '\n' and scrolling as needed
  while (start < message.length()) {
    size_t newline_pos = message.find('\n', start);
    if (newline_pos == std::string::npos) {
      // No more newlines, append rest to buffer
      line_buffer += message.substr(start);
      // Update or add the current line in the log
      if (!message_log.empty()) {
        message_log.back() = line_buffer;
      } else {
        message_log.push_back(line_buffer);
      }
      break;
    } else {
      // Append up to newline, then push as a new line in the log
      line_buffer += message.substr(start, newline_pos - start);
      message_log.push_back(line_buffer);
      line_buffer.clear();
      // Scroll up: keep only the last MAX_LOG_LINES lines
      while (message_log.size() > MAX_LOG_LINES) {
        message_log.pop_front();
      }
      start = newline_pos + 1;
    }
  }
  // Select font based on font_size
  switch (font_size) {
    case 8: u8g2.setFont(u8g2_font_ncenB08_tr); break;
    case 10: u8g2.setFont(u8g2_font_ncenB10_tr); break;
    case 12: u8g2.setFont(u8g2_font_ncenB12_tr); break;
    case 14: u8g2.setFont(u8g2_font_ncenB14_tr); break;
    case 18: u8g2.setFont(u8g2_font_ncenB18_tr); break;
    case 24: u8g2.setFont(u8g2_font_ncenB24_tr); break;
    default: u8g2.setFont(u8g2_font_ncenB10_tr); break;
  }
  if (clear) u8g2.clearBuffer();
  u8g2.setDrawColor(COLOR_WHITE);
  // Calculate line height for vertical spacing
  int line_height = u8g2.getMaxCharHeight() + 2;
  int y = line_height; // Start at first line
  // Draw each message in the log, wrapping long lines
  for (const auto& msg : message_log) {
    int x = 0;
    size_t start = 0;
    size_t len = msg.length();
    while (start < len) {
      // Find how many characters fit in 128 pixels
      size_t end = start;
      int w = 0;
      while (end < len) {
        std::string sub = msg.substr(start, end - start + 1);
        w = u8g2.getUTF8Width(sub.c_str());
        if (w > SCREEN_WIDTH) break;
        end++;
      }
      if (end == start) end++; // Always print at least one character
      std::string line = msg.substr(start, end - start);
      u8g2.drawUTF8(x, y, line.c_str()); // Draw the line
      y += line_height;
      if (y > SCREEN_HEIGHT) break;
      start = end;
    }
    if (y > SCREEN_HEIGHT) break;
  }
  u8g2.sendBuffer(); // Update the display
}

void facePrint(const int number, uint8_t font_size, bool clear) {
  facePrint(std::to_string(number), font_size, clear);
}
void facePrint(const float number, uint8_t font_size, bool clear) {
  facePrint(std::to_string(number), font_size, clear);
}
void facePrint(const char message, uint8_t font_size, bool clear) {
  facePrint(std::string(1, message), font_size, clear);
}

void facePrintln(const std::string &message, uint8_t font_size, bool clear) {
  facePrint(message + '\n', font_size, clear);
}
void facePrintln(const int number, uint8_t font_size, bool clear) {
  facePrint(std::to_string(number) + '\n', font_size, clear);
}
void facePrintln(const float number, uint8_t font_size, bool clear) {
  facePrint(std::to_string(number) + '\n', font_size, clear);
}
void facePrintln(const char message, uint8_t font_size, bool clear) {
  facePrint(std::string(1, message) + '\n', font_size, clear);
}


void facePrintMiddle(const std::string &text, bool clear, uint8_t font_size) {
  if (!u8g2_initialized) return;
  // Select font based on font_size parameter
  switch (font_size) {
    case 8:
      u8g2.setFont(u8g2_font_ncenB08_tr); // 8pt font
      break;
    case 10:
      u8g2.setFont(u8g2_font_ncenB10_tr); // 10pt font
      break;
    case 12:
      u8g2.setFont(u8g2_font_ncenB12_tr); // 12pt font
      break;
    case 14:
      u8g2.setFont(u8g2_font_ncenB14_tr); // 14pt font
      break;
    case 18:
      u8g2.setFont(u8g2_font_ncenB18_tr); // 18pt font
      break;
    case 24:
      u8g2.setFont(u8g2_font_ncenB24_tr); // 24pt font
      break;
    default:
      u8g2.setFont(u8g2_font_ncenB08_tr); // Default to 8pt font
      break;
  }
  if (clear) {
    u8g2.clearBuffer();
  }
  u8g2.setDrawColor(COLOR_WHITE);
  int16_t x, y;
  uint16_t w, h;
  w = u8g2.getUTF8Width(text.c_str());
  h = u8g2.getMaxCharHeight();
  x = (SCREEN_WIDTH - w) / 2; // Center horizontally
  y = (SCREEN_HEIGHT - h) / 2 + h; // Center vertically (baseline)
  u8g2.drawUTF8(x, y, text.c_str());
  u8g2.sendBuffer(); // Update display
}

void facePrintMiddle(const int number, bool clear, uint8_t font_size) {
  facePrintMiddle(std::to_string(number), clear, font_size);
}

void facePrintMiddle(const float number, bool clear, uint8_t font_size) {
  facePrintMiddle(std::to_string(number), clear, font_size);
}

void facePrintMiddle(const char message, bool clear, uint8_t font_size) {
  facePrintMiddle(std::string(1, message), clear, font_size);
}

/**
 * @brief Clears the display buffer (does not update the screen until display_display is called).
 */
void display_clearDisplay() {
  if (!u8g2_initialized) return;
  u8g2.clearBuffer();
}
/**
 * @brief Draws a filled rounded rectangle at (x, y) with width w, height h, corner radius r, and color.
 *        Ensures valid radius and minimum size for the rectangle.
 */
void display_fillRoundRect(int x, int y, int w, int h, int r, int color) {
  if (!u8g2_initialized) return;
  u8g2.setDrawColor(color);
  // behavior is not defined if r is smaller than the height or width
  if (w < 2 * (r + 1)) {
    r = (w / 2) - 1;
  }
  if (h < 2 * (r + 1)) {
    r = (h / 2) - 1;
  }
  // check if height and width are valid when calling drawRBox
  u8g2.drawRBox(x, y, w < 1 ? 1 : w, h < 1 ? 1 : h, r);
}

/**
 * @brief Sends the buffer to the display (refreshes the screen).
 */
void display_display() {
  if (!u8g2_initialized) return;
  u8g2.sendBuffer();
}


/**
 * @brief Draws a filled triangle with the given coordinates and color.
 */
void display_fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color) {
  if (!u8g2_initialized) return;
  u8g2.setDrawColor(color);
  u8g2.drawTriangle(x0, y0, x1, y1, x2, y2);
}


/**
 * @brief Draws both eyes at their current positions and sizes; updates display if requested.
 * @param update If true, sends the buffer to the display after drawing.
 */
void draw_eyes(bool update) {
  if (!u8g2_initialized) return;
  display_clearDisplay();
  //draw from center
  int x = int(left_eye_x - left_eye_width / 2);
  int y = int(left_eye_y - left_eye_height / 2);
  display_fillRoundRect(x, y, left_eye_width, left_eye_height, corner_radius, COLOR_WHITE);
  x = int(right_eye_x - right_eye_width / 2);
  y = int(right_eye_y - right_eye_height / 2);
  display_fillRoundRect(x, y, right_eye_width, right_eye_height, corner_radius, COLOR_WHITE);
  if (update) {
    display_display();
  }
}


/**
 * @brief Resets both eyes to their default (centered) positions and sizes, then draws them.
 * @param update If true, updates the display after drawing.
 */
void eyes_reset(bool update) {
  //move eyes to the center of the display, defined by SCREEN_WIDTH, SCREEN_HEIGHT
  left_eye_height = ref_eye_height;
  left_eye_width = ref_eye_width;
  right_eye_height = ref_eye_height;
  right_eye_width = ref_eye_width;

  left_eye_x = SCREEN_WIDTH / 2 - ref_eye_width / 2 - ref_space_between_eye / 2;
  left_eye_y = SCREEN_HEIGHT / 2;
  right_eye_x = SCREEN_WIDTH / 2 + ref_eye_width / 2 + ref_space_between_eye / 2;
  right_eye_y = SCREEN_HEIGHT / 2;

  corner_radius = ref_corner_radius;

  draw_eyes(update);
}

/**
 * @brief Performs a blink animation by shrinking and restoring the eyes.
 * @param speed The speed of the blink (default: 12).
 */
void eyes_blink(int speed) {
  eyes_reset(false);

  draw_eyes(true);


  for (int i = 0; i < 3; i++) {
    left_eye_height = left_eye_height - speed;
    right_eye_height = right_eye_height - speed;

    left_eye_width = left_eye_width + 3;
    right_eye_width = right_eye_width + 3;
  draw_eyes(true);
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_height = left_eye_height + speed;
    right_eye_height = right_eye_height + speed;
    left_eye_width = left_eye_width - 3;
    right_eye_width = right_eye_width - 3;
  draw_eyes(true);
    delay(1);
  }
}



/**
 * @brief Sets the eyes to a "sleeping" state (thin horizontal lines).
 */
void eyes_sleep() {
  eyes_reset(false);

  left_eye_height = 2;
  left_eye_width = ref_eye_width;
  right_eye_height = 2;
  right_eye_width = ref_eye_width;


  corner_radius = 0;
  draw_eyes(true);
}
/**
 * @brief Wakes up the eyes with an opening animation.
 */
void eyes_wakeup() {
  eyes_reset(false);

  eyes_sleep();

  for (int h = 2; h <= ref_eye_height; h += 2) {
    left_eye_height = h;
    right_eye_height = h;
    corner_radius = h;
    if (corner_radius > ref_corner_radius) {
      corner_radius = ref_corner_radius;
    }
    draw_eyes(true);
  }
}


/**
 * @brief Draws a "happy" eye expression by overlaying triangles on the lower part of the eyes.
 */
void eyes_happy() {
  eyes_reset(false);
  //draw inverted triangle over eye lower part
  int offset = ref_eye_height / 2;
  for (int i = 0; i < 10; i++) {
    display_fillTriangle(left_eye_x - left_eye_width / 2 - 1, left_eye_y + offset, left_eye_x + left_eye_width / 2 + 1, left_eye_y + 5 + offset, left_eye_x - left_eye_width / 2 - 1, left_eye_y + left_eye_height + offset, COLOR_BLACK);
    display_fillTriangle(right_eye_x + right_eye_width / 2 + 1, right_eye_y + offset, right_eye_x - left_eye_width / 2 - 2, right_eye_y + 5 + offset, right_eye_x + right_eye_width / 2 + 1, right_eye_y + right_eye_height + offset, COLOR_BLACK);
    offset -= 2;
    display_display();
    delay(1);
  }


  display_display();
  delay(1000);
}

/**
 * @brief Performs a saccade (quick eye movement) in the specified direction.
 * @param direction_x Horizontal direction: -1 (left), 1 (right), 0 (none).
 * @param direction_y Vertical direction: -1 (up), 1 (down), 0 (none).
 */
void eyes_saccade(int direction_x, int direction_y) {
  //quick movement of the eye, no size change. stay at position after movement, will not move back,  call again with opposite direction
  //direction == -1 :  move left
  //direction == 1 :  move right

  int direction_x_movement_amplitude = 8;
  int direction_y_movement_amplitude = 6;
  int blink_amplitude = 8;

  for (int i = 0; i < 1; i++) {
    left_eye_x += direction_x_movement_amplitude * direction_x;
    right_eye_x += direction_x_movement_amplitude * direction_x;
    left_eye_y += direction_y_movement_amplitude * direction_y;
    right_eye_y += direction_y_movement_amplitude * direction_y;

    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
  draw_eyes(true);
    delay(1);
  }

  for (int i = 0; i < 1; i++) {
    left_eye_x += direction_x_movement_amplitude * direction_x;
    right_eye_x += direction_x_movement_amplitude * direction_x;
    left_eye_y += direction_y_movement_amplitude * direction_y;
    right_eye_y += direction_y_movement_amplitude * direction_y;

    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;

  draw_eyes(true);
    delay(1);
  }
}

/**
 * @brief Moves the eyes in a big movement to the right.
 */
void eyes_move_right_big() {
  eyes_move_big(1);
}
/**
 * @brief Moves the eyes in a big movement to the left.
 */
void eyes_move_left_big() {
  eyes_move_big(-1);
}
/**
 * @brief Moves the eyes in a big movement in the specified direction, with size and blink animation.
 * @param direction -1 for left, 1 for right.
 */
void eyes_move_big(int direction) {
  //direction == -1 :  move left
  //direction == 1 :  move right

  int direction_oversize = 1;
  int direction_movement_amplitude = 2;
  int blink_amplitude = 5;

  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude * direction;
    right_eye_x += direction_movement_amplitude * direction;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    if (direction > 0) {
      right_eye_height += direction_oversize;
      right_eye_width += direction_oversize;
    } else {
      left_eye_height += direction_oversize;
      left_eye_width += direction_oversize;
    }

  draw_eyes(true);
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x += direction_movement_amplitude * direction;
    right_eye_x += direction_movement_amplitude * direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    if (direction > 0) {
      right_eye_height += direction_oversize;
      right_eye_width += direction_oversize;
    } else {
      left_eye_height += direction_oversize;
      left_eye_width += direction_oversize;
    }
  draw_eyes(true);
    delay(1);
  }

  delay(1000);

  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude * direction;
    right_eye_x -= direction_movement_amplitude * direction;
    right_eye_height -= blink_amplitude;
    left_eye_height -= blink_amplitude;
    if (direction > 0) {
      right_eye_height -= direction_oversize;
      right_eye_width -= direction_oversize;
    } else {
      left_eye_height -= direction_oversize;
      left_eye_width -= direction_oversize;
    }
  draw_eyes(true);
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye_x -= direction_movement_amplitude * direction;
    right_eye_x -= direction_movement_amplitude * direction;
    right_eye_height += blink_amplitude;
    left_eye_height += blink_amplitude;
    if (direction > 0) {
      right_eye_height -= direction_oversize;
      right_eye_width -= direction_oversize;
    } else {
      left_eye_height -= direction_oversize;
      left_eye_width -= direction_oversize;
    }
  draw_eyes(true);
    delay(1);
  }
  eyes_reset(true);
}




/**
 * @brief Launches a facial animation based on the given index.
 * @param animation_index Index of the animation to launch.
 */
void launch_animation_with_index(int animation_index) {



  if (animation_index > max_animation_index) {
    animation_index = 8;
  }

  switch (animation_index) {
    case 0:
      eyes_wakeup();
      break;
    case 1:
      eyes_reset(true);
      break;
    case 2:
      eyes_move_right_big();
      break;
    case 3:
      eyes_move_left_big();
      break;
    case 4:
      eyes_blink(12);
      delay(1000);
      break;
    case 5:
      eyes_blink(12);
      break;
    case 6:
      eyes_happy();
      break;
    case 7:
      eyes_sleep();
      break;
    case 8:
      break;
      eyes_reset(true);
      for (int i = 0; i < 20; i++) {
        int dir_x = random(-1, 2);
        int dir_y = random(-1, 2);
        eyes_saccade(dir_x, dir_y);
        delay(1);
        eyes_saccade(-dir_x, -dir_y);
        delay(1);
      }
      break;
  }
}



/**
 * @brief Demo/test function for face emoji animations. Cycles animations in demo mode or responds to serial commands.
 */
void testFaceEmoji() {

  if (demo_mode == 1) {
    // cycle animations
    launch_animation_with_index(current_animation_index++);
    if (current_animation_index > max_animation_index) {
      current_animation_index = 0;
    }
  }


  //send "Ax" for animation x  (ex. A2 will launch animation 2)
  if (Serial.available()) {
    String data = Serial.readString();
    data.trim();
    char cmd = data[0];

    if (cmd == 'A') {
      demo_mode = 0;

      String arg = data.substring(1, data.length());
      int anim = arg.toInt();
      launch_animation_with_index(anim);
      Serial.print(cmd);
      Serial.print(arg);
    }
  }
}

/**
 * @brief FreeRTOS task handle for running face emoji animations and handling serial commands.
 */
TaskHandle_t FaceEmojiTask_Handle;

/**
 * @brief FreeRTOS task for running face emoji animations and handling serial commands.
 */
void FaceEmojiTask(void *parameter) {
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());
  for (;;) {

    if (demo_mode == 1) {
      // cycle animations
      launch_animation_with_index(current_animation_index++);
      if (current_animation_index > max_animation_index) {
        current_animation_index = 0;
      }
    }


    //send "Ax" for animation x  (ex. A2 will launch animation 2)
    if (Serial.available()) {
      String data = Serial.readString();
      data.trim();
      char cmd = data[0];

      if (cmd == 'A') {
        demo_mode = 0;

        String arg = data.substring(1, data.length());
        int anim = arg.toInt();
        launch_animation_with_index(anim);
        Serial.print(cmd);
        Serial.print(arg);
      }
    }
  }
}

/**
 * @brief Initializes the face emoji system, display, and starts the animation task.
 */
void initialize_face() {
  //initialize the u8g2 lib.
  u8g2.begin();
  u8g2_initialized = true;

  //clear screen and display startup info.
  display_clearDisplay();
  eyes_sleep();
  u8g2.setFont(u8g2_font_ncenB10_tr);
  u8g2.drawStr(0, 10, "CHIKO");

  display_display();

  // Create FaceEmojiTask on core 1
  // xTaskCreatePinnedToCore(
  //   FaceEmojiTask, "FaceEmojiTask", 10000, NULL, 1, &FaceEmojiTask_Handle, 1);
}

