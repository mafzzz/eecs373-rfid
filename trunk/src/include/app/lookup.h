/*
 * lookup.h
 *
 *  Created on: Apr 9, 2011
 *      Author: Zach
 */

#ifndef LOOKUP_H_
#define LOOKUP_H_

#include "app/product.h"

int lookup_barcode(char * result, char * barcode);
int lookup_nutrition(nutrition_t * nut);


#endif /* LOOKUP_H_ */
