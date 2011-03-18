#include "device/disp.h"

int main()
{
	/* Initialize devices */
	disp_init();
	
	/* Start application */
	main_disp_test();
	
	while(1){};
}
