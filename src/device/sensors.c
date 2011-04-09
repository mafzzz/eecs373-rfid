#include "sys/FreeRTOS.h"
#include "sys/task.h"
#include "sys/queue.h"
#include "sys/portmacro.h"
#include "sys/defs.h"

#include "device/temp.h"

#include "app/sensors.h"
#include "app/app.h"

void sensors_task()
{
	int current_temp;
	for(;;) {
		/* Put thread to sleep until we want to repoll */
		vTaskDelay(portTICK_RATE_MS * SENSOR_POLL_MS);

		/* Poll the temperature sensor */
		//current_temp = temp_read();
		current_temp = 123;

		/* Pass to control unit */
		xQueueSend(g_sensors_queue, &current_temp, 0);
	}
}
