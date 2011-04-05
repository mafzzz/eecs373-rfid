/*
 * barcode.c
 *
 *  Created on: Mar 25, 2011
 *      Author: mladmon
 */

#include "device/barcode.h"
#include "driver/core_uart_apb.h"
#include <stdio.h>
#include <stdarg.h>

void barcode_init() {
	/* Initialize CoreUARTapb for bar code scanner settings */
	UART_init(&g_barcode_uart, BARCODE_UART_BASE_ADDR, BARCODE_BAUD_VALUE, DATA_8_BITS | NO_PARITY);
}


