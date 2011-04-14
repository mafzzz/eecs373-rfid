#include <inttypes.h>

#ifndef KEYPAD_H_
#define KEYPAD_H_

typedef struct keypad_t {
	union {
		struct keys {
			unsigned long key1 		: 1;
			unsigned long key_2		: 1;
			unsigned long key_3		: 1;
			unsigned long key_a		: 1;
			unsigned long key_4		: 1;
			unsigned long key_5		: 1;
			unsigned long key_6		: 1;
			unsigned long key_b		: 1;
			unsigned long key_7		: 1;
			unsigned long key_8		: 1;
			unsigned long key_9		: 1;
			unsigned long key_c		: 1;
			unsigned long key_star	: 1;
			unsigned long key_0		: 1;
			unsigned long key_pound	: 1;
			unsigned long key_d		: 1;
		} keys;
		uint32_t val;
	};
} keypad_t;

keypad_t keypad_read();
void keypad_task();

#endif
