#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#include <stddef.h>

#define container_of(ptr, type, member) ({ \
                const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
                (type *)( (char *)__mptr - offsetof(type,member) );})

/*
 * Architectures might want to move the poison pointer offset into some
 * well-recognized area such as 0xdead000000000000, that is also not mappable by
 * user-space exploits:
 */
#define POISON_POINTER_DELTA 0x0

/*
 * These are non-NULL pointers that will result in page faults under normal
 * circumstances, used to verify that nobody uses non-initialized list entries.
 */
#define LIST_POISON1  ((struct list_head *) 0x00100100 + POISON_POINTER_DELTA)
#define LIST_POISON2  ((struct list_head *) 0x00200200 + POISON_POINTER_DELTA)

/**
 * @ingroup list
 * Simple doubly linked list implementation.
 */
struct list_head {
	/** pointer to the next element */
	struct list_head *next;
	/** pointer to the previous element */
	struct list_head *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

/**
 * @ingroup list
 * 
 * Declares and initializes a list head
 */
#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __list_add(struct list_head *new,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/**
 * @ingroup list
 * @fn inline void list_add(struct list_head *new, struct list_head *head)
 * @brief add a new list entry
 * @param new new entry to be added
 * @param head list head to add it after
 *
 * Insert a new entry after the specified head.  This is good for implementing
 * stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}


/**
 * @ingroup list
 * @fn inline void list_add_tail(struct list_head *new, struct list_head *head)
 * @brief add a new entry
 * @param new new entry to be added
 * @param head list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * @ingroup list
 * @fn inline void list_del(struct list_head *entry) 
 * @brief deletes entry from list.
 * @param entry the element to delete from the list.
 * @note list_empty() on entry does not return true after this, the entry is
 *   in an undefined state.
 */
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}

/**
 * @ingroup list
 * @fn inline void list_replace(struct list_head *old, struct list_head *new)
 * @brief replace old entry by new one
 * @param old the element to be replaced
 * @param new the new element to insert
 *
 * If old was empty, it will be overwritten.
 */
static inline void list_replace(struct list_head *old,
				struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline void list_replace_init(struct list_head *old,
					struct list_head *new)
{
	list_replace(old, new);
	INIT_LIST_HEAD(old);
}

/**
 * @ingroup list
 * @fn inline void list_del_init(struct list_head *entry)
 * @brief deletes entry from list and reinitialize it.
 * @param entry the element to delete from the list.
 */
static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}

/**
 * @ingroup list
 * @fn inline void list_move(struct list_head *list, struct list_head *head)
 * @brief delete from one list and add as another's head
 * @param list the entry to move
 * @param head the head that will precede our entry
 */
static inline void list_move(struct list_head *list, struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add(list, head);
}

/**
 * @ingroup list
 * @fn inline void list_move_tail(struct list_head *list, struct list_head *head)
 * @brief delete from one list and add as another's tail
 * @param list the entry to move
 * @param head the head that will follow our entry
 */
static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
	__list_del(list->prev, list->next);
	list_add_tail(list, head);
}

/**
 * @ingroup list
 * @fn inline int list_is_last(const struct list_head *list, const struct
 *   list_head *head)
 * @brief tests whether list is the last entry in list head
 * @param list the entry to test
 * @param head the head of the list
 */
static inline int list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}

/**
 * @ingroup list
 * @fn inline int list_empty(const struct list_head *head)
 * @brief tests whether a list is empty
 * @param head the list to test.
 */
static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}

/**
 * @ingroup list
 * @fn inline int list_empty_careful(const struct list_head *head) 
 * @brief tests whether a list is empty and not being modified
 * @param head the list to test
 * @note using list_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is list_del_init(). Eg. it cannot be used
 * if another CPU could re-list_add() it.
 *
 * tests whether a list is empty _and_ checks that no other CPU might be in the
 * process of modifying either member (next or prev)
 *
 */
static inline int list_empty_careful(const struct list_head *head)
{
	struct list_head *next = head->next;
	return (next == head) && (next == head->prev);
}

/**
 * @ingroup list
 * @fn inline int list_is_singular(const struct list_head *head)
 * @brief tests whether a list has just one entry.
 * @param head the list to test.
 */
static inline int list_is_singular(const struct list_head *head)
{
	return !list_empty(head) && (head->next == head->prev);
}

static inline void __list_cut_position(struct list_head *list,
		struct list_head *head, struct list_head *entry)
{
	struct list_head *new_first = entry->next;
	list->next = head->next;
	list->next->prev = list;
	list->prev = entry;
	entry->next = list;
	head->next = new_first;
	new_first->prev = head;
}

/**
 * @ingroup list
 * @fn inline void list_cut_position(struct list_head *list, struct list_head
 *   *head, struct list_head *entry) 
 * @brief cut a list into two
 * @param list a new list to add all removed entries
 * @param head a list with entries
 * @param entry an entry within head, could be the head itself
 *	and if so we won't cut the list
 *
 * This helper moves the initial part of head, up to and
 * including entry, from head to list. You should
 * pass on entry an element you know is on head. list
 * should be an empty list or a list you do not care about
 * losing its data.
 *
 */
static inline void list_cut_position(struct list_head *list,
		struct list_head *head, struct list_head *entry)
{
	if (list_empty(head))
		return;
	if (list_is_singular(head) &&
		(head->next != entry && head != entry))
		return;
	if (entry == head)
		INIT_LIST_HEAD(list);
	else
		__list_cut_position(list, head, entry);
}

static inline void __list_splice(const struct list_head *list,
				 struct list_head *prev,
				 struct list_head *next)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

/**
 * @ingroup list
 * @fn inline void list_splice(const struct list_head *list, struct list_head
 *   *head)
 * @brief join two lists, this is designed for stacks
 * @param list the new list to add.
 * @param head the place to add it in the first list.
 */
static inline void list_splice(const struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head, head->next);
}

/**
 * @ingroup list
 * @fn inline void list_splice_tail(struct list_head *list, struct list_head
 *   *head)
 * @brief join two lists, each list being a queue
 * @param list the new list to add.
 * @param head the place to add it in the first list.
 */
static inline void list_splice_tail(struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head->prev, head);
}

/**
 * @ingroup list
 * @fn inline void list_splice_init(struct list_head *list, struct list_head
 *   *head)
 * @brief join two lists and reinitialise the emptied list.
 * @param list the new list to add.
 * @param head the place to add it in the first list.
 *
 * The list at list is reinitialised
 */
static inline void list_splice_init(struct list_head *list,
				    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head, head->next);
		INIT_LIST_HEAD(list);
	}
}

/**
 * @ingroup list
 * @fn inline void list_splice_tail_init(struct list_head *list, struct
 *   list_head *head)
 * @brief join two lists and reinitialise the emptied list
 * @param list the new list to add.
 * @param head the place to add it in the first list.
 *
 * Each of the lists is a queue.
 * The list at list is reinitialised
 */
static inline void list_splice_tail_init(struct list_head *list,
					 struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head->prev, head);
		INIT_LIST_HEAD(list);
	}
}

/**
 * @ingroup list
 * @def list_entry(ptr, type, member)
 * @brief get the struct for this entry
 * @param ptr	the &struct list_head pointer.
 * @param type the type of the struct this is embedded in.
 * @param member the name of the list_struct within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * @ingroup list
 * @def list_first_entry(ptr, type, member)
 * @brief get the first element from a list
 * @param ptr the list head to take the element from.
 * @param type the type of the struct this is embedded in.
 * @param member the name of the list_struct within the struct.
 * @note this list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

/**
 * @ingroup list
 * @def list_for_each(pos, head)
 * @brief iterate over a list
 * @param pos the &struct list_head to use as a loop cursor.
 * @param head the head for your list.
 */
#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * @ingroup list
 * @def list_for_each_prev(pos, head)
 * @brief iterate over a list backwards
 * @param pos the &struct list_head to use as a loop cursor.
 * @param head the head for your list.
 */
#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); \
        	pos = pos->prev)

/**
 * @ingroup list
 * @def list_for_each_safe(pos, n, head)
 * @brief iterate over a list safe against removal of list entry
 * @param pos the &struct list_head to use as a loop cursor.
 * @param n another &struct list_head to use as temporary storage
 * @param head the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * @ingroup list
 * @def list_for_each_prev_safe(pos, n, head)
 * @brief iterate over a list backwards safe against removal of list entry
 * @param pos the &struct list_head to use as a loop cursor.
 * @param n another &struct list_head to use as temporary storage
 * @param head the head for your list.
 */
#define list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)

/**
 * @ingroup list
 * @def list_for_each_entry(pos, head, type, member)
 * @brief iterate over list of given type
 * @param pos the type * to use as a loop cursor.
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, type, member)				\
	for (pos = list_entry((head)->next, type, member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, type, member))

/**
 * @ingroup list
 * @def list_for_each_entry_reverse(pos, head, type, member)
 * @brief iterate backwards over list of given type.
 * @param pos the type * to use as a loop cursor.
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 */
#define list_for_each_entry_reverse(pos, head, type, member)			\
	for (pos = list_entry((head)->prev, type, member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.prev, type, member))

/**
 * @ingroup list
 * @def list_prepare_entry(pos, head, type, member)
 * @brief prepare a pos entry for use in list_for_each_entry_continue()
 * @param pos the type * to use as a start point
 * @param head the head of the list
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 *
 * Prepares a pos entry for use as a start point in
 * list_for_each_entry_continue().
 */
#define list_prepare_entry(pos, head, type, member) \
	((pos) ? : list_entry(head, type, member))

/**
 * @ingroup list
 * @def list_for_each_entry_continue(pos, head, type, member)
 * @brief continue iteration over list of given type
 * @param pos the type * to use as a loop cursor.
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 *
 * Continue to iterate over list of given type, continuing after the current
 * position.
 */
#define list_for_each_entry_continue(pos, head, type, member) 		\
	for (pos = list_entry(pos->member.next, type, member);	\
	     &pos->member != (head);	\
	     pos = list_entry(pos->member.next, type, member))

/**
 * @ingroup list
 * @def list_for_each_entry_continue_reverse(pos, head, type, member)
 * @brief iterate backwards from the given point
 * @param pos the type * to use as a loop cursor.
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 *
 * Start to iterate over list of given type backwards, continuing after the
 * current position.
 */
#define list_for_each_entry_continue_reverse(pos, head, type, member)		\
	for (pos = list_entry(pos->member.prev, type, member);	\
	     &pos->member != (head);	\
	     pos = list_entry(pos->member.prev, type, member))

/**
 * @ingroup list
 * @def list_for_each_entry_from(pos, head, type, member)
 * @brief iterate over list of given type from the current point
 * @param pos the type * to use as a loop cursor.
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */
#define list_for_each_entry_from(pos, head, type, member) 			\
	for (; &pos->member != (head);	\
	     pos = list_entry(pos->member.next, type, member))

/**
 * @ingroup list
 * @def list_for_each_entry_safe(pos, n, head, type, member)
 * @brief iterate over list of given type safe against removal of list entry
 * @param pos the type * to use as a loop cursor.
 * @param n another type * to use as temporary storage
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, type, member)			\
	for (pos = list_entry((head)->next, type, member),	\
		n = list_entry(pos->member.next, type, member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, type, member))

/**
 * @ingroup list
 * @def list_for_each_entry_safe_continue(pos, n, head, type, member)
 * @param pos the type * to use as a loop cursor.
 * @param n another type * to use as temporary storage
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 *
 * Iterate over list of given type, continuing after current point, safe against
 * removal of list entry.
 */
#define list_for_each_entry_safe_continue(pos, n, head, type, member) 		\
	for (pos = list_entry(pos->member.next, type, member), 		\
		n = list_entry(pos->member.next, type, member);		\
	     &pos->member != (head);						\
	     pos = n, n = list_entry(n->member.next, type, member))

/**
 * @ingroup list
 * @def list_for_each_entry_safe_from(pos, n, head, type member)
 * @param pos the type * to use as a loop cursor.
 * @param n another type * to use as temporary storage
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 *
 * Iterate over list of given type from current point, safe against removal of
 * list entry.
 */
#define list_for_each_entry_safe_from(pos, n, head, type, member) 			\
	for (n = list_entry(pos->member.next, type, member);		\
	     &pos->member != (head);						\
	     pos = n, n = list_entry(n->member.next, type, member))

/**
 * @ingroup list
 * @def list_for_each_entry_safe_reverse(pos, n, head, type, member)
 * @param pos the type * to use as a loop cursor.
 * @param n another type * to use as temporary storage
 * @param head the head for your list.
 * @param type type of *pos
 * @param member the name of the list_struct within the struct.
 *
 * Iterate backwards over list of given type, safe against removal of list
 * entry.
 */
#define list_for_each_entry_safe_reverse(pos, n, head, type, member)		\
	for (pos = list_entry((head)->prev, type, member),	\
		n = list_entry(pos->member.prev, type, member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.prev, type, member))


#endif

