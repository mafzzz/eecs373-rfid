/*
 * barcode.h
 *
 *  Created on: Mar 25, 2011
 *      Author: mladmon
 */

#ifndef BARCODE_H_
#define BARCODE_H_

#include "driver/core_uart_apb.h"
#include "devices.h"

/* Configuration Parameters */
#define BARCODE_UART_BASE_ADDR 0x40050200UL
#define BARCODE_BAUD_RATE 9600
#define BARCODE_BAUD_VALUE ((FAB_CLK / (BARCODE_BAUD_RATE*16))-1)

/* UART_instance_t instantiation for the bar code scanner CoreUARTapb hardware instance */
UART_instance_t g_barcode_uart;


/* Functions */
void barcode_init();



#endif /* BARCODE_H_ */
