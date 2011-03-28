/*
 * main.c
 *
 *  Created on: Mar 27, 2011
 *      Author: Dan
 */

#include <stdio.h>
#include <inttypes.h>
#include "device/network_driver.h"
#include "driver/mss_uart.h"

int main()
{
	printf("Starting ethernet driver test project.");
	init_network();

	while(1) {}

	return 0;
}
