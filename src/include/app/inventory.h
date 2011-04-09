/*
 * inventory.h
 *
 *  Created on: Mar 21, 2011
 *      Author: Zach
 */
#ifndef _INVENTORY_H
#define _INVENTORY_H

#include "sys/linux_list.h"
#include "app/product.h"

/* Defined in inventory.c */
extern struct list_head g_inventory_contents;

void inventory_all_add(product_t *product);
void inventory_stocked_toggle(product_t *product);
product_t * inventory_all_lookup(uint32_t rfid_id);
product_t * inventory_stocked_lookup(uint32_t rfid_id);

#endif /* _INVENTORY_H */
