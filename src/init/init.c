#include "device/disp.h"

extern int main(void);
int main()
{
	/* Initialize devices */
	disp_init();
	
	/* Start application */
	main();
	
	while(1){};
}
