/*
 * app.h
 *
 *  Created on: Apr 9, 2011
 *      Author: Zach
 */
#include "sys/FreeRTOS.h"
#include "sys/queue.h"

#include "device/rfid.h"

#ifndef APP_H_
#define APP_H_

// Input -> Control queues
extern xQueueHandle g_rfid_queue;			// RFID tags
extern xQueueHandle g_barcode_queue;		// Barcodes
extern xQueueHandle g_sensors_queue;		// Slow poll misc sensor data
extern xQueueHandle g_keypad_queue;			// Keypad presses

// Control -> Output queues
extern xQueueHandle g_screen_status_queue;	// Screen (system status)

#define MAX_NAME_LEN 20 // Maximum name length of a product
#define BARCODE_LEN 11 // 1 byte for string terminator
#define RFID_LEN 16 // 1 byte for string terminator
#define BARCODE_MSG_LEN (BARCODE_LEN+1)
#define RFID_MSG_LEN (RFID_LEN+1)
#define STATUS_MSG_LEN 40

typedef struct {
	char data [BARCODE_MSG_LEN];
	int test;
} barcode_t;

typedef struct {
	char data [RFID_MSG_LEN];
	int test;
} rfid_t;

typedef struct status_t {
	char data [STATUS_MSG_LEN];
} status_t;

#endif /* APP_H_ */
