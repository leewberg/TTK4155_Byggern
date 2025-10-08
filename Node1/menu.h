#pragma once
#include "input.h"
#include "oled.h"
#include <avr/io.h>

typedef struct {
	char *name;
	// submenu?
} MenuItem;

typedef struct {
	MenuItem *items;
	uint8_t item_count;
	uint8_t current_index;
	uint8_t move_cooldown; // to avoid moving too fast: have return to neutral
						   // before a move
} Menu;

Menu *menu_init();

int menu_update(
	Menu *menu,
	volatile INPUT_DATA *data); // updates both display. return -1 if none
								// pressed, and index of item if pressed
