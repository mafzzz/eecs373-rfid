#ifndef _SCREEN_H
#define _SCREEN_H

#include <inttypes.h>

#define SCREEN_TOP 119
#define SCREEN_BOTTOM 10
#define SCREEN_LEFT 0
#define SCREEN_RIGHT 159
#define SCREEN_CHAR_HEIGHT	9
#define SCREEN_CHAR_WIDTH	6

typedef enum screen_mode_t {
	SCREEN_MODE_STARTUP = 0,
	SCREEN_MODE_IDLE,
	SCREEN_MODE_INVENTORY,
	SCREEN_MODE_NUTRITION,
	SCREEN_MODE_RECIPE,
	SCREEN_MODE_COUNT
} screen_mode_t;

typedef struct {
	uint32_t x;
	uint32_t y;
} screen_curs_t;

void screen_task();

void screen_init(void);
void screen_mode(screen_mode_t new_mode);
void screen_redraw(int re_dbg, int re_mode, int re_menu);

#endif
