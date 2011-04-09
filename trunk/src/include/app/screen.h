#ifndef _SCREEN_H
#define _SCREEN_H

#include <inttypes.h>

#define SCREEN_TOP 128
#define SCREEN_LEFT 0

typedef enum screen_mode_t {
	SCREEN_MODE_STARTUP = 0,
	SCREEN_MODE_IDLE,
	SCREEN_MODE_INVENTORY,
	SCREEN_MODE_NUTRITION,
	SCREEN_MODE_STATS,
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
void screen_redraw(void);

#endif
