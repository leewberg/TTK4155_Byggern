/* 
todo: Moving this to CPP looks like it takes more Flash storage. Figure out why.
*/
// #include "SSD1306.h"
#pragma once
#include <stdlib.h>
#include "constants.h"
#include "time.h"
#include "doom_game/sprites.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Reads a char from an F() string
// #define F_char(ifsh, ch)    pgm_read_byte(reinterpret_cast<PGM_P>(ifsh) + ch)
#define F_char(ifsh, ch)    reinterpret_cast<PGM_P>(ifsh)[ch]

// This is slightly faster than bitRead (also bits are read from left to right)
const static uint8_t bit_mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
#define read_bit(b, n)      b & bit_mask[n] ? 1 : 0

void setupDisplay();
void fps();
double getActualFps();
bool getGradientPixel(uint8_t x, uint8_t y, uint8_t i);
void fadeScreen(uint8_t intensity, bool color);
void drawByte(uint8_t x, uint8_t y, uint8_t b);
// uint8_t getByte(uint8_t x, uint8_t y);
void drawPixel(int8_t x, int8_t y, bool color, bool raycasterViewport);
void drawVLine(uint8_t x, int8_t start_y, int8_t end_y, uint8_t intensity);
void drawSprite(int8_t x, int8_t y, const uint8_t bitmap[], const uint8_t mask[], int16_t w, int16_t h, uint8_t sprite, double distance, bool only_add);
void drawChar(int8_t x, int8_t y, char ch);
void drawText(int8_t x, int8_t y, char *txt, uint8_t space);
void drawInt(uint8_t x, uint8_t y, uint8_t num);
void invertDisplay(bool invert);
void clear_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

// void drawText(int8_t x, int8_t y, const __FlashStringHelper txt, uint8_t space = 1);

// Initialize screen. Following line is for OLED 128x64 connected by I2C
// Adafruit_SSD1306<SCREEN_WIDTH, SCREEN_HEIGHT> display;

// FPS control
extern double delta;
extern uint32_t lastFrameTime;

extern uint8_t *display_buf;

// We don't handle more than MAX_RENDER_DEPTH depth, so we can safety store
// z values in a byte with 1 decimal and save some memory,
extern uint8_t zbuffer[ZBUFFER_SIZE];
