#include <stdio.h>
#include <stdlib.h>
#include "dsu.h"


int dsu_find_lead(Dsu* array, int index) {
	if (array[array[index].leader].leader != array[index].leader ) {
		array[index].leader = dsu_find_lead(array, array[index].leader);
	}
	return array[index].leader;
}


int dsu_sort(Dsu* array, int from, int in) {

	array[from].leader = dsu_find_lead(array, from);
	array[in].leader = dsu_find_lead(array, in);
	if (array[from].leader != array[in].leader) {


		if (array[array[from].leader].weight <= array[array[in].leader].weight) {
			array[array[from].leader].leader = array[in].leader;
			array[array[in].leader].weight += array[array[from].leader].weight;
		}
		else {
			array[array[in].leader].leader = array[from].leader;
			array[array[from].leader].weight += array[array[in].leader].weight;
		}
		array[from].leader = array[array[from].leader].leader;
		array[in].leader = array[array[in].leader].leader;

		return 1;
	}
	return 0;
}
