#include "device/barcode.h"
#include "driver/mss_watchdog.h"
#include <inttypes.h>
#include "driver/mss_gpio.h"

/* create a receive data buffer */
#define MAX_RX_DATA_SIZE    13
#define PARSED_RX_DATA_SIZE 11
uint8_t rx_data[MAX_RX_DATA_SIZE];
char parsed_rx_data[PARSED_RX_DATA_SIZE];
uint8_t rx_size;

__attribute__ ((interrupt)) void Barcode_Handler(void)
{
	/* RXRDY is high when data is available in the receive data buffer/FIFO.
	   This bit is cleared by reading the Receive Data Register.	 */
	rx_size = UART_get_rx(&g_barcode_uart, rx_data, MAX_RX_DATA_SIZE);

	int i;
	if(rx_size == 13) //if statement for debugging purposes
	{
		for(i=0; i < PARSED_RX_DATA_SIZE; i++){
			parsed_rx_data[i] = (char)(rx_data[i] & 0x0F);
		}
	}

	MSS_GPIO_clear_irq( MSS_GPIO_0 );
	NVIC_ClearPendingIRQ(GPIO0_IRQn);
}

int main()
{
    /* Watchdog Disabling function */
    MSS_WD_disable();

    /* Initialize the bar code scanner CoreUARTapb hardware */
    barcode_init();

    /* Initialize the MSS GPIO & GPIO_0 Interrupt */
    MSS_GPIO_init();
    MSS_GPIO_config( MSS_GPIO_0, MSS_GPIO_INPUT_MODE | MSS_GPIO_IRQ_EDGE_POSITIVE );
    MSS_GPIO_enable_irq( MSS_GPIO_0 );
    NVIC_EnableIRQ(GPIO0_IRQn);

    while(1){}

	return 0;
}

/*
    uint8_t err_status;
    err_status = UART_get_err_status(&g_uart);

    if(UART_APB_NO_ERROR == err_status )
    {
         rx_size = UART_get_rx( &g_uart, rx_data, MAX_RX_DATA_SIZE );
    }


   void send_using_interrupt
   (
       uint8_t * pbuff,
       size_t tx_size
   )
   {
       size_t size_in_fifo;
       size_in_fifo = UART_fill_tx_fifo( &g_uart, pbuff, tx_size );
   }
*/
