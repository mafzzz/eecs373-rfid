/*
 * screen.c
 *
 *  Created on: Mar 21, 2011
 *      Author: Zach
 */

#include "sys/linux_list.h"
#include "device/disp.h"
#include "app/screen.h"
#include "app/product.h"
#include "app/inventory.h"

/* Current screen mode */
screen_mode_t screen_current_mode;



/* Screen cursors */
screen_curs_t _screen_cursors[SCREEN_MODE_COUNT];
#define SCREEN_CURSOR _screen_cursors[screen_current_mode]

/* Screen mode functions */
void screen_idle();
void screen_inventory();
void screen_nutrition();
void screen_recipe();

void (*screen_funcs[]) (void) = {
		screen_idle,
		screen_inventory,
		screen_nutrition,
		screen_recipe
};


/* Screen thread loop */
void screen_task()
{
	for(;;) {
		int screen_changed = 0;
		// TODO: Check for input from keypad
		// Redraw the screen based on current mode if it needs redraw

	}
}

void screen_init()
{
	int i;
	disp_init();
	disp_clear();

	/* Set up cursors for each mode */
	for(i = 0; i < SCREEN_MODE_COUNT; i++) {
		SCREEN_CURSOR.x = 0;
		SCREEN_CURSOR.y = 0;
	}

	screen_current_mode = SCREEN_MODE_IDLE;
	screen_redraw();
}

void screen_mode(screen_mode_t new_mode)
{
	screen_current_mode = new_mode;
}

/* Redraws the screen based on the current mode */
void screen_redraw()
{
	/* Clear the display */
	disp_clear();

	/* Based on the current mode, call the appropriate function	*/
	screen_funcs[screen_current_mode]();
}

/* SCREEN MODE FUNCTIONALITY */
void screen_idle()
{

}

void screen_inventory()
{
	product_t *cur_product;

	list_for_each_entry(cur_product, &g_inventory_contents, product_t, inventory_list) {
		disp_printf("%s ", cur_product->name);
		SCREEN_CURSOR.y += 5;
		//disp_move_cursor(cursor->x, cursor->y);
	}
}

void screen_recipe()
{

}

void screen_nutrition()
{

}
