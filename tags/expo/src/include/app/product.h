/*
 * product.h
 *
 *  Created on: Mar 21, 2011
 *      Author: Zach
 */
#ifndef PRODUCT_H_
#define PRODUCT_H_

#include <inttypes.h>

#include "sys/uthash.h"

#include "app/app.h"


typedef struct nutrition_t {
	uint32_t servings_total;
	uint32_t servings_remaining;
	uint32_t cals_per_serving;
	/* Below units in milligrams */
	uint32_t fat_per_serving;
	uint32_t satfat_per_serving;
	uint32_t transfat_per_serving;
	uint32_t cholesterol_per_serving;
	uint32_t sodium_per_serving;
	uint32_t carbs_per_serving;
	uint32_t fiber_per_serving;
	uint32_t sugar_per_serving;
	uint32_t protein_per_serving;
} nutrition_t;

typedef struct product_t {
	rfid_t rfid;
	barcode_t upc;
	char name[MAX_NAME_LEN];
	uint32_t stocked;
	nutrition_t *nutrition;

	// List and hash structures
	struct product_t *next;
	struct product_t *prev;
	UT_hash_handle hh_all;
	UT_hash_handle hh_stocked;

} product_t;


/* Nutrition functionality */
void nutrition_init(nutrition_t *nutrition, char * upc);

/* Product functionality */
void product_init(product_t *product, rfid_t rfid, barcode_t upc, int do_nutrition_search);

void product_get_name(barcode_t *upc, char * name);

#endif /* PRODUCT_H_ */
