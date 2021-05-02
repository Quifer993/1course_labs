#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <limits.h>


enum Inf{ INFINITY = -2, MAKED = -1};


typedef struct Edge {
	int from;
	int length;
} Edge;


typedef struct Node {
	int value;
} Node;


typedef struct Graph {
	Node* array;
	int n;
	int m;
} Graph;


bool prim(const Graph* graph, Edge* edges_ans) {
	int count = 1;
	edges_ans[0].length = MAKED;
	edges_ans[0].from = 0;
	Edge min;
	min.length = INFINITY;
	min.from = 0;

	while (count < graph->n) {
		char used = 0;
		for (int j = 0; j < graph->n; j++) {
			if (graph->array[min.from * graph->n + j].value > 0 && edges_ans[j].length != MAKED) {
				if (graph->array[min.from * graph->n + j].value < edges_ans[j].length || edges_ans[j].length == INFINITY) {
					edges_ans[j].length = graph->array[min.from * graph->n + j].value;
					edges_ans[j].from = min.from;
				}
			}
		}
	
		min.length = INFINITY;
		min.from = 0;
		for (int i = 0; i < graph->n; i++) {
			if ( (edges_ans[i].length > 0 && edges_ans[i].length < min.length) || (min.length == INFINITY && edges_ans[i].length > 0)) {
				if (edges_ans[i].length < edges_ans[min.from].length || min.from == 0) {
					min.from = i;
				}
				used = 1;
			}
		}

		edges_ans[min.from].length = MAKED;
		if (used == 0) {
			return false;
		}
		count++;
	}

	return true;
}


bool input_edges(FILE* test_file, Graph* graph) {
	int line;
	int column;
	int weight;
	for (int i = 0; i < graph->m; i++) {
		if (fscanf(test_file, "%i%i%i", &line, &column, &weight) == EOF) {
			printf("bad number of lines");
			return 0;
		}

		if (line < 1 || column < 1 || line > graph->n || column > graph->n) {
			printf("bad vertex");
			return 0;
		}

		if (weight > INT_MAX || weight < 0) {
			printf("bad length");
			return 0;
		}

		line -= 1;
		column -= 1;

		graph->array[line * graph->n + column].value = weight;
		graph->array[column * graph->n + line].value = weight;
	}
	return true;
}


int main() {
	FILE* file;
	file = stdin;
	Graph graph; 

	if (fscanf(file, "%i", &graph.n) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (fscanf(file, "%i", &graph.m) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (graph.n <= 0 || graph.n > 5000) {
		if (graph.n == 0) {
			printf("no spanning tree");
		}
		else {
			printf("bad number of vertices");
		}
		return 0;

	}

	if (graph.m < 0 || graph.m > graph.n * (graph.n + 1) / 2) {
		printf("bad number of edges");
		fclose(file);
		return 0;
	}

	graph.array = (Node*)calloc(graph.n * graph.n, sizeof(Node));
	if (graph.array == NULL) {
		fclose(file);
		return 0;
	}

	Edge* edges_ans = (Edge*)malloc((graph.n) * sizeof(Edge));
	if (edges_ans == NULL) {
		free(graph.array);
		fclose(file);
		return 0;
	}
	for (int i = 0; i < graph.n; i++) {
		edges_ans[i].length = INFINITY;
	}

	if ( input_edges(file, &graph) ) {
		if ( prim(&graph, edges_ans) ) {
			for (int i = 1; i < graph.n; i++) {
				printf("%i %i\n", edges_ans[i].from + 1, i + 1);
			}
		}
		else{
            printf("no spanning tree");
		}
	}	

	free(graph.array);
	free(edges_ans);
	fclose(file);
	return 0;
}
