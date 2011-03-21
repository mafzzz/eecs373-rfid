/*
 * screen.c
 *
 *  Created on: Mar 21, 2011
 *      Author: Zach
 */

#include <sys/list.h>
#include <device/disp.h>
#include <app/screen.h>

screen_mode_t screen_cur_mode;

void screen_init()
{
	disp_init();
	screen_cur_mode = SCREEN_MODE_WELCOME;
	screen_redraw();
}

void screen_mode(screen_mode_t new_mode)
{
	screen_cur_mode = new_mode;
	screen_redraw();
}

/* Redraws the screen based on the current mode */
void screen_redraw()
{
	switch(screen_cur_mode) {
	case SCREEN_MODE_WELCOME:
		break;
	case SCREEN_MODE_INVENTORY:
		screen_inventory();
		break;
	case SCREEN_MODE_STATS:
		break;
	case SCREEN_MODE_RECIPE:
		break;
	}
}

void screen_inventory() {

}
