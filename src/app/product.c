#include <stdlib.h>
#include <inttypes.h>
#include "app/app.h"
#include <app/product.h>

void product_get_name(barcode_t *upc, char * name) {
	/* Static product lookup for now */
	char * upc_id = upc->data;
	if(!strcmp(upc_id, "12000712999")) {
		strcpy(name, "PepsiCo Cup (22oz)");
	} else if(!strcmp(upc_id, "12000071768")) {
		strcpy(name, "Ocean Spray Strawberry Kiwi");
	} else if(!strcmp(upc_id, "39510800106")) {
		strcpy(name, "SOBE Green Tea");
	} else {
		strcpy(name, "Unknown product");
	}
}

/* Product functionality */
void product_init(product_t *product, rfid_t rfid, barcode_t upc, int do_nutrition_search)
{
	product->rfid = rfid;
	product->upc = upc;
	strncpy(product->name, "Unnamed Product", MAX_NAME_LEN);
	// Place product name request to lookup

	if(0 /* TODO: Nutrition lookup */) {
		nutrition_t *new_nutrition = (nutrition_t *)malloc(sizeof(nutrition_t));
		//nutrition_init(new_nutrition, upc);
		product->nutrition = new_nutrition;
	} else {
		product->nutrition = NULL;
	}
}

/* Nutrition functionality */
void nutrition_init(nutrition_t *nutrition, char * upc)
{

}
