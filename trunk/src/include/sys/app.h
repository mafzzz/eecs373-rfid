#include "sys/FreeRTOS.h"
#include "sys/queue.h"

#ifndef _APP_H
#define _APP_H_

void init();

extern xQueueHandle sensors_screen_queue;

#endif
