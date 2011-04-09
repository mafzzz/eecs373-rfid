#include <stddef.h>
#include <stdlib.h>

#include <sys/utlist.h>
#include <sys/uthash.h>

#include "app/inventory.h"


static struct product_t *products_all = NULL;		// Hash table of all products
static struct product_t *products_stocked = NULL;	// Hash table of products currently stocked

void inventory_all_add(product_t *product)
{
	// Add product to 'all'
	HASH_ADD(hh_all, products_all, rfid_id, sizeof(int), product);
}

void inventory_stocked_toggle(product_t *product)
{
	struct product_t *product_result;
	HASH_FIND(hh_stocked, products_stocked, &(product->rfid_id), sizeof(uint32_t), product_result);
	if(product_result) {
		HASH_DELETE(hh_stocked, products_stocked, product_result);
	} else {
		HASH_ADD(hh_stocked, products_stocked, rfid_id, sizeof(uint32_t), product);
	}
}

product_t * inventory_stocked_lookup(uint32_t rfid_id)
{
	struct product_t *product_result;
	HASH_FIND(hh_stocked, products_stocked, &rfid_id, sizeof(uint32_t), product_result);
	return product_result;
}

product_t * inventory_all_lookup(uint32_t rfid_id)
{
	struct product_t *product_result;
	HASH_FIND(hh_all, products_all, &rfid_id, sizeof(uint32_t), product_result);
	return product_result;
}
