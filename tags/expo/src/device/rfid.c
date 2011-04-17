/*
 * rfid.c
 *
 */

#include "driver/core_uart_apb.h"
#include <stdio.h>
#include <stdarg.h>

#include "sys/FreeRTOS.h"
#include "sys/semphr.h"

#include "driver/mss_gpio.h"
#include "device/rfid.h"

#include "app/app.h"

static xSemaphoreHandle rfid_top_sem;

static int _rfid_init_done = 0;

__attribute__ ((interrupt)) void RFID_Handler(void)
{
	MSS_GPIO_clear_irq( MSS_GPIO_1 );

	if(!_rfid_init_done) {
		return;
	}
	xSemaphoreGive(rfid_top_sem);
	MSS_GPIO_disable_irq(MSS_GPIO_1);
}

void rfid_task() {
	for(;;) {
		char temp;
		rfid_t msg;
		int bytes_read;
		int i;
		// Wait for wakeup from interrupt
		vTaskDelay(portTICK_RATE_MS * 100);
		xSemaphoreTake(rfid_top_sem, portMAX_DELAY);

		/* Wait until we get a 4C (start of header) */
		while(1) {
			bytes_read = UART_get_rx(&g_rfid_uart, (unsigned char*)&temp, 1);
			if(bytes_read == 0) {
				break;
			} else if(temp == 0x4C) {

				bytes_read = UART_get_rx(&g_rfid_uart, (unsigned char*)&temp, 1);
				if(bytes_read == 0 || temp != 0x57) {
					/* Good read */
					break;
				}

				/* Don't care about the transponder type */
				bytes_read = UART_get_rx(&g_rfid_uart, (unsigned char*)&temp, 1);

				/* RXRDY is high when data is available in the receive data buffer/FIFO.
				This bit is cleared by reading the Receive Data Register.	 */
				bytes_read = UART_get_rx(&g_rfid_uart, (unsigned char*)&msg.data, RFID_LEN);
				if(bytes_read != RFID_LEN) {
					break;
				}

				msg.data[RFID_MSG_LEN-1] = '\0';

				/* Pass ID to control unit */
				xQueueSendToBack(g_rfid_queue, &msg, 0);
				vTaskDelay(portTICK_RATE_MS * 1000); //Ignore rfid for a short bit

				/* Break out of while loop */
				break;
			}
		}

		/* Clear the buffer until it's empty */
		while((bytes_read = UART_get_rx(&g_rfid_uart, (unsigned char*)&temp, 1)) > 0);

		MSS_GPIO_enable_irq(MSS_GPIO_1);
	}
}

void rfid_init() {
	/* Initialize CoreUARTapb for bar code scanner settings */
	UART_init(&g_rfid_uart, RFID_UART_BASE_ADDR, RFID_BAUD_VALUE, DATA_8_BITS | NO_PARITY);

	/* Initialize the MSS GPIO & GPIO_0 Interrupt */
    MSS_GPIO_config( MSS_GPIO_1, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_BOTH );
    MSS_GPIO_enable_irq( MSS_GPIO_1 );
    NVIC_EnableIRQ(GPIO1_IRQn);

	vSemaphoreCreateBinary( rfid_top_sem );
	_rfid_init_done = 1;
}


