/*
 * screen.c
 *
 *  Created on: Mar 21, 2011
 *      Author: Zach
 */

#include <stdio.h>

#include "sys/linux_list.h"
#include "device/disp.h"
#include "device/rfid.h"
#include "device/keypad.h"
#include "app/screen.h"
#include "app/product.h"
#include "app/inventory.h"
#include "app/app.h"

#include "sys/FreeRTOS.h"
#include "sys/queue.h"
#include "sys/task.h"

/* Current screen mode */
static screen_mode_t screen_current_mode;

/* Debug status (from control) */
char status_text[20];
int got_status = 0;

/* Current temperature */
int cur_temp = -1;
int got_cur_temp = 0;

keypad_t keypad_val;
barcode_t barcode;
static char last_barcode[12];
static char last_rfid[17];
static int rfid_count = 0;
static int barcode_count = 0;
rfid_t rfid;
char rfid_val[RFID_LEN];
int got_barcode_val = 0;


/* Screen cursors */
screen_curs_t _screen_cursors[1/*SCREEN_MODE_COUNT*/];
//#define SCREEN_CURSOR _screen_cursors[screen_current_mode]
#define SCREEN_CURSOR _screen_cursors[0]

#define SCREEN_CURSOR_X			(SCREEN_CURSOR.x)
#define SCREEN_CURSOR_Y			(SCREEN_CURSOR.y)
#define SCREEN_CURSOR_SET(X,Y) {SCREEN_CURSOR.x = X; SCREEN_CURSOR.y = Y; SCREEN_CURSOR_UPDATE();}
#define SCREEN_SET_CURSOR(X,Y) SCREEN_CURSOR_SET(X,Y)
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
		"All",
		"Rec"
};


/* Screen thread loop */
void screen_task()
{

	// Move from startup to idle mode
	printf("SCREEN: Starting, going to idle task\n\r");
	screen_current_mode = SCREEN_MODE_INVENTORY;
	screen_redraw(1,1,1);
	for(;;) {
		barcode_t barcodeMsg;
		status_t statusMsg;
		rfid_t rfidMsg;

		printf("SCREEN: Start loop\n\r");
		//int screen_changed = 1;
		// TODO: Check for input from keypad

		// Check the debug status queue to see if we should update
		printf("SCREEN: Checking screen queue\n\r");
		if(xQueueReceive(g_screen_status_queue, &statusMsg, 0)) {
			strncpy(status_text, statusMsg.data, STATUS_MSG_LEN);
			//Status changed, draw
			screen_redraw(1,1,0);
		}

		// Check the sensor queue to see if we should update
		if(xQueueReceive(g_sensors_queue, &cur_temp, 0)) {
			screen_redraw(1,0,0);
		}

		// Check the rfid queue to see if we should update
		if(xQueuePeek(g_rfid_queue, &rfid, 0)) {
			strncpy(last_rfid, rfid.data, RFID_MSG_LEN);
			if(screen_current_mode == SCREEN_MODE_IDLE) {
				screen_redraw(0,1,0);
			}
		}

		// Check the barcode queue to see if we should update
		if(xQueuePeek(g_barcode_queue, &barcode, 0)) {
			strncpy(last_barcode, barcode.data, BARCODE_MSG_LEN);
			barcode_count++;
			if(screen_current_mode == SCREEN_MODE_IDLE) {
				screen_redraw(0,1,0);
			}
		}

		// Check the keypad queue to see if we should update
		if(xQueueReceive(g_keypad_queue, &keypad_val, 0)) {
			if(keypad_val.keys.key_a) {
				screen_current_mode = SCREEN_MODE_IDLE;
			} else if(keypad_val.keys.key_b) {
				screen_current_mode = SCREEN_MODE_INVENTORY;
			} else if(keypad_val.keys.key_c) {
				screen_current_mode = SCREEN_MODE_NUTRITION;
			} else if(keypad_val.keys.key_d) {
				screen_current_mode = SCREEN_MODE_RECIPE;
			}
			screen_redraw(0,1,1);
		}
	}
}

void screen_init()
{
	int i;
	disp_init();

	/* Set up cursors for each mode */
	for(i = 0; i < SCREEN_MODE_COUNT; i++) {
		_screen_cursors[i].x = 0;
		_screen_cursors[i].y = SCREEN_TOP;
	}

	status_text[0] = '\0';
}

/* Redraws the screen based on the current mode */
void screen_redraw(int re_dbg, int re_mode, int re_menu)
{
	int i;
	/* Draw the status text */
	//re_dbg = re_mode = re_menu = 1;
	if(re_dbg) {
		disp_clear_rect(DISP_LEFT, SCREEN_TOP+1, DISP_RIGHT, DISP_TOP);
		SCREEN_SET_CURSOR(DISP_LEFT, DISP_TOP);
		disp_printf(" RFID Fridge    Temp: %iF", cur_temp); SCREEN_NEWLINE();
		disp_printf("S: %.23s", status_text); SCREEN_NEWLINE();
		disp_line(DISP_LEFT, SCREEN_TOP+1, DISP_RIGHT, SCREEN_TOP+1);
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
		disp_line(DISP_LEFT, DISP_BOTTOM+11, DISP_RIGHT, DISP_BOTTOM+11);
		/* Draw each mode option*/
		SCREEN_SET_CURSOR(DISP_LEFT, DISP_BOTTOM+SCREEN_CHAR_HEIGHT);
		disp_printf("Menu: ");
		for(i = SCREEN_MODE_IDLE; i < SCREEN_MODE_COUNT; i++) {
			if(i == screen_current_mode) {
				disp_printf("*");
			}
			disp_printf("%.4s",screen_names[i]);
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

	SCREEN_NEWLINE();
	disp_printf("Quick View: "); SCREEN_NEWLINE();
	disp_printf(" Last barcode: %.11s", last_barcode); SCREEN_NEWLINE();
	disp_printf(" Last RFID:"); SCREEN_NEWLINE();
	disp_printf("       %.16s", last_rfid); SCREEN_NEWLINE();
	SCREEN_NEWLINE();
	disp_printf("Stats: ");SCREEN_NEWLINE();
	disp_printf(" Items (stocked): %i", inventory_stocked_count()); SCREEN_NEWLINE();
	disp_printf(" Items (all): %i", inventory_all_count()); SCREEN_NEWLINE();
	//disp_printf(" Total RFID scans: %i", rfid_count); SCREEN_NEWLINE();
	//disp_printf(" Total barcode scans: %i", barcode_count); SCREEN_NEWLINE();
}

void screen_inventory()
{
	int stocked_num = 0;
	product_t *s, *tmp;
	SCREEN_CURSOR_SET(SCREEN_LEFT, SCREEN_TOP);
	stocked_num = inventory_stocked_count();
	SCREEN_NEWLINE();
	if(stocked_num == 0) {
		disp_printf("No products in fridge"); SCREEN_NEWLINE();
	} else {
		disp_printf("%i product(s) in fridge", inventory_stocked_count()); SCREEN_NEWLINE();
	}
	LOCK_INVENTORY();
	HASH_ITER(hh_stocked, inventory_products_stocked, s, tmp) {
		if(s != NULL) {
			char name[50];
			product_get_name(&s->upc, name);
			disp_printf(" %.20s", name); SCREEN_NEWLINE();
		}
	}

	UNLOCK_INVENTORY();
}

void screen_recipe()
{
	disp_printf("Recipe mode"); SCREEN_NEWLINE();SCREEN_NEWLINE();
	disp_printf(" COMING SOON TO ");SCREEN_NEWLINE();
	disp_printf("A FRIDGE NEAR YOU");SCREEN_NEWLINE();
}

void screen_nutrition()
{
	int all_num = 0;
		product_t *s, *tmp;
		SCREEN_CURSOR_SET(SCREEN_LEFT, SCREEN_TOP);
		all_num = inventory_all_count();
		SCREEN_NEWLINE();
		if(all_num == 0) {
			disp_printf("No products ever seen"); SCREEN_NEWLINE();
		} else {
			disp_printf("%i product(s) ever seen", inventory_all_count()); SCREEN_NEWLINE();
		}
		LOCK_INVENTORY();
		HASH_ITER(hh_all, inventory_products_all, s, tmp) {
			if(s != NULL) {
				char name[50];
				product_get_name(&s->upc, name);
				disp_printf(" %.20s", name); SCREEN_NEWLINE();
			}
		}

		UNLOCK_INVENTORY();
}
