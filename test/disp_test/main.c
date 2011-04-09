/* Graphical display test
 * Author: zrenner
 * Date: 3/13/2011 */

#include "device/disp.h"
#include <inttypes.h>

int main() {
	disp_init();
	disp_init();
	disp_clear();


	while(1) {
		int i,j;
		for(i = 0; i < 100; i++) {
			for(j = 0; j < 5000000; j++) {
				/* Wait */
			}
			disp_printf("%s", "TEST ");
		}

	}

	return 0;
}
