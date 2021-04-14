#ifndef FUNCTIONS_H
#define FUNCTIONS_H


typedef struct Edge {
	int from;
	int in;
	int weight;
}Edge;


typedef struct Dsu {
	int parent;
	int leader;
	int weight;
}Dsu;


typedef struct Answer{
	int first;
	int second;
}Answer;


int dsu_comparator(const void* ptr_left, const void* ptr_right);
int dsu_find_lead(Dsu* array, int index);
void dsu_sort(Dsu* array, Edge* edges, Answer* array_ans, int n, int m);



#endif //FUNCTIONS_H
