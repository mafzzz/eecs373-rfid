#ifndef DISP_H__
#define DISP_H__
#include "driver/core_uart_apb.h"
#include "devices.h"


#define DISP_UART_BASE	0x40050000UL
#define DISP_BAUD_RATE	115200
#define DISP_BAUD_VALUE ((FAB_CLK / (DISP_BAUD_RATE*16))-1)

/* Display commands */
#define DISP_CMD_START		0x7C
#define DISP_CMD_CLEAR		0x00
#define DISP_CMD_MOVEX		0x18
#define DISP_CMD_MOVEY		0x19
#define DISP_CMD_SETPIXEL	0x10
#define DISP_CMD_INVERT		0x12
#define DISP_CMD_SETPIXEL	0x10
#define DISP_CMD_DEMO		0x04
#define DISP_CMD_BACKLIGHT	0x02
#define DISP_CMD_LINE		0x0C
#define DISP_CMD_CLEAR_RECT	0x05
#define DISP_CMD_BAUD		0x07

#define DISP_MAX_STRLEN 100 /* For disp_print */

#define DISP_LEFT 0
#define DISP_RIGHT 159
#define DISP_TOP 127
#define DISP_BOTTOM 0

UART_instance_t g_disp_uart;

typedef struct {
	uint8_t *data;
	uint32_t size;
} disp_req_t;

int disp_init();
void disp_clear();
void disp_clear_rect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void disp_draw_pixel(uint8_t x, uint8_t y);
void disp_set_pixel(uint8_t x, uint8_t y, uint8_t val);
void disp_move_cursor(uint8_t x, uint8_t y);
void disp_putc(const char c);
int disp_printf(const char * fmt, ...);
void disp_newline();
void disp_invert();
void disp_demo();
void disp_set_backlight(int level);
void disp_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void disp_set_baud(char rate);

#endif
