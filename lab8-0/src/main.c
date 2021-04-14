#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "dsu.h"


typedef struct Graph {
	Dsu* array;
}Graph;


int main() {
	int n;
	int m;
	if (scanf("%i%i", &n , &m) == EOF) {
		printf("bad number of lines");
		return 0;
	}

	if (n <= 0 || n > 5000) {
		if (n == 0) {
			printf("no spanning tree");
		}
		else {
			printf("bad number of vertices");
		}
		return 0;
	}


	if (m < 0 || m > n * (n + 1) / 2) {
		printf("bad number of edges");
		return 0;
	}

	Edge* edges = (Edge*)malloc(m * sizeof(Edge));
	Answer* array_ans = (Answer*)malloc((n - 1) * sizeof(Answer));

	Graph graph;
	graph.array = (Dsu*)malloc(n * sizeof(Dsu));
	for (int i = 0; i < n; i++) {
		graph.array[i].weight = 1;
		graph.array[i].leader = i;
		graph.array[i].parent= i;
	}

	dsu_sort(graph.array, edges, array_ans, n, m);

	free(graph.array);
	free(edges);
	free(array_ans);
	return 0;
}
