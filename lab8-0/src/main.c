#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>
#include "dsu.h"


typedef struct Answer {
	int first;
	int second;
}Answer;


typedef struct Edge {
	int from;
	int in;
	int weight;
}Edge;


typedef struct Graph {
	Dsu* array;
}Graph;


int dsu_comparator(const void* ptr_left, const void* ptr_right) {
	int left = ((Edge*)ptr_left)->weight;
	int right = ((Edge*)ptr_right)->weight;
	return left - right;
}


int check_parents(Dsu* array, Answer* array_ans, int n) {
	int parents = 0;
	for (int i = 0; i < n; i++) {
		if (array[i].parent == i)
			parents++;

		if (parents > 1) {
			return parents;
		}
	}
	for (int i = 0; i < n - 1; i++) {
		printf("%i %i\n", array_ans[i].first, array_ans[i].second);
	}
	return 1;
}


int pull_edges(Edge *edges, int m, int n) {
	int strings = 0;

	for (int i = 0; i < m; i++) {
		if (scanf("%i%i%i", &edges[i].from, &edges[i].in, &edges[i].weight) == EOF) {
			if (strings < m) {
				printf("bad number of lines");
			}
			return -1;
		}
		strings++;
		if (edges[i].from > n || edges[i].in > n || edges[i].from < 1 || edges[i].from >5000 || edges[i].in > 5000 || edges[i].in < 1) {
			printf("bad vertex");
			return -1;
		}
		if (edges[i].weight > INT_MAX || edges[i].weight < 0) {
			printf("bad length");
			return -1;
		}
		edges[i].from -= 1;
		edges[i].in -= 1;
	}
	qsort(edges, m, sizeof(Edge), dsu_comparator);

	return 0;
}


int main() {
	int n, m;
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
		graph.array[i].parent= i;
	}

	if (pull_edges(edges, m, n) != -1) {
		int edges_count = 0;
		for (int i = 0; i < m; i++) {
			if (dsu_find_parents(graph.array, edges[i].from, edges[i].in)) {
				if (edges[i].from < edges[i].in) {
					array_ans[edges_count].first = edges[i].from + 1;
					array_ans[edges_count++].second = edges[i].in + 1;
				}
				else {
					array_ans[edges_count].first = edges[i].in + 1;
					array_ans[edges_count++].second = edges[i].from + 1;
				}
			}
		}

		if (check_parents(graph.array, array_ans, n) != 1) {
			printf("no spanning tree");
		}
	}

	free(graph.array);
	free(edges);
	free(array_ans);
	return 0;
}
