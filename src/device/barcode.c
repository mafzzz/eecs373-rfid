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

#define BARCODE_LENGTH    13
#define BARCODE_MEANINGFUL 11

static xSemaphoreHandle barcode_top_sem;

__attribute__ ((interrupt)) void Barcode_Handler(void)
{
	MSS_GPIO_clear_irq( MSS_GPIO_0 );
	NVIC_ClearPendingIRQ(GPIO0_IRQn);
	xSemaphoreGive(barcode_top_sem);
}

void barcode_task() {
	char rx_data[BARCODE_LENGTH];
	for(;;) {
		barcode_msg_t msg;
		int bytes_read;
		int i;
		// Wait for wakeup from interrupt
		xSemaphoreTake(barcode_top_sem, portMAX_DELAY);

		/* RXRDY is high when data is available in the receive data buffer/FIFO.
		   This bit is cleared by reading the Receive Data Register.	 */
		bytes_read = UART_get_rx(&g_barcode_uart, (unsigned char*)&msg.data, BARCODE_LENGTH);

		if(bytes_read == BARCODE_LENGTH) // Includes \r and \n
		{
			for(i=0; i < BARCODE_MEANINGFUL; i++){
				rx_data[i] = (rx_data[i] & 0x0F) + '0'; //Get character numeral from int
			}
			rx_data[BARCODE_MEANINGFUL] = '\0'; //Null term barcode

			// Grab barcode data, put in queue to control module
			xQueueSend(g_barcode_queue, &msg, 0);
		}
	}
}

void barcode_init() {
	/* Initialize CoreUARTapb for bar code scanner settings */
	UART_init(&g_barcode_uart, BARCODE_UART_BASE_ADDR, BARCODE_BAUD_VALUE, DATA_8_BITS | NO_PARITY);

	/* Initialize the MSS GPIO & GPIO_0 Interrupt */
    MSS_GPIO_init();
    MSS_GPIO_config( MSS_GPIO_0, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE );
    MSS_GPIO_enable_irq( MSS_GPIO_0 );
    NVIC_EnableIRQ(GPIO0_IRQn);


	vSemaphoreCreateBinary( barcode_top_sem );
}


