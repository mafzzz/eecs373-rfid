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

__attribute__ ((interrupt)) void RFID_Handler(void)
{
	xSemaphoreGive(rfid_top_sem);
	MSS_GPIO_disable_irq(MSS_GPIO_1);
}

void rfid_task() {
	char rx_data[20];
	for(;;) {
		barcode_msg_t msg;
		int bytes_read;
		int i;
		// Wait for wakeup from interrupt
		xSemaphoreTake(rfid_top_sem, portMAX_DELAY);

		/* RXRDY is high when data is available in the receive data buffer/FIFO.
		   This bit is cleared by reading the Receive Data Register.	 */
		bytes_read = UART_get_rx(&g_rfid_uart, (unsigned char*)&msg.data, 20);
		MSS_GPIO_clear_irq( MSS_GPIO_1 );
		NVIC_ClearPendingIRQ(GPIO1_IRQn);
		MSS_GPIO_enable_irq(MSS_GPIO_1);
	}
}

void rfid_init() {
	/* Initialize CoreUARTapb for bar code scanner settings */
	UART_init(&g_rfid_uart, RFID_UART_BASE_ADDR, RFID_BAUD_VALUE, DATA_8_BITS | NO_PARITY);

	/* Initialize the MSS GPIO & GPIO_0 Interrupt */
    MSS_GPIO_init();
    MSS_GPIO_config( MSS_GPIO_1, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_BOTH );
    MSS_GPIO_enable_irq( MSS_GPIO_1 );
    NVIC_EnableIRQ(GPIO1_IRQn);

	vSemaphoreCreateBinary( rfid_top_sem );
}


