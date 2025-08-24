
/**
 * @file chiko_face.h
 * @brief Declarations and state for ChikoBot's facial animation and display routines using u8g2.
 *
 * This header defines variables and functions for controlling and animating the robot's eyes and facial expressions
 * on an OLED display, including blinking, saccades, happy/sleepy eyes, and demo/command modes.
 */

#ifndef __CHIKO_FACEEMOJI__
#define __CHIKO_FACEEMOJI__

#include <SPI.h>
#include <U8g2lib.h>

// Color definitions for display
int COLOR_WHITE = 1;
int COLOR_BLACK = 0;

// Demo mode state variables
int demo_mode = 1;                        // 1 = demo mode, 0 = command mode
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
U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/NULL, /* dc=*/26, /* reset=*/25);

/**
 * @brief Demo/test function for face emoji animations. Cycles animations in demo mode or responds to serial commands.
 */
void testFaceEmoji();

/**
 * @brief Clears the display buffer (does not update the screen until display_display is called).
 */
void display_clearDisplay();

/**
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
void reset_eyes(bool update);

/**
 * @brief Performs a blink animation by shrinking and restoring the eyes.
 * @param speed The speed of the blink.
 */
void blink(int speed);

/**
 * @brief Sets the eyes to a "sleeping" state (thin horizontal lines).
 */
void sleep();

/**
 * @brief Wakes up the eyes with an opening animation.
 */
void wakeup();

/**
 * @brief Performs a saccade (quick eye movement) in the specified direction.
 * @param direction_x Horizontal direction: -1 (left), 1 (right), 0 (none).
 * @param direction_y Vertical direction: -1 (up), 1 (down), 0 (none).
 */
void saccade(int direction_x, int direction_y);

/**
 * @brief Draws a "happy" eye expression by overlaying triangles on the lower part of the eyes.
 */
void happy_eye();

/**
 * @brief Moves the eyes in a big movement to the right.
 */
void move_right_big_eye();

/**
 * @brief Moves the eyes in a big movement to the left.
 */
void move_left_big_eye();

/**
 * @brief Moves the eyes in a big movement in the specified direction, with size and blink animation.
 * @param direction -1 for left, 1 for right.
 */
void move_big_eye(int direction);

/**
 * @brief Launches a facial animation based on the given index.
 * @param animation_index Index of the animation to launch.
 */
void launch_animation_with_index(int animation_index);

/**
 * @brief Initializes the face emoji system, display, and starts the animation task.
 */
void initFaceEmoji();

#endif
