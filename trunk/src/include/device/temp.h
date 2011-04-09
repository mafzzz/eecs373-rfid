/*
 * temp.h
 *
 *  Created on: Apr 4, 2011
 *      Author: djonik
 */

#ifndef TEMP_H_
#define TEMP_H_

#include "driver/mss_ace.h"
#include "driver/mss_uart.h"

#define TEMP_CHANNEL_HANDLE "TEMP_INPUT"

/* Initializes necessary peripherals for reading temperature. */
void temp_init();

/* Reads the current temperature in degrees Farenheit. */
uint16_t temp_read();

#endif /* TEMP_H_ */
