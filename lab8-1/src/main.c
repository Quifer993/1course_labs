#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <limits.h>


enum Inf{ INFINITY = -2, DONE = -1};
enum TypeError{ INFINITY = 0, DONE = -1};


typedef struct Edge {
	int from;
	int length;
} Edge;


typedef struct Node {
	int value;
} Node;


typedef struct Graph {
	int* matrix;
	int vertices;
	int edges;
} Graph;


bool prim(const Graph* graph, Edge* edges_ans) {
	int count = 1;
	edges_ans[0].length = DONE;
	edges_ans[0].from = 0;
	Edge min;
	min.length = INFINITY;
	min.from = 0;

	while (count < graph->vertices) {
		bool used = false;
		for (int j = 0; j < graph->vertices; j++) {
			if (graph->matrix[min.from * graph->vertices + j] > 0 && edges_ans[j].length != DONE) {
				if (graph->matrix[min.from * graph->vertices + j] < edges_ans[j].length || edges_ans[j].length == INFINITY) {
					edges_ans[j].length = graph->matrix[min.from * graph->vertices + j] ;
					edges_ans[j].from = min.from;
				}
			}
		}
	
		min.length = INFINITY;
		min.from = 0;
		for (int i = 0; i < graph->vertices; i++) {
			if ( (edges_ans[i].length > 0 && edges_ans[i].length < min.length) || (min.length == INFINITY && edges_ans[i].length > 0)) {
				if (edges_ans[i].length < edges_ans[min.from].length || min.from == 0) {
					min.from = i;
				}
				used = true;
			}
		}

		edges_ans[min.from].length = DONE;
		if (used == false) {
			return used;
		}
		count++;
	}

	return true;
}


void put_to_matrix(Graph* graph,int line, int column, int weight) {
	graph->matrix[line * graph->vertices + column] = weight;
	graph->matrix[column * graph->vertices + line] = weight;
}


bool input_edges(FILE* test_file, Graph* graph) {
	int line;
	int column;
	int weight;
	for (int i = 0; i < graph->edges; i++) {
		if (fscanf(test_file, "%i%i%i", &line, &column, &weight) == EOF) {
			printf("bad number of lines");
			return false;
		}

		if (line < 1 || column < 1 || line > graph->vertices || column > graph->vertices) {
			printf("bad vertex");
			return false;
		}

		if (weight > INT_MAX || weight < 0) {
			printf("bad length");
			return false;
		}

		line -= 1;
		column -= 1;
		put_to_matrix(graph, line, column, weight);
	}
	return true;
}


int main() {
	FILE* file;
	file = stdin;
	Graph graph; 

	if (fscanf(file, "%i", &graph.vertices) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (fscanf(file, "%i", &graph.edges) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (graph.vertices <= 0 || graph.vertices > 5000) {
		if (graph.vertices == 0) {
			printf("no spanning tree");
		}
		else {
			printf("bad number of vertices");
		}
		return 0;

	}

	if (graph.edges < 0 || graph.edges > graph.vertices * (graph.vertices + 1) / 2) {
		printf("bad number of edges");
		fclose(file);
		return 0;
	}

	graph.matrix = (int*)calloc(graph.vertices * graph.vertices, sizeof(int));
	if (graph.matrix == NULL) {
		fclose(file);
		return 0;
	}

	Edge* edges_ans = (Edge*)malloc((graph.vertices) * sizeof(Edge));
	if (edges_ans == NULL) {
		free(graph.matrix);
		fclose(file);
		return 0;
	}
	for (int i = 0; i < graph.vertices; i++) {
		edges_ans[i].length = INFINITY;
	}

	if ( input_edges(file, &graph) ) {
		if ( prim(&graph, edges_ans) ) {
			for (int i = 1; i < graph.vertices; i++) {
				printf("%i %i\n", edges_ans[i].from + 1, i + 1);
			}
		}
		else{
			printf("no spanning tree");
		}
	}	

	free(graph.matrix);
	free(edges_ans);
	fclose(file);
	return 0;
}
