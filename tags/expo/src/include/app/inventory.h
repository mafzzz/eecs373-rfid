/*
 * inventory.h
 *
 *  Created on: Mar 21, 2011
 *      Author: Zach
 */
#ifndef _INVENTORY_H
#define _INVENTORY_H

#include "sys/FreeRTOS.h"
#include <sys/semphr.h>
#include "sys/linux_list.h"
#include "app/product.h"
#include "app/app.h"

/* Defined in inventory.c */
extern struct list_head g_inventory_contents;
extern xSemaphoreHandle inventory_sem;
extern product_t * inventory_products_all;
extern product_t * inventory_products_stocked;

#define LOCK_INVENTORY() xSemaphoreTake(inventory_sem, portMAX_DELAY)
#define UNLOCK_INVENTORY() xSemaphoreGive(inventory_sem)

void inventory_all_add(product_t *product);
int inventory_stocked_toggle(product_t *product);
product_t * inventory_all_lookup(rfid_t *rfid_id);
product_t * inventory_stocked_lookup(rfid_t *rfid_id);
int inventory_all_count();
int inventory_stocked_count();


#endif /* _INVENTORY_H */
