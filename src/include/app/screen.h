#ifndef __SCREEN_H__
#define __SCREEN_H__

typedef enum screen_mode_t {
	SCREEN_MODE_WELCOME = 0,
	SCREEN_MODE_INVENTORY,
	SCREEN_MODE_STATS,
	SCREEN_MODE_RECIPE
} screen_mode_t;

void screen_init(void);
void screen_mode(screen_mode_t new_mode);
void screen_redraw(void);


#endif
