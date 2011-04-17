#include <sys/FreeRTOS.h>
#include <sys/queue.h>
#include <sys/task.h>
#include "device/keypad.h"
#include <inttypes.h>
#include "app/app.h"
#define REG_BASE 0x40050300


void keypad_task() {
	for(;;) {
		keypad_t keypad;
		keypad = keypad_read();
		/* Pass to screen */
		if(keypad.val != 0) {
			xQueueSend(g_keypad_queue, &keypad, 0);
		}
		/* Put thread to sleep until we want to repoll */
		vTaskDelay(portTICK_RATE_MS * 10);
	}
}
keypad_t keypad_read() {

	keypad_t state;
	uint32_t* ACCESS_REG = (uint32_t*)REG_BASE;

	*ACCESS_REG = 0x01;
	state.val = (*ACCESS_REG);
	*ACCESS_REG = 0x02;
	state.val += 16*(*ACCESS_REG);
	*ACCESS_REG = 0x04;
	state.val += 256*(*ACCESS_REG);
	*ACCESS_REG = 0x08;
	state.val += 4096*(*ACCESS_REG);
	*ACCESS_REG = 0x00;

	return state;
}

