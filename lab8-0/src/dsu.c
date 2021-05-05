#include "dsu.h"


int dsu_find_lead(Dsu* array, int index) {
	if (array[array[index].parent].parent != array[index].parent ) {
		array[index].parent = dsu_find_lead(array, array[index].parent);
	}
	return array[index].parent;
}


int dsu_find_parents(Dsu* array, int from, int in) {
	array[from].parent = dsu_find_lead(array, from);
	array[in].parent = dsu_find_lead(array, in);
	if (array[from].parent != array[in].parent) {
		if (array[array[from].parent].weight <= array[array[in].parent].weight) {
			array[array[from].parent].parent = array[in].parent;
			array[array[in].parent].weight += array[array[from].parent].weight;
		}
		else {
			array[array[in].parent].parent = array[from].parent;
			array[array[from].parent].weight += array[array[in].parent].weight;
		}
		array[from].parent = array[array[from].parent].parent;
		array[in].parent = array[array[in].parent].parent;
		return 1;
	}
	return 0;
}
