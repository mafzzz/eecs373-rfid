#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#include "sys/FreeRTOS.h"
#include "sys/queue.h"
#include "sys/task.h"
#include "sys/defs.h"

#include "app/app.h"
#include "app/product.h"
#include "app/inventory.h"

enum control_state {
	STATE_IDLE = 0,
	STATE_GOTBARCODE,
	STATE_FRESHEN
};

static int state, next_state;
/* Main control thread loop */

void control_task()
{
	for(;;) {

		printf("CONTROL: Start loop\r\n");

		// Queue message counts
		uint32_t rfid_num_messages = 0;
		uint32_t barcode_num_messages = 0;
		uint32_t sensors_num_messages = 0;
		uint32_t keypad_num_messages = 0;

		int rfid_message;
		char * barcode_message;

		rfid_num_messages = uxQueueMessagesWaiting(g_rfid_queue);
		barcode_num_messages = uxQueueMessagesWaiting(g_barcode_queue);
		sensors_num_messages = uxQueueMessagesWaiting(g_sensors_queue);
		keypad_num_messages = uxQueueMessagesWaiting(g_keypad_queue);


		printf(" CONTROL: rfid messages: %d\n\r", rfid_num_messages);
		printf(" CONTROL: barcode messages: %d\n\r", barcode_num_messages);
		printf(" CONTROL: sensors messages: %d\n\r", sensors_num_messages);
		printf(" CONTROL: keypad messages: %d\n\r", keypad_num_messages);

		//printf("CONTROL: barcode messages: %u\r\n", barcode_num_messages);
		//printf("CONTROL: sensor messages: %u\r\n", sensors_num_messages);
		//printf("CONTROL: keypad messages: %u\r\n", keypad_num_messages);

		//Status test to screen

		/* Control logic */
		switch(state) {
		case STATE_IDLE:
			printf("CONTROL: State = IDLE\r\n");
			// Got BARCODE
			if(barcode_num_messages > 0) {
				xQueueSend(g_screen_status_queue, "Got barcode wait rfid", 0);
				// Wait for a fresh RFID tag and go to GOTBARCODE state
				while(xQueueReceive( g_rfid_queue, &rfid_message, ( portTickType ) 0 )); // We want a fresh rfid tag, so empty queue
				next_state = STATE_GOTBARCODE;
			}
			// Got RFID tag
			else if(rfid_num_messages > 0) {
				xQueueSend(g_screen_status_queue, "Got rfid", 0);
				struct product_t *product;
				xQueueReceive( g_rfid_queue, &rfid_message, ( portTickType ) 0 );

				// Check if we have ever seen this RFID tag before
				product = inventory_all_lookup(rfid_message);

				if(product) {
					// We've seen the tag before, so toggle it's stocked state
					inventory_stocked_toggle(product);
				} else {
					/* We don't do anything with RFID tags we haven't seen before,
					 * unless we've received barcode first (in which case we handle in STATE_GOTBARCODE)
					 */
				}
				next_state = STATE_IDLE;
			}

			break;
		case STATE_GOTBARCODE:
			printf("CONTROL: State = GOTBARCODE\r\n");

			// We got a barcode.  Wait for a RFID tag to show up
			if(rfid_num_messages > 0) {
				struct product_t *product;

				xQueueReceive( g_barcode_queue, &barcode_message, ( portTickType ) 0 );
				xQueueReceive( g_rfid_queue, &rfid_message, ( portTickType ) 0 );

				product = inventory_all_lookup(rfid_message);
				if(product) {
					// We've seen this RFID tag before!  This is bad!  Freshen and idle!
				} else {
					// Got barcode, and a new RFID tag.  Make a new product and add to inventory
					product = malloc(sizeof(product_t));
					product_init(product, rfid_message, barcode_message, 0 /* No nutrition lookup yet */);
					inventory_all_add(product);
					// New product starts unstocked, so stock it
					inventory_stocked_toggle(product);
				}
				next_state = STATE_FRESHEN;
			} else {
				next_state = STATE_GOTBARCODE;
			}
			break;
		case STATE_FRESHEN:
			printf("CONTROL: State = FRESHEN\r\n");
			xQueueSend(g_screen_status_queue, "FRESHEN: Ignore rfid/bar", 0);
			vTaskDelay(500);		//Put the thread to sleep for a while
			// Clear RFID and BARCODE queues on wakeup
			while(xQueueReceive( g_rfid_queue, &rfid_message, ( portTickType ) 0 ));
			while(xQueueReceive( g_barcode_queue, &barcode_message, ( portTickType ) 0 ));
			next_state = STATE_IDLE;
			break;

		}
		// Go to next state
		state = next_state;


	}
}
