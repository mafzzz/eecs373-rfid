/*
 * product.h
 *
 *  Created on: Mar 21, 2011
 *      Author: Zach
 */
#ifndef PRODUCT_H_
#define PRODUCT_H_

#include <inttypes.h>
#include <sys/linux_list.h>

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
	uint32_t rfid_id;
	uint32_t upc;
	char * name;
	uint32_t present;
	nutrition_t *nutrition;
	struct list_head inventory_list;
} product_t;


/* Nutrition functionality */
void nutrition_init(nutrition_t *nutrition, uint32_t upc);

/* Product functionality */
void product_init(product_t *product, uint32_t rfid_id, uint32_t upc, int do_nutrition_search);


#endif /* PRODUCT_H_ */