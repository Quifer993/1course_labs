#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <limits.h>


enum Inf{ INFINITY = -2, DONE = -1};
enum TypeError { OK, LINES, VERTEX, LENGTH };


typedef struct Edge {
	int from;
	int length;
} Edge;


typedef struct Graph {
	int* matrix;
	int vertices;
	int edges;
} Graph;


int pop_matrix(const Graph* graph, int line, int column) {
	return graph->matrix[line * graph->vertices + column];
}


void put_matrix(Graph* graph, int line, int column, int weight) {
	graph->matrix[line * graph->vertices + column] = weight;
	graph->matrix[column * graph->vertices + line] = weight;
}


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
			int lenght_now = edges_ans[j].length;
			if (pop_matrix(graph, min.from, j) > 0 && lenght_now != DONE) {
				if (pop_matrix(graph, min.from, j) < lenght_now || lenght_now == INFINITY) {
					edges_ans[j].length = pop_matrix(graph, min.from, j) ;
					edges_ans[j].from = min.from;
				}
			}
		}
	
		min.length = INFINITY;
		min.from = 0;
		for (int i = 0; i < graph->vertices; i++) {
			int lenght_now = edges_ans[i].length;
			if ( (lenght_now > 0 && lenght_now < min.length) || (min.length == INFINITY && lenght_now > 0)) {
				if (lenght_now < edges_ans[min.from].length || min.from == 0) {
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


enum TypeError input_edges(FILE* test_file, Graph* graph) {
	int line;
	int column;
	int weight;
	for (int i = 0; i < graph->edges; i++) {
		if (fscanf(test_file, "%i%i%i", &line, &column, &weight) == EOF) {
			return LINES;
		}

		if (line < 1 || column < 1 || line > graph->vertices || column > graph->vertices) {
			return VERTEX;
		}

		if (weight > INT_MAX || weight < 0) {
			return LENGTH;
		}

		line -= 1;
		column -= 1;
		put_matrix(graph, line, column, weight);
	}
	return OK;
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

	int is_error = input_edges(file, &graph);
	if (is_error == OK) {
		if ( prim(&graph, edges_ans) ) {
			for (int i = 1; i < graph.vertices; i++) {
				printf("%i %i\n", edges_ans[i].from + 1, i + 1);
			}
		}
		else{
			printf("no spanning tree");
		}
	}	
	else if (is_error == LINES) {
		printf("bad number of lines");
	}
	else if (is_error == VERTEX) {
		printf("bad vertex");
	}
	else if (is_error == LENGTH) {
		printf("bad length");
	}

	free(graph.matrix);
	free(edges_ans);
	fclose(file);
	return 0;
}
