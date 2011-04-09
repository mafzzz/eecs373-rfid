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
#include "app/app.h"

#include "sys/FreeRTOS.h"
#include "sys/queue.h"

/* Current screen mode */
screen_mode_t screen_current_mode;

/* Status indicator */
char status_text[20];


/* Screen cursors */
screen_curs_t _screen_cursors[SCREEN_MODE_COUNT];
#define SCREEN_CURSOR _screen_cursors[screen_current_mode]

#define SCREEN_CURSOR_X			(SCREEN_CURSOR.x / 6)
#define SCREEN_CURSOR_Y			((SCREEN_TOP - SCREEN_CURSOR)/8)
#define SCREEN_CURSOR_SET(X,Y) {SCREEN_CURSOR.x = X*6; SCREEN_CURSOR.y = SCREEN_TOP - Y*8; );
#define SCREEN_CURSOR_UPDATE() disp_move_cursor(SCREEN_CURSOR.x, SCREEN_CURSOR.y)


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
	// Move from startup to idle mode
	screen_current_mode = SCREEN_MODE_IDLE;
	screen_redraw();
	for(;;) {
		int screen_changed = 1;
		// TODO: Check for input from keypad
		// Redraw the screen based on current mode if it needs redraw
		if(xQueueReceive(g_screen_status_queue, &status_text, 0)) {
			//Status changed, redraw
			screen_redraw();
		}

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
		SCREEN_CURSOR.y = SCREEN_TOP;
	}

	status_text[0] = '\0';

	screen_current_mode = SCREEN_MODE_STARTUP;
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

	/* Draw the status text */

	disp_move_cursor(SCREEN_LEFT, SCREEN_TOP);
	disp_printf("Status: %s", status_text);
	SCREEN_CURSOR_UPDATE();

	/* Based on the current mode, call the appropriate function	*/
	screen_funcs[screen_current_mode]();
}

/* SCREEN MODE FUNCTIONALITY */
void screen_startup()
{
	disp_printf("Startup mode");
}
void screen_idle()
{
	disp_printf("Idle mode");
}

void screen_inventory()
{
	disp_printf("Inventory mode");
}

void screen_recipe()
{
	disp_printf("Recipe mode");
}

void screen_nutrition()
{
	disp_printf("Nutrition mode");
}
