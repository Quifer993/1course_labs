#ifndef FUNCTIONS_H
#define FUNCTIONS_H


typedef struct Dsu {
	int parent;
	int weight;
}Dsu;


int dsu_find_lead(Dsu* array, int index);
int dsu_find_parents(Dsu* array, int from, int in);



#endif //FUNCTIONS_H
