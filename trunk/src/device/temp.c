/*
 * temp.c
 *
 *  Created on: Apr 4, 2011
 *      Author: djonik
 */
#include "device/temp.h"

ace_channel_handle_t adc_handle;

void temp_init() {
	ACE_init();
	adc_handle = ACE_get_channel_handle((const uint8_t *)TEMP_CHANNEL_HANDLE);
}


uint16_t temp_read() {

	uint16_t adc_data = ACE_get_ppe_sample(adc_handle);
	return adc_data >> 4;
}
