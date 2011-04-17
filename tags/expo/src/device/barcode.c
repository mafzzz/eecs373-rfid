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

#include "driver/mss_gpio.h"
#include "device/barcode.h"

#include "app/app.h"

#define BARCODE_FULL_LENGTH    13

static xSemaphoreHandle barcode_top_sem;
static int _barcode_init_done = 0;

__attribute__ ((interrupt)) void Barcode_Handler(void)
{
	NVIC_ClearPendingIRQ(GPIO0_IRQn);
	MSS_GPIO_disable_irq(MSS_GPIO_0);
	if(!_barcode_init_done) {
		return;
	}
	xSemaphoreGive(barcode_top_sem);
}

void barcode_task() {
	char rx_data[BARCODE_FULL_LENGTH];
	for(;;) {
		barcode_t msg;
		int bytes_read;
		char temp;
		int i;
		// Wait for wakeup from interrupt
		xSemaphoreTake(barcode_top_sem, portMAX_DELAY);

		/* RXRDY is high when data is available in the receive data buffer/FIFO.
		   This bit is cleared by reading the Receive Data Register.	 */
		bytes_read = UART_get_rx(&g_barcode_uart, (unsigned char*)&rx_data, BARCODE_FULL_LENGTH);

		if(bytes_read == BARCODE_FULL_LENGTH) // Includes \r and \n
		{
			for(i=0; i < BARCODE_LEN; i++){
				msg.data[i] = (rx_data[i] & 0x0F) + '0'; //Get character numeral from int
			}
			msg.data[BARCODE_MSG_LEN-1] = '\0';

			// Grab barcode data, put in queue to control module
			xQueueSendToBack(g_barcode_queue, &msg, 0);

		}
		while((bytes_read = UART_get_rx(&g_barcode_uart, (unsigned char*)&temp, 100)) > 0);
		MSS_GPIO_enable_irq(MSS_GPIO_0);
	}
}

void barcode_init() {
	/* Initialize CoreUARTapb for bar code scanner settings */
	UART_init(&g_barcode_uart, BARCODE_UART_BASE_ADDR, BARCODE_BAUD_VALUE, DATA_8_BITS | NO_PARITY);

	/* Initialize the MSS GPIO & GPIO_0 Interrupt */
    MSS_GPIO_config( MSS_GPIO_0, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE);
    MSS_GPIO_enable_irq( MSS_GPIO_0 );
    NVIC_EnableIRQ(GPIO0_IRQn);


	vSemaphoreCreateBinary( barcode_top_sem );
	_barcode_init_done = 1;
}


