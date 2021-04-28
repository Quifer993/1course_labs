#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>


enum Color { WHITE, GRAY, BLACK };


typedef struct Node {
	int value;
}Node;


typedef struct Graph {
	Node* array;
	int n;
	int m;//color

}Graph;


bool check_line(int* array_maked, const Graph* graph, int* pointer, int line, int * answer) {
	bool is_cycle = false;
	if (array_maked[line] != GRAY) {
		for (int j = 0; j < graph->n && !is_cycle; j++) {//������
			if (graph->array[line * graph->n + j].value == 1) {
				if (array_maked[j] != BLACK) {
					array_maked[line] = GRAY;
					is_cycle = check_line(array_maked, graph, pointer, j, answer);
				}
			}

		}
	}
	else {
		is_cycle = true;
	}
	array_maked[line] = BLACK;
	if (*pointer >= 0 ) {
		answer[(*pointer)--] = line + 1;
	}
	return is_cycle;
}


bool has_cycle(const Graph* graph, int* array_maked, int* answer) {
	bool is_cycle = false;

	int pointer = graph->n - 1;
	for (int i = 0; i < graph->n && !is_cycle; i++) {//�������
		if (array_maked[i] == WHITE) {
			is_cycle = check_line(array_maked, graph, &pointer, i, answer);
		}
	}

	return is_cycle;
}


int top_sort(FILE* test_file, Graph* graph, int* answer) {
	int* array_maked;
	int line;
	int column;
	array_maked = (int*)calloc(graph->n, sizeof(int));
 	if (array_maked == NULL) {
		return -1;
	}

	for (int i = 0; i < graph->m; i++) {
		if (fscanf(test_file, "%i%i", &line, &column) == EOF) {
			printf("bad number of lines");
			free(array_maked);
			return -1;
		}

		if (line < 1 || column < 1 ||line > graph->n ||column > graph->n) {
			printf("bad vertex");
			free(array_maked);
			return -1;
		}

		line -= 1;
		column -= 1;

		graph->array[line * graph->n + column].value = 1;
	}

	bool is_cycle = has_cycle(graph, array_maked, answer);
	free(array_maked);
	return is_cycle;
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

	if (graph.n < 0 || graph.n > 2000) {
		printf("bad number of vertices");
		fclose(file);
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


	int* answer = (int*)malloc(graph.n * sizeof(int));
	int is_cycle = top_sort(file, &graph, answer);
	if (is_cycle > 0) {
		printf("impossible to sort");
	}
	else if(is_cycle == 0){
		for (int i = 0; i < graph.n; i++) {
			printf("%i ", answer[i]);
		}
	}

	free(graph.array);
	free(answer);
	fclose(file);
	return 0;
}
