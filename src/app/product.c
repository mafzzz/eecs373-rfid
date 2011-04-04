#include <stdlib.h>
#include <inttypes.h>
#include <app/product.h>

/* Product functionality */
void product_init(product_t *product, uint32_t rfid_id, uint32_t upc, int do_nutrition_search)
{
	product->rfid_id = rfid_id;
	product->upc = upc;
	product->name = NULL;
	if(do_nutrition_search) {
		nutrition_t *new_nutrition = (nutrition_t *)malloc(sizeof(nutrition_t));
		nutrition_init(new_nutrition, upc);
		product->nutrition = new_nutrition;
	} else {
		product->nutrition = NULL;
	}
}

/* Nutrition functionality */
void nutrition_init(nutrition_t *nutrition, uint32_t upc)
{
	/* TODO: Implement nutrition web lookup based on a UPC */
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
}
