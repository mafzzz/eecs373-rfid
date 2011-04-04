#include <stddef.h>

#include "sys/linux_list.h"
#include "app/inventory.h"


LIST_HEAD(g_inventory_contents);

void inventory_add(product_t *product)
{
	list_add(&(product->inventory_list), &g_inventory_contents);
}
