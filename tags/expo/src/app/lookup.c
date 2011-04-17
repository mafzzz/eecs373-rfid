#include "app/lookup.h"

void lookup_task() {
	// Wait for lookup requests
//	uint32_t servings_total;
//	uint32_t servings_remaining;
//	uint32_t cals_per_serving;
//	/* Below units in milligrams */
//	uint32_t fat_per_serving;
//	uint32_t satfat_per_serving;
//	uint32_t transfat_per_serving;
//	uint32_t cholesterol_per_serving;
//	uint32_t sodium_per_serving;
//	uint32_t carbs_per_serving;
//	uint32_t fiber_per_serving;
//	uint32_t sugar_per_serving;
//	uint32_t protein_per_serving;

}


int lookup_barcode(char * name, char * barcode) {
	strcpy(name, "SOME_PRODUCT");
	return 0;
}

int lookup_nutrition(nutrition_t * nut) {
	return -1;
}
