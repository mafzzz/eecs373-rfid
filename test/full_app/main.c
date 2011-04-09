#include "sys/app.h"
#include <stdio.h>

int main()
{
	printf("Main starting\r\n");
	/* Run system init */
	init();

	/* init should never return */
	return 0;
}

