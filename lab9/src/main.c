#include <stdio.h>
#include <malloc.h>
#include <limits.h>


enum WorkWithOstov { INT_MAXX = -3, MAKED = -2, INFINITY = -1, WORKS = 0, OK = 1, OVERFLOW = 2, ERROR = 3, NO_PATH = 4 };


typedef struct Answer {
	int from;
	unsigned int length;
	char type;
}Answer;


typedef struct Node {
	unsigned int value;
}Node;


typedef struct Graph {
	Node* array;
	int n;
	int m;
}Graph;


char assembly_ostov(int from, int in, const Graph* graph, Answer* answer) {
	int count = 1;
	int count_int_max = 0;
	answer[from].length = 0;
	answer[from].from = from;
	answer[from].type = MAKED;
	Answer min = {from, 0, INFINITY};

	while (count < graph->n) {
		char used = 0;
		for (int j = 0; j < graph->n; j++) {
			if (graph->array[min.from * graph->n + j].value > 0 && answer[j].type > MAKED) {
				if (graph->array[min.from * graph->n + j].value + answer[min.from].length <= answer[j].length || answer[j].type == INFINITY) {
					answer[j].length = graph->array[min.from * graph->n + j].value + answer[min.from].length;
					if (answer[j].length > INT_MAX) {
						answer[j].type = OVERFLOW;
					}
					else {
						answer[j].type = WORKS;			
					}
					answer[j].from = min.from;
				}
				if (graph->array[min.from * graph->n + j].value + answer[min.from].length > INT_MAX) {
					count_int_max++;
				}
			}
		}

		min.length = 0;
		min.type = INFINITY;
		min.from = from;
		for (int i = 0; i < graph->n; i++) {
			if (answer[i].length > 0 && (answer[i].length < min.length || min.type == INFINITY) && answer[i].type != MAKED) {
				if (answer[i].length <= answer[min.from].length || min.from == from) {
					min.from = i;
					min.type = WORKS;
					min.length = answer[i].length;
				}
				used = 1;
			}
		}

		if (min.length > INT_MAX) {
			answer[min.from].length = 2147483648; // INT_MAX + 1
			answer[min.from].type = INT_MAXX;
		}
		else {
			answer[min.from].length = min.length;
			answer[min.from].type = MAKED;
		}

		if (used == 0) {
			if (from != in) {
				return NO_PATH;
			}
			else {
				return OK;
			}
		}
		count++;
	}

	if (count_int_max > 1 && (answer[in].type == OVERFLOW || answer[in].type == INT_MAXX) )
		return OVERFLOW;

	int point = in;
	while (answer[point].from != from) {
		if (count < 0) {
			return NO_PATH;
		}
		point = answer[point].from;
		count--;
	}

	return OK;
}


int Deikstra(int from, int in, FILE* test_file, Graph* graph, Answer* answer) {
	int line;
	int column;
	unsigned int weight;
	for (int i = 0; i < graph->m; i++) {
		if (fscanf(test_file, "%i%i%u", &line, &column, &weight) == EOF) {
			printf("bad number of lines");
			return ERROR;
		}

		if (line < 1 || column < 1 || line > graph->n || column > graph->n) {
			printf("bad vertex");
			return ERROR;
		}

		if (weight > INT_MAX) {
			printf("bad length");
			return ERROR;
		}

		line -= 1;
		column -= 1;

		graph->array[line * graph->n + column].value = weight;
		graph->array[column * graph->n + line].value = weight;
	}

	char is_create = assembly_ostov(from, in, graph, answer);
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
	
	int from;
	int in;
	if (fscanf(file, "%i%i", &from, &in) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (from < 1 || in < 1 || from > graph.n || in > graph.n) {
		printf("bad vertex");
		fclose(file);
		return 0;
	}

	in--;
	from--;

	if (fscanf(file, "%i", &graph.m) == EOF) {
		printf("bad number of lines");
		fclose(file);
		return 0;
	}

	if (graph.n <= 0 || graph.n > 5000) {
		if (graph.n == 0) {
			printf("oo");
		}
		else {
			printf("bad number of vertices");
		}
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

	Answer* answer = (Answer*)malloc((graph.n) * sizeof(Answer));
	if (answer == NULL) {
		free(graph.array);
		fclose(file);
		return 0;
	}
	for (int i = 0; i < graph.n; i++) {
		answer[i].type = INFINITY;
		answer[i].length = 0;
	}

	int is_spanning = Deikstra(from, in, file, &graph, answer);

	if (is_spanning != ERROR) {
		for (int i = 0; i < graph.n; i++) {
			if (answer[i].type == OVERFLOW || answer[i].type == INT_MAXX) {
				printf("INT_MAX+ ");
			}
			else if (answer[i].type == INFINITY) {
				printf("oo ");
			}
			else {
				printf("%u ", answer[i].length);
			}
		}
		printf("\n");

		if (is_spanning == NO_PATH) {
			printf("no path");

		}
		else if (is_spanning == OK) {
			int point = in;
			printf("%i ", in + 1);
			while (answer[point].from != from) {
				printf("%i ", answer[point].from + 1);
				point = answer[point].from;
			}
			printf("%i", from + 1);
		}
		else {
			printf("overflow");
		}
	}

	free(graph.array);
	free(answer);
	fclose(file);
	return 0;
}
