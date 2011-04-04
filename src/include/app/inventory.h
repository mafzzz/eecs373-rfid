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

void inventory_add(product_t *product);
int inventory_set_presence(uint32_t upc, int presence);

#endif /* _INVENTORY_H */
