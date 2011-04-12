/*
 * rfid.h
 *
 */

#ifndef RFID_H_
#define RFID_H_

#include "driver/core_uart_apb.h"
#include "devices.h"

/* Configuration Parameters */
#define RFID_UART_BASE_ADDR 0x40050100UL
#define RFID_BAUD_RATE 9600
#define RFID_BAUD_VALUE ((FAB_CLK / (RFID_BAUD_RATE*16))-1)

/* UART_instance_t instantiation for the rfid CoreUARTapb hardware instance */
UART_instance_t g_rfid_uart;

/* Functions */
void rfid_init();

void rfid_task();



#endif /* RFID_H_ */
