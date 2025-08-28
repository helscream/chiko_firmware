#ifndef __CHIKO_FACEEMOJI__
#define __CHIKO_FACEEMOJI__

#include <SPI.h>
#include <U8g2lib.h>
#include <string>





/**
 * @brief Demo/test function for face emoji animations. Cycles animations in demo mode or responds to serial commands.
 */
void testFaceEmoji();

/**
 * @brief Clears the display buffer (does not update the screen until display_display is called).
 */
void display_clearDisplay();


/**
 * @brief Demo/test function for face emoji animations. Cycles animations in demo mode or responds to serial commands.
 *        Call this in your main loop for demo/command mode.
 *
 * @brief Draws a filled rounded rectangle at (x, y) with width w, height h, corner radius r, and color.
 */
void display_fillRoundRect();

/**
 * @brief Sends the buffer to the display (refreshes the screen).
 */
void display_display();

/**
 * @brief Draws a filled triangle with the given coordinates and color.
 */
void display_fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, int color);

/**
 * @brief Draws both eyes at their current positions and sizes; updates display if requested.
 * @param update If true, sends the buffer to the display after drawing.
 */
void draw_eyes(bool update);

/**
 * @brief Resets both eyes to their default (centered) positions and sizes, then draws them.
 * @param update If true, updates the display after drawing.
 */
void eyes_reset(bool update);

/**
 * @brief Performs a blink animation by shrinking and restoring the eyes.
 * @param speed The speed of the blink.
 */
void eyes_blink(int speed);

/**
 * @brief Sets the eyes to a "sleeping" state (thin horizontal lines).
 */
void eyes_sleep();

/**
 * @brief Wakes up the eyes with an opening animation.
 */
void eyes_wakeup();

/**
 * @brief Performs a saccade (quick eye movement) in the specified direction.
 * @param direction_x Horizontal direction: -1 (left), 1 (right), 0 (none).
 * @param direction_y Vertical direction: -1 (up), 1 (down), 0 (none).
 */
void eyes_saccade(int direction_x, int direction_y);

/**
 * @brief Draws a "happy" eye expression by overlaying triangles on the lower part of the eyes.
 */
void eyes_happy();

/**
 * @brief Moves the eyes in a big movement to the right.
 */
void eyes_move_right_big();

/**
 * @brief Moves the eyes in a big movement to the left.
 */
void eyes_move_left_big();

/**
 * @brief Moves the eyes in a big movement in the specified direction, with size and blink animation.
 * @param direction -1 for left, 1 for right.
 */
void eyes_move_big(int direction);

/**
 * @brief Launches a facial animation based on the given index.
 * @param animation_index Index of the animation to launch.
 */
void launch_animation_with_index(int animation_index);

/**
 * @brief Initializes the face emoji system, display, and starts the animation task.
 */
void initialize_face();


/**
 * @brief Print text centered on the display.
 * @param text The string to print
 * @param clear If true, clears the display before printing
 * @param font_size Font size (8, 10, 12, 14, 18, 24)
 */
void facePrintMiddle(const std::string &text, bool clear = true, uint8_t font_size = 10);
/**
 * @brief Print an integer centered on the display.
 * @param number The integer to print
 * @param clear If true, clears the display before printing
 * @param font_size Font size (8, 10, 12, 14, 18, 24)
 */
void facePrintMiddle(const int number, bool clear = true, uint8_t font_size = 10);
/**
 * @brief Print a float centered on the display.
 * @param number The float to print
 * @param clear If true, clears the display before printing
 * @param font_size Font size (8, 10, 12, 14, 18, 24)
 */
void facePrintMiddle(const float number, bool clear = true, uint8_t font_size = 10);
/**
 * @brief Print a single character centered on the display.
 * @param message The character to print
 * @param clear If true, clears the display before printing
 * @param font_size Font size (8, 10, 12, 14, 18, 24)
 */
void facePrintMiddle(const char message, bool clear = true, uint8_t font_size = 10);

/**
 * @brief Print text to the display log, scrolling up as new lines are added. '\n' triggers a new line.
 * @param message The string to print (can include '\n')
 * @param font_size Font size (8, 10, 12, 14, 18, 24)
 * @param clear If true, clears the display before printing
 */
void facePrint(const std::string &message, uint8_t font_size = 8, bool clear = true);
/**
 * @brief Print an integer to the display log, scrolling up as new lines are added.
 * @param number The integer to print
 * @param font_size Font size (8, 10, 12, 14, 18, 24)
 * @param clear If true, clears the display before printing
 */
void facePrint(const int number, uint8_t font_size = 8, bool clear = true);
/**
 * @brief Print a float to the display log, scrolling up as new lines are added.
 * @param number The float to print
 * @param font_size Font size (8, 10, 12, 14, 18, 24)
 * @param clear If true, clears the display before printing
 */
void facePrint(const float number, uint8_t font_size = 8, bool clear = true);
/**
 * @brief Print a single character to the display log, scrolling up as new lines are added.
 * @param message The character to print
 * @param font_size Font size (8, 10, 12, 14, 18, 24)
 * @param clear If true, clears the display before printing
 */
void facePrint(const char message, uint8_t font_size = 8, bool clear = true);
// void facePrint(string message);

#endif
