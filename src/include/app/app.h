/*
 * app.h
 *
 *  Created on: Apr 9, 2011
 *      Author: Zach
 */
#include "sys/FreeRTOS.h"
#include "sys/queue.h"

#ifndef APP_H_
#define APP_H_

// Input -> Control queues
extern xQueueHandle g_rfid_queue;			// RFID tags
extern xQueueHandle g_barcode_queue;		// Barcodes
extern xQueueHandle g_sensors_queue;		// Slow poll misc sensor data
extern xQueueHandle g_keypad_queue;			// Keypad presses

// Control -> Output queues
extern xQueueHandle g_screen_status_queue;	// Screen (system status)


#endif /* APP_H_ */
