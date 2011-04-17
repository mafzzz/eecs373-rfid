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

static int state, next_state, prev_state;
/* Main control thread loop */

static int barcode_wait = 0;
void control_task()
{
	xQueueSendToBack(g_screen_status_queue, "Waiting for RFID or Barcode",portMAX_DELAY);

	for(;;) {
		printf("CONTROL: Start loop\r\n");

		// Queue message counts
		uint32_t rfid_num_messages = 0;
		uint32_t barcode_num_messages = 0;

		barcode_t barcode_msg;
		rfid_t rfid_msg;

		rfid_num_messages = uxQueueMessagesWaiting(g_rfid_queue);
		barcode_num_messages = uxQueueMessagesWaiting(g_barcode_queue);

		/* Control logic */
		switch(state) {
		case STATE_IDLE:
			if(state != prev_state) {
				xQueueSendToBack(g_screen_status_queue, "Waiting for product",portMAX_DELAY);
			}
			// Got RFID
			if(rfid_num_messages > 0) {
				product_t *product;
				xQueueReceive( g_rfid_queue, &rfid_msg, ( portTickType ) 0 );

				// Check if we have ever seen this RFID tag before
				product = inventory_all_lookup(&rfid_msg);

				if(product) {
					// We've seen the tag before, so toggle it's stocked state
					if(inventory_stocked_toggle(product)) {
						xQueueSendToBack(g_screen_status_queue, "Product inserted",portMAX_DELAY);
					} else {
						xQueueSendToBack(g_screen_status_queue, "Product removed",portMAX_DELAY);
					}
					next_state = STATE_FRESHEN;
				} else {
					/* We don't do anything with RFID tags we haven't seen before,
					 * unless we've received barcode first (in which case we handle in STATE_GOTBARCODE)
					 */
					xQueueSendToBack(g_screen_status_queue, "Didn't recognize RFID", portMAX_DELAY);
					next_state = STATE_IDLE;
				}
			} else if(barcode_num_messages > 0) {
				xQueueSendToBack(g_screen_status_queue, "Place in fridge", portMAX_DELAY);
				// Wait for a fresh RFID tag and go to GOTBARCODE state
				next_state = STATE_GOTBARCODE;
			}

			break;
		case STATE_GOTBARCODE:
			printf("CONTROL: State = GOTBARCODE\r\n");
			xQueueReceive( g_barcode_queue, &barcode_msg, ( portTickType ) 0 );

			// We got a barcode.  Wait for a RFID tag to show up
			if(rfid_num_messages > 0) {
				struct product_t *product;

				xQueueReceive( g_rfid_queue, &rfid_msg, ( portTickType ) 0 );

				product = inventory_all_lookup(&rfid_msg);
				if(product) {
					// We've seen this RFID tag before!  This is bad!  Freshen and idle!
					xQueueSendToBack(g_screen_status_queue, "Bad pair, RFID exists", portMAX_DELAY);

				} else {
					// Got barcode, and a new RFID tag.  Make a new product and add to inventory
					product = malloc(sizeof(product_t));
					product_init(product, rfid_msg, barcode_msg, 0 /* No nutrition lookup yet */);
					inventory_all_add(product);
					// New product starts unstocked, so stock it
					inventory_stocked_toggle(product);
					xQueueSendToBack(g_screen_status_queue, "New item inserted", portMAX_DELAY);

				}
				next_state = STATE_FRESHEN;
			} else {

				if(!barcode_wait) {
					/* Give 5 seconds to get an RFID tag */
					barcode_wait = 1;
					next_state = STATE_GOTBARCODE;
					vTaskDelay(portTICK_RATE_MS * 5000);
				} else {
					barcode_wait = 0;
					//xQueueSendToBack(g_screen_status_queue, "Giving up on barcode", portMAX_DELAY);
					next_state = STATE_IDLE;
				}
			}
			break;
		case STATE_FRESHEN:
			printf("CONTROL: State = FRESHEN\r\n");
			vTaskDelay(portTICK_RATE_MS * 2000);		//Put the thread to sleep for a while
			// Clear RFID and BARCODE queues on wakeup
			while(xQueueReceive( g_rfid_queue, &rfid_msg, ( portTickType ) 0 ));
			while(xQueueReceive( g_barcode_queue, &barcode_msg, ( portTickType ) 0 ));
			next_state = STATE_IDLE;
			break;

		}
		// Go to next state
		prev_state = state;
		state = next_state;


	}
}
