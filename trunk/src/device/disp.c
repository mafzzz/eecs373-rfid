#include "device/disp.h"
#include "driver/core_uart_apb.h"
#include <stdio.h>
#include <stdarg.h>

#define MAX_CMD_SIZE 10

void disp_req(disp_req_t *req);

int disp_init()
{
	/* Initialize UART for display settings */
	UART_init(&g_disp_uart, DISP_UART_BASE, DISP_BAUD_VALUE, DATA_8_BITS | NO_PARITY);
	return 0;
}

void disp_clear()
{
	 disp_req_t req;
	 uint8_t cmd[2];
	 cmd[0] = DISP_CMD_START;
	 cmd[1] = DISP_CMD_CLEAR;
	 req.data = (uint8_t *)&cmd[0];
	 req.size = 2;
	 disp_req(&req);
}

inline void disp_draw_pixel(uint8_t x, uint8_t y)
{
	disp_set_pixel(x, y, 1);
}


void disp_set_pixel(uint8_t x, uint8_t y, uint8_t val)
{
	disp_req_t req;
	uint8_t cmd[5];
	cmd[0] = DISP_CMD_START;
	cmd[1] = DISP_CMD_SETPIXEL;
	cmd[2] = x;
	cmd[3] = y;
	cmd[4] = val;
	req.data = (uint8_t *)&cmd[0];
	req.size = 5;
	disp_req(&req);
}

void disp_putc(char c)
{
	disp_req_t req;
	req.data = (uint8_t *)&c;
	req.size = 1;
	disp_req(&req);
}

int disp_printf(const char * fmt, ...)
{
	/* Prepare string */
	char buf[DISP_MAX_STRLEN];
	int count;
	va_list args;
	va_start(args, fmt);
	count = vsnprintf(buf, DISP_MAX_STRLEN, fmt, args);

	/* Create request */
	disp_req_t req;
	req.data = (uint8_t *)&buf[0];
	req.size = count;
	disp_req(&req);

	va_end(args);
	return count;
}

void disp_move_cursor(uint8_t x, uint8_t y)
{
	disp_req_t req;
	uint8_t cmd[3];

	/* Move X coord */
	cmd[0] = DISP_CMD_START;
	cmd[1] = DISP_CMD_MOVEX;
	cmd[2] = x;
	req.data = cmd;
	req.size = 3;
	disp_req(&req);

	cmd[0] = DISP_CMD_START;
	cmd[1] = DISP_CMD_MOVEY;
	cmd[2] = y;
	req.data = cmd;
	req.size = 3;
	disp_req(&req);
}

void disp_invert()
{
	disp_req_t req;
	uint8_t cmd[2];
	cmd[0] = DISP_CMD_START;
	cmd[1] = DISP_CMD_INVERT;
	req.data = cmd;
	req.size = 2;
	disp_req(&req);
}

void disp_demo()
{
	disp_req_t req;
	uint8_t cmd[2];
	cmd[0] = DISP_CMD_START;
	cmd[1] = DISP_CMD_DEMO;
	req.data = cmd;
	req.size = 2;
	disp_req(&req);
}

void disp_set_backlight(int level)
{
	disp_req_t req;
	uint8_t cmd[3];
	if(level < 0 || level > 100) {
		return;
	}

	cmd[0] = DISP_CMD_START;
	cmd[1] = DISP_CMD_BACKLIGHT;
	cmd[2] = level;
	req.data = cmd;
	req.size = 3;
	disp_req(&req);
}

void disp_req(disp_req_t *req)
{
	UART_send(&g_disp_uart, req->data, req->size);
}
