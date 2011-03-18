/* Graphical display test
 * Author: zrenner
 * Date: 3/13/2011 */

#include "device/disp.h"
#include <inttypes.h>

int main_disp_test() {
	/* Initialize UART */

	disp_clear();


	while(1) {
		int i,j;
		for(i = 0; i < 100; i++) {
			for(j = 0; j < 50000; j++) {
				/* Wait */
			}
			disp_printf("%s", "This is a test");
		}

	}

	return 0;
}
