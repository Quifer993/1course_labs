#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "dsu.h"


enum TypeError { OK, LINES, VERTEX, LENGTH };


typedef struct Edge {
	int from;
	int to;
	int weight;
} Edge;


int compare_edges(const void* ptr_left, const void* ptr_right) {
	int left = ((Edge const*)ptr_left)->weight;
	int right = ((Edge const *)ptr_right)->weight;
	return left - right;
}


int check_parents(Dsu* dsu_vertices, int n) {
	int parents = 0;
	for (int i = 0; i < n; i++) {
		if (dsu_vertices[i].parent == i) {
			parents++;
		}

		if (parents > 1) {
			return parents;
		}
	}
	return 1;
}


int pull_edges(Edge *edges, int m, int n) {
	for (int i = 0; i < m; i++) {
		if (scanf("%i%i%i", &edges[i].from, &edges[i].to, &edges[i].weight) == EOF) {
			return LINES;
		}
		if (edges[i].from > n || edges[i].to > n || edges[i].from < 1 || edges[i].from >5000 || edges[i].to > 5000 || edges[i].to < 1) {
			return VERTEX;
		}
		if (edges[i].weight > INT_MAX || edges[i].weight < 0) {
			return LENGTH;
		}
		edges[i].from -= 1;
		edges[i].to -= 1;
	}

	return OK;
}


Edge* alg_kraskal(Dsu* dsu_vertices, Edge* edges, int n, int m) {
	qsort(edges, m, sizeof(Edge), compare_edges);
	int edges_count = 0;

	Edge* edges_ans = (Edge*)malloc((n - 1) * sizeof(Edge));
	if (edges_ans == NULL) {
		return NULL;
	}

	for (int i = 0; i < m; i++) {
		if (dsu_find_parents(dsu_vertices, edges[i].from, edges[i].to)) {
			edges_ans[edges_count].from = edges[i].from;
			edges_ans[edges_count++].to = edges[i].to;
		}
	}

	if (check_parents(dsu_vertices, n) != 1) {
		free(edges_ans);
		edges_ans = NULL;
	}

	return edges_ans;
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
	if (edges == NULL) {
		return 0;
	}

	Dsu* dsu_vertices = (Dsu*)malloc(n * sizeof(Dsu));
	if (dsu_vertices == NULL) {
		free(edges);
		return 0;
	}

	for (int i = 0; i < n; i++) {
		dsu_vertices[i].weight = 1;
		dsu_vertices[i].parent= i;
	}

	int is_error = pull_edges(edges, m, n);
	if (is_error == OK) {
		Edge* edges_answer = alg_kraskal(dsu_vertices, edges, n, m);
		if (edges_answer == NULL) {
			printf("no spanning tree");
		}
		else {
			for (int i = 0; i < n - 1; i++) {
				printf("%i %i\n", edges_answer[i].from + 1, edges_answer[i].to + 1);
			}
			free(edges_answer);
		}
	}
	else if(is_error ==	LINES){
		printf("bad number of lines");
	}
	else if(is_error == VERTEX){
		printf("bad vertex");
	}
	else if(is_error == LENGTH){
		printf("bad length");
	}

	free(dsu_vertices);
	free(edges);
	return 0;
}
