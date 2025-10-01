#pragma once
#include <avr/io.h>
#include "consts.h"

void oled_init();

void oled_reset();

void oled_home();

void oled_goto_line(uint8_t line);

void oled_clear_line(uint8_t line);

void oled_set_pos(uint8_t line, uint8_t col);

void oled_write_data(const char c); //something something volatile

void oled_print(const char* string);


void oled_set_brightness(uint8_t level);
