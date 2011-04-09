#include <stdlib.h>
#include <inttypes.h>
#include <app/product.h>

/* Product functionality */
void product_init(product_t *product, uint32_t rfid_id, char * upc, int do_nutrition_search)
{
	product->rfid_id = rfid_id;
	product->upc = upc;
	product->name = NULL;
	// Place product name request to lookup

	if(0 /* TODO: Nutrition lookup */) {
		nutrition_t *new_nutrition = (nutrition_t *)malloc(sizeof(nutrition_t));
		nutrition_init(new_nutrition, upc);
		product->nutrition = new_nutrition;
	} else {
		product->nutrition = NULL;
	}
}

/* Nutrition functionality */
void nutrition_init(nutrition_t *nutrition, char * upc)
{

}
