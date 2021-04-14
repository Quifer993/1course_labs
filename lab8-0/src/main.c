#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <limits.h>
#include "dsu.h"


typedef struct Graph {
	Dsu* array;
}Graph;


int dsu_comparator(const void* ptr_left, const void* ptr_right) {
	int left = ((Edge*)ptr_left)->weight;
	int right = ((Edge*)ptr_right)->weight;
	return left - right;
}


int dsu_find_lead(Dsu* array, int index) {
	if (array[array[index].leader].leader != array[index].leader) {
		array[index].leader = dsu_find_lead(array, array[index].leader);
	}
	return array[index].leader;
}


void dsu_sort(Dsu* array, Edge* edges, Answer* array_ans, int n, int m) {
	int strings = 0;

	for (int i = 0; i < m; i++) {
		if (scanf("%i%i%i", &edges[i].from, &edges[i].in, &edges[i].weight) == EOF) {
			if (strings < m) {
				printf("bad number of lines");
			}
			return;
		}
		strings++;
		if (edges[i].from > n || edges[i].in > n || edges[i].from < 1 || edges[i].from >5000 || edges[i].in > 5000 || edges[i].in < 1) {
			printf("bad vertex");
			return;
		}
		if (edges[i].weight > INT_MAX || edges[i].weight < 0) {
			printf("bad length");
			return;
		}
		edges[i].from -= 1;
		edges[i].in -= 1;
	}
	qsort(edges, m, sizeof(Edge), dsu_comparator);

	int edges_count = 0;
	for (int i = 0; i < m; i++) {
		array[edges[i].from].leader = dsu_find_lead(array, edges[i].from);
		array[edges[i].in].leader = dsu_find_lead(array, edges[i].in);
		if (array[edges[i].from].leader != array[edges[i].in].leader) {
			if (edges[i].from < edges[i].in) {
				array_ans[edges_count].first = edges[i].from + 1;
				array_ans[edges_count++].second = edges[i].in + 1;
			}
			else {
				array_ans[edges_count].first = edges[i].in + 1;
				array_ans[edges_count++].second = edges[i].from + 1;
			}

			if (array[array[edges[i].from].leader].weight <= array[array[edges[i].in].leader].weight) {
				array[array[edges[i].from].leader].leader = array[edges[i].in].leader;
				array[array[edges[i].in].leader].weight += array[array[edges[i].from].leader].weight;
			}
			else {
				array[array[edges[i].in].leader].leader = array[edges[i].from].leader;
				array[array[edges[i].from].leader].weight += array[array[edges[i].in].leader].weight;
			}
			array[edges[i].from].leader = array[array[edges[i].from].leader].leader;
			array[edges[i].in].leader = array[array[edges[i].in].leader].leader;
		}
	}

	int leaders = 0;
	for (int i = 0; i < n; i++) {
		if (array[i].leader == i)
			leaders++;

		if (leaders > 1) {
			printf("no spanning tree");
			return;
		}
	}
	for (int i = 0; i < n - 1; i++) {
		printf("%i %i\n", array_ans[i].first, array_ans[i].second);
	}
}


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
