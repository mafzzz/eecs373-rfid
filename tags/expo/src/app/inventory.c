#include <stddef.h>
#include <stdlib.h>

#include <sys/utlist.h>
#include <sys/uthash.h>

#include <sys/FreeRTOS.h>
#include <sys/semphr.h>

#include "app/inventory.h"

#include "app/app.h"

xSemaphoreHandle inventory_sem;
#define LOCK_INVENTORY() xSemaphoreTake(inventory_sem, portMAX_DELAY)
#define UNLOCK_INVENTORY() xSemaphoreGive(inventory_sem)

struct product_t *inventory_products_all = NULL;		// Hash table of all products
struct product_t *inventory_products_stocked = NULL;	// Hash table of products currently stocked

void inventory_all_add(product_t *product)
{
	LOCK_INVENTORY();
	// Add product to 'all'
	HASH_ADD(hh_all, inventory_products_all, rfid, RFID_LEN, product);
	UNLOCK_INVENTORY();
}

int inventory_all_count() {
	int count;
	LOCK_INVENTORY();
	count = HASH_CNT(hh_all, inventory_products_all);
	UNLOCK_INVENTORY();
	return count;
}

int inventory_stocked_count() {
	int count;
	LOCK_INVENTORY();
	count = HASH_CNT(hh_stocked, inventory_products_stocked);
	UNLOCK_INVENTORY();
	return count;
}

int inventory_stocked_toggle(product_t *product)
{
	int res;
	struct product_t *product_result;

	LOCK_INVENTORY();
	HASH_FIND(hh_stocked, inventory_products_stocked, &(product->rfid), RFID_LEN, product_result);
	if(product_result) {
		HASH_DELETE(hh_stocked, inventory_products_stocked, product_result);
		res = 0;
	} else {
		HASH_ADD(hh_stocked, inventory_products_stocked, rfid, RFID_LEN, product);
		res = 1;
	}
	UNLOCK_INVENTORY();
	return res;
}

product_t * inventory_stocked_lookup(rfid_t *rfid)
{
	LOCK_INVENTORY();
	struct product_t *product_result;
	HASH_FIND(hh_stocked, inventory_products_stocked, rfid->data, RFID_LEN, product_result);
	return product_result;
	UNLOCK_INVENTORY();
}

product_t * inventory_all_lookup(rfid_t *rfid)
{
	struct product_t *product_result;

	LOCK_INVENTORY();
	HASH_FIND(hh_all, inventory_products_all, rfid->data, RFID_LEN, product_result);
	UNLOCK_INVENTORY();
	return product_result;
}

void inventory_init() {
	vSemaphoreCreateBinary( inventory_sem );
}
