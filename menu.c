#include "menu.h"
#include "oled.h"
#include "uart.h"
#include <stdlib.h>

Menu *menu_init() {
	Menu *menu = malloc(sizeof(Menu));
	MenuItem *items = malloc(sizeof(MenuItem) * 4);

	items[0].name = "New Game";
	items[1].name = "Options";
	items[2].name = "Credits";
	items[3].name = "Exit";

	menu->items = items;
	menu->item_count = 4;
	menu->current_index = 0;
	menu->move_cooldown = 0;
	return menu;
}

int menu_update(Menu *menu, volatile INPUT_DATA *data) {
	// check buttons. might remap later to that adc processing
	if ((!menu->move_cooldown) && (data->joy_pos.y > 200)) {
		if (menu->current_index > 0) {
			menu->current_index--;
		}
		menu->move_cooldown = 1;
	} else if ((!menu->move_cooldown) && (data->joy_pos.y < 80)) {
		if (menu->current_index < menu->item_count - 1) {
			menu->current_index++;
			menu->move_cooldown = 1;
		}
	} else if (data->joy_pos.y > 100 && data->joy_pos.y < 135) {
		menu->move_cooldown = 0;
	}

	// do display shit
	oled_home();
	for (int i = 0; i < menu->item_count; i++) {
		oled_set_pos(i, 0);
		if (i == menu->current_index) {
			oled_print("> ");
		} else {
			oled_print("  ");
		}
		oled_print(menu->items[i].name);
	}

	if (data->joy_button & 0b1) {
		return menu->current_index;
	}

	return -1;
}
