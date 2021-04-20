#ifndef FUNCTIONS_H
#define FUNCTIONS_H


typedef struct Dsu {
	int parent;
	int leader;
	int weight;
}Dsu;


typedef struct Answer{
	int first;
	int second;
}Answer;


int dsu_find_lead(Dsu* array, int index);
void dsu_sort(Dsu* array, int from, int in);



#endif //FUNCTIONS_H
