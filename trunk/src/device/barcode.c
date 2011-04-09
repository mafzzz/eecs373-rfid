/*
 * barcode.c
 *
 *  Created on: Mar 25, 2011
 *      Author: mladmon
 */

//#include "device/barcode.h"
#include "driver/core_uart_apb.h"
#include <stdio.h>
#include <stdarg.h>

#include "sys/FreeRTOS.h"
#include "sys/semphr.h"

#include "app/app.h"

static xSemaphoreHandle barcode_top_sem;


__attribute__ ((interrupt)) void Barcode_Handler(void)
{
	xSemaphoreGive(barcode_top_sem);
}

void barcode_task() {
	for(;;) {
		char * barcode_str = "1234567";
		// Wait for wakeup from interrupt
		xSemaphoreTake(barcode_top_sem, portMAX_DELAY);

		// Grab barcode data, put in queue to control module
		xQueueSend(g_barcode_queue, &barcode_str, 0);
	}
}

void barcode_init() {
	/* Initialize CoreUARTapb for bar code scanner settings */
	//UART_init(&g_barcode_uart, BARCODE_UART_BASE_ADDR, BARCODE_BAUD_VALUE, DATA_8_BITS | NO_PARITY);
	vSemaphoreCreateBinary( barcode_top_sem );
}


