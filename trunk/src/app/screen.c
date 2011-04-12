/*
 * screen.c
 *
 *  Created on: Mar 21, 2011
 *      Author: Zach
 */

#include <stdio.h>

#include "sys/linux_list.h"
#include "device/disp.h"
#include "app/screen.h"
#include "app/product.h"
#include "app/inventory.h"
#include "app/app.h"

#include "sys/FreeRTOS.h"
#include "sys/queue.h"
#include "sys/task.h"

/* Current screen mode */
screen_mode_t screen_current_mode;

/* Debug status (from control) */
char status_text[20];
int got_status = 0;

/* Current temperature */
int cur_temp = -1;
int got_cur_temp = 0;

int keypad_val;
char barcode_val[11];
int got_barcode_val = 0;


/* Screen cursors */
screen_curs_t _screen_cursors[SCREEN_MODE_COUNT];
#define SCREEN_CURSOR _screen_cursors[screen_current_mode]

#define SCREEN_CURSOR_X			(SCREEN_CURSOR.x)
#define SCREEN_CURSOR_Y			(SCREEN_CURSOR.y)
#define SCREEN_CURSOR_SET(X,Y) {SCREEN_CURSOR.x = X; SCREEN_CURSOR.y = Y; SCREEN_CURSOR_UPDATE();}
#define SCREEN_CURSOR_NEWLINE()	{ SCREEN_CURSOR.y -= SCREEN_CHAR_HEIGHT; SCREEN_CURSOR_UPDATE(); }
#define SCREEN_NEWLINE()		SCREEN_CURSOR_NEWLINE()
#define SCREEN_CURSOR_UPDATE() disp_move_cursor(SCREEN_CURSOR.x, SCREEN_CURSOR.y)


/* Screen mode functions */
void screen_startup();
void screen_idle();
void screen_inventory();
void screen_nutrition();
void screen_recipe();

void (*screen_funcs[]) (void) = {
		screen_startup,
		screen_idle,
		screen_inventory,
		screen_nutrition,
		screen_recipe
};

char *screen_names[] = {
		"Startup",
		"Stat",
		"Inv",
		"Nut",
		"Rec"
};


/* Screen thread loop */
void screen_task()
{

	// Move from startup to idle mode
	printf("SCREEN: Starting, going to idle task\n\r");
	screen_mode(SCREEN_MODE_IDLE);
	screen_redraw(1,1,1);
	for(;;) {
		barcode_msg_t barcodeMsg;
		status_msg_t statusMsg;

		printf("SCREEN: Start loop\n\r");
		//int screen_changed = 1;
		// TODO: Check for input from keypad

		// Check the debug status queue to see if we should update
		printf("SCREEN: Checking screen queue\n\r");
		if(xQueueReceive(g_screen_status_queue, &statusMsg, 0)) {
			strcpy(&status_text, statusMsg.data);
			//Status changed, redraw
			screen_redraw(1,0,0);
		}

		// Check the sensor queue to see if we should update
		if(xQueueReceive(g_sensors_queue, &cur_temp, 0)) {
			screen_redraw(0,1,0);
		}

		// Check the keypad queue to see if we should update
		if(xQueueReceive(g_keypad_queue, &keypad_val, 0)) {
			screen_redraw(0,0,1);
		}

		// Check the barcode queue to see if we should update
		if(xQueueReceive(g_barcode_queue, &barcodeMsg, 0)) {
			memcpy(&barcode_val, barcodeMsg.data, 11);
			screen_redraw(0,1,0);
		}

		vTaskDelay(portTICK_RATE_MS * 500);

	}
}

void screen_init()
{
	int i;
	disp_init();
	disp_clear();

	/* Set up cursors for each mode */
	for(i = 0; i < SCREEN_MODE_COUNT; i++) {
		_screen_cursors[i].x = 0;
		_screen_cursors[i].y = SCREEN_TOP;
	}

	status_text[0] = '\0';

	screen_current_mode = SCREEN_MODE_STARTUP;
	screen_redraw(1,1,1);
}

void screen_mode(screen_mode_t new_mode)
{
	screen_current_mode = new_mode;
}

/* Redraws the screen based on the current mode */
void screen_redraw(int re_dbg, int re_mode, int re_menu)
{
	int i;

	/* Draw the status text */
	if(re_dbg) {
		disp_clear_rect(DISP_LEFT, DISP_TOP-7, DISP_RIGHT, DISP_TOP);
		disp_move_cursor(DISP_LEFT, DISP_TOP);
		disp_printf("Smart Fridge - Temp: %iF", cur_temp);
		disp_line(DISP_LEFT, DISP_TOP-7, DISP_RIGHT, DISP_TOP-7);
	}


	if(re_mode) {
		disp_clear_rect(SCREEN_LEFT, SCREEN_BOTTOM, SCREEN_RIGHT, SCREEN_TOP);
		SCREEN_CURSOR_UPDATE(); //Put the display cursor back to the screen cursor
		/* Based on the current mode, call the appropriate function	*/
		screen_funcs[screen_current_mode]();
	}

	if(re_menu) {
		disp_clear_rect(DISP_LEFT, DISP_BOTTOM, DISP_RIGHT, DISP_BOTTOM+10);
		/* Draw the menu bar */
		disp_line(DISP_LEFT, DISP_BOTTOM+9, DISP_RIGHT, DISP_BOTTOM+9);
		/* Draw each mode option*/
		disp_move_cursor(DISP_LEFT, DISP_BOTTOM+SCREEN_CHAR_HEIGHT-2);
		disp_printf("Menu: ");
		for(i = SCREEN_MODE_IDLE; i < SCREEN_MODE_COUNT; i++) {
			if(i == screen_current_mode) {
				disp_printf("*");
			}
			disp_printf("%s",screen_names[i]);
			if(i < SCREEN_MODE_COUNT-1) {
				disp_printf("  ");
			}
		}
	}
}

/* SCREEN MODE FUNCTIONALITY */
void screen_startup()
{
	disp_printf("Startup mode");
}
void screen_idle()
{
	/* Reset state cursor for redraw (don't need to keep cursor in this mode */
	SCREEN_CURSOR_SET(SCREEN_LEFT, SCREEN_TOP);

	disp_printf("Status:"); SCREEN_NEWLINE();
	disp_printf("%s", status_text); SCREEN_NEWLINE();
	disp_printf("Quick View: "); SCREEN_NEWLINE();
	disp_printf(" Last barcode: %s", barcode_val); SCREEN_NEWLINE();
	disp_printf(" Last RFID: N/A"); SCREEN_NEWLINE();
	disp_printf("Stats: ");SCREEN_NEWLINE();
	disp_printf(" Items in stock: X"); SCREEN_NEWLINE();
	disp_printf(" Items ever in stock: X"); SCREEN_NEWLINE();
	disp_printf(" Total RFID scans: X"); SCREEN_NEWLINE();
	disp_printf(" Total barcode scans: X"); SCREEN_NEWLINE();

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
