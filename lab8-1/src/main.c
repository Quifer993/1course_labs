#include <stdio.h>
#include <malloc.h>
#include <limits.h>


enum Inf{ INFINITY = -2, MAKED = -1};


typedef struct Answer {
	int from;
	int length;
}Answer;


typedef struct Node {
	int value;
}Node;


typedef struct Graph {
	Node* array;
	int n;
	int m;
}Graph;


char assembly_ostov(const Graph* graph, Answer* answer) {
	int count = 1;
	answer[0].length = MAKED;
	answer[0].from = 0;
	Answer min;
	min.length = INFINITY;
	min.from = 0;

	while (count < graph->n) {
		char used = 0;
		for (int j = 0; j < graph->n; j++) {
			if (graph->array[min.from * graph->n + j].value > 0 && answer[j].length != MAKED) {
				if (graph->array[min.from * graph->n + j].value < answer[j].length || answer[j].length == INFINITY) {
					answer[j].length = graph->array[min.from * graph->n + j].value;
					answer[j].from = min.from;
				}
			}
		}
	
		min.length = INFINITY;
		min.from = 0;
		for (int i = 0; i < graph->n; i++) {
			if ( (answer[i].length > 0 && answer[i].length < min.length) || (min.length == INFINITY && answer[i].length > 0)) {
				if (answer[i].length < answer[min.from].length || min.from == 0) {
					min.from = i;
				}
				used = 1;
			}
		}

		answer[min.from].length = MAKED;
		if (used == 0) {
			return 0;
		}
		count++;
	}

	return 1;

}


int prim(FILE * test_file, Graph * graph, Answer * answer) {
	int line;
	int column;
	int weight;
	for (int i = 0; i < graph->m; i++) {
		if (fscanf(test_file, "%i%i%i", &line, &column, &weight) == EOF) {
			printf("bad number of lines");
			return -1;
		}

		if (line < 1 || column < 1 || line > graph->n || column > graph->n) {
			printf("bad vertex");
			return -1;
		}

		if (weight > INT_MAX || weight < 0) {
			printf("bad length");
			return -1;
		}

		line -= 1;
		column -= 1;

		graph->array[line * graph->n + column].value = weight;
		graph->array[column * graph->n + line].value = weight;
	}

	char is_create = assembly_ostov(graph, answer);
	return is_create;
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

	Answer* answer = (Answer*)malloc((graph.n) * sizeof(Answer));
	if (answer == NULL) {
		free(graph.array);
		fclose(file);
		return 0;
	}
	for (int i = 0; i < graph.n; i++) {
		answer[i].length = INFINITY;
	}

	int is_spanning = prim(file, &graph, answer);
	if (is_spanning == 0) {
		printf("no spanning tree");
	}
	else {
		if (is_spanning == 1) {
			for (int i = 1; i < graph.n; i++) {
				printf("%i %i\n", answer[i].from + 1, i + 1);
			}
		}
	}

	free(graph.array);
	free(answer);
	fclose(file);
	return 0;
}
