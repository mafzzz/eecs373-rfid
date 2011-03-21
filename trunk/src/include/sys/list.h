/* Generic linked link
 * Author: zrenner
 * Date: 3/21/2011
 */
#include <stddef.h>

typedef struct item_t {
	void 			*data;
	struct item_t 	*next;
	struct item_t 	*prev;
} item_t;

typedef struct list_t {
	size_t 			item_size;
	struct item_t 	*head;
	struct item_t	*tail;
} list_t;

void list_init(list_t * list, size_t size)
{
	list->item_size = size;
	list->head = NULL;
}

void list_insert_head(list_t * list, item_t * item)
{
	item_t *prev_head = list->head;
	if(prev_head) {
		prev_head->prev = item;
	}
	item->next = prev_head;
	item->prev = NULL;
	list->head = item;
}

void list_insert_tail(list_t * list, item_t * item)
{
	item_t *prev_tail = list->tail;
	if(prev_tail) {
		prev_tail->prev = item;
	}
	item->next = NULL;
	item->prev = prev_tail;
	list->tail = item;
}

#define list_for_each(list, cur)) \
	for (cur = list->head; cur != list->tail; cur = cur->next)

#define list_for_each_safe(list, cur, n) \
	for (cur = list->head, n = cur->next; cur != list->tail; cur = n, n = cur->next)

#define list_data(item, type) \
	(type *) item->data
