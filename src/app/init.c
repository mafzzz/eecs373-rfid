#include <stdio.h>
/* FreeRTOS kernel */
#include "sys/FreeRTOS.h"
#include "sys/task.h"
#include "sys/queue.h"
#include "sys/semphr.h"

/* System */
#include "sys/defs.h"

/* Application */
#include "app/app.h"
#include "app/sensors.h"
#include "app/screen.h"
#include "app/control.h"

/* Device wakeup semaphores */
xSemaphoreHandle barcode_sem;
xSemaphoreHandle rfid_sem;

/* Tasks */
xTaskHandle screen_task_handle; // Responsible for updating the screen
xTaskHandle rfid_task_handle; // Responsible for receiving RFID tags and deciding appropriate action
xTaskHandle barcode_task_handle; // Responsible for receiving barcode tags and deciding appropriate action
xTaskHandle sound_task_handle; // Responsible for outputting sound requests
xTaskHandle sensors_task_handle; // Responsible for receiving temperature data at a reasonable rate
xTaskHandle control_task_handle;

/* Queues */
// Input -> Control queues
xQueueHandle g_rfid_queue;			// RFID tags
xQueueHandle g_barcode_queue;		// Barcodes
xQueueHandle g_sensors_queue;		// Slow poll sensor data
xQueueHandle g_keypad_queue;		// Keypad presses

// Control -> Output queues
xQueueHandle g_screen_status_queue;	// Screen (system status)

const char* screen_task_name = "SCREENTASK";
const char* rfid_task_name = "RFIDTASK";
const char* barcode_task_name = "BARCODETASK";
const char* sensors_task_name = "SENSORSTASK";
const char* control_task_name = "CONTROLTASK";


int main()
{
	printf("Init starting\n");
	/* TODO Configure hardware */

	/* Create device wakeup semaphores */
	vSemaphoreCreateBinary(barcode_sem);
	vSemaphoreCreateBinary(rfid_sem);

	/* Create the tasks */
	xTaskCreate( screen_task,  (signed portCHAR * )screen_task_name, DEFAULT_TASK_STACK_SIZE, NULL, 0, &screen_task_handle );
	//xTaskCreate( sensors_task,  (signed portCHAR * )sensors_task_name, DEFAULT_TASK_STACK_SIZE, NULL, 0, &sensors_task_handle );
	xTaskCreate( control_task,  (signed portCHAR * )control_task_name, DEFAULT_TASK_STACK_SIZE, NULL, 0, &control_task_handle );

	/* Create the queues */
	g_rfid_queue = xQueueCreate(10, sizeof(uint32_t));
	g_barcode_queue = xQueueCreate(10, sizeof(char) * 9);
	g_sensors_queue = xQueueCreate(10, sizeof(uint32_t));
	g_keypad_queue = xQueueCreate(10, sizeof(uint32_t));

	screen_init();

	/* Start the scheduler, this should never return */
 	vTaskStartScheduler();

	/* Scheduler failed to start (died), the world is ending */
	while(1) {}
	return 0;
}
