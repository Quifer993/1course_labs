#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>


enum Color { WHITE, GRAY, BLACK };


typedef struct Node {
	char value;
} Node;


typedef struct Graph {
	Node* matrix;
	int vertices;
	int edges;
} Graph;


bool check_line(char* colors, const Graph* graph, int* pointer, int line, int* answer) {
	char is_cycle = false;
	if (colors[line] != GRAY) {
		for (int j = 0; j < graph->vertices && !is_cycle && graph->matrix[line * graph->vertices + j].value == 1; j++ ) {
			if (colors[j] != BLACK) {
				colors[line] = GRAY;
				is_cycle = check_line(colors, graph, pointer, j, answer);
			}
		}
	}
	else {
		is_cycle = true;
	}
	colors[line] = BLACK;
	if (*pointer >= 0 ) {
		answer[(*pointer)--] = line + 1;
	}
	return is_cycle;
}


bool top_sort(const Graph* graph, int* answer) {
	char is_cycle = false;

	char* colors = (char*)calloc(graph->vertices, sizeof(char));
	if (colors == NULL) {
		return false;
	}

	int pointer = graph->vertices - 1;
	for (int i = 0; i < graph->vertices && !is_cycle; i++) {
		if (colors[i] == WHITE) {
			is_cycle = check_line(colors, graph, &pointer, i, answer);
		}
	}
	free(colors);
	return is_cycle;
}


bool input_edges(FILE* test_file, Graph* graph) {
	int line;
	int column;

	for (int i = 0; i < graph->edges; i++) {
		if (fscanf(test_file, "%i%i", &line, &column) == EOF) {
			printf("bad number of lines");
			return false;
		}

		if (line < 1 || column < 1 ||line > graph->vertices ||column > graph->vertices) {
			printf("bad vertex");
			return false;
		}

		line -= 1;
		column -= 1;
		graph->matrix[line * graph->vertices + column].value = 1;
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

	if (graph.vertices < 0 || graph.vertices > 2000) {
		printf("bad number of vertices");
		fclose(file);
		return 0;
	}

	if (graph.edges < 0 || graph.edges > graph.vertices * (graph.vertices + 1) / 2) {
		printf("bad number of edges");
		fclose(file);
		return 0;
	}

	graph.matrix = (Node*)calloc(graph.vertices * graph.vertices, sizeof(Node));
	if (graph.matrix == NULL) {
		fclose(file);
		return 0;
	}

	int* answer = (int*)malloc(graph.vertices * sizeof(int));
	if (answer == NULL) {
		free(graph.matrix);
		fclose(file);
		return 0;
	}

	if ( input_edges(file, &graph) ) {
		if (top_sort(&graph, answer) != 0) {
			printf("impossible to sort");
		}
		else {
			for (int i = 0; i < graph.vertices; i++) {
				printf("%i ", answer[i]);
			}
		}
	}

	free(graph.matrix);
	free(answer);
	fclose(file);
	return 0;
}
